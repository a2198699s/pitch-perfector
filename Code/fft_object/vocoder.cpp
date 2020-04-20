#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "vocoder.h"
#include "fft.h"
#include <fftw3.h>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;
const double A4 = 440.0;
const double C0 = A4*pow(2, -4.75);
const string notes[12] = {"C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#", "A ", "A#", "B "};

//add a peak finder to this class?

Vocoder::Vocoder(int sampleRate, int bufferSize, const double* scaleFrequencies) : currentNote("AAA") {
    this->samplerate = sampleRate;
    this->scaleFrequencies = scaleFrequencies;
    this->bufferSize = bufferSize;
    frequencyResolution = (float)samplerate/(float)bufferSize;
};

const char* Vocoder::frequencyToNote(double freq) { 
    int stepsFromC0 = round(12*log2(freq/C0));
    int octave = stepsFromC0 / 12;
    int n = stepsFromC0 % 12;
    return (notes[n] + std::to_string(octave)).c_str();
}

double Vocoder::getClosest(double val1, double val2, double target) { 
    if (target - val1 >= val2 - target) 
        return val2; 
    else
        return val1; 
} 

// Adapted from https://www.geeksforgeeks.org/find-closest-number-array/
// Returns the note closest to target in notes[] 
double Vocoder::findClosestNote(const double notes[], int n, double target) { 
    // Corner cases 
    if (target <= notes[0]) 
        return notes[0]; 
    if (target >= notes[n - 1]) 
        return notes[n - 1]; 
  
    // Doing binary search 
    int i = 0, j = n, mid = 0; 
    while (i < j) { 
        mid = (i + j) / 2; 
  
        if (notes[mid] == target) 
            return notes[mid]; 
  
        /* If target is less than array element, 
            then search in left */
        if (target < notes[mid]) { 
  
            // If target is greater than previous 
            // to mid, return closest of two 
            if (mid > 0 && target > notes[mid - 1]) 
                return getClosest(notes[mid - 1], 
                                  notes[mid], target); 
            /* Repeat for left half */
            j = mid; 
        } 
        // If target is greater than mid 
        else { 
            if (mid < n - 1 && target < notes[mid + 1]) 
                return getClosest(notes[mid], 
                                  notes[mid + 1], target); 
            // update i 
            i = mid + 1;  
        } 
    } 
    // Only single element left after search 
    return notes[mid]; 
} 



float Vocoder::SampleToFreq(int sample) {
    float freq = (frequencyResolution) * sample;
    return freq;
};

void Vocoder::findPeak(){
    double max = 0;
    int maxIndex = 0;

    for (int i=0; i<FFT_BUFFER_SIZE; ++i) {
        if (fourierSpectrum[i][0] > max) {
            max = fourierSpectrum[i][0];
            maxIndex = i;
        }
    }
    peakFrequency = maxIndex * this->frequencyResolution;
}


void Vocoder::setFourierSpectrum(fftw_complex* fftSpectrum){
    for (int i=0; i<FFT_BUFFER_SIZE; ++i) {
        fourierSpectrum[i][0] = fftSpectrum[i][0];
        fourierSpectrum[i][1] = fftSpectrum[i][1];      
    }
}


int Vocoder::FrequencyToIndex(double frequency) {
    return round(frequency/frequencyResolution);
}

void Vocoder::pitchShift() {
    findPeak();
    closestNoteFrequency = findClosestNote(scaleFrequencies, 8, peakFrequency);
    difference = closestNoteFrequency - peakFrequency;
    binDifference = FrequencyToIndex(difference);
    // differencePointer = &difference;

    int shift = binDifference;
    const char* newNote = frequencyToNote(closestNoteFrequency);
    memcpy(currentNote, newNote, sizeof(char)*4);

    if (abs(shift > 5) ) return;
    if (shift >= 0) {
        memmove(fourierSpectrum+shift, fourierSpectrum, sizeof(fftw_complex)*(FFT_BUFFER_SIZE-shift));
    }
    else {
        memmove(fourierSpectrum, fourierSpectrum-shift, sizeof(fftw_complex)*(FFT_BUFFER_SIZE+shift)); 
    }
}
