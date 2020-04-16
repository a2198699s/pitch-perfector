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

//add a peak finder to this class?

Vocoder::Vocoder(int sampleRate, int bufferSize, const double* scaleFrequencies) {
    this->samplerate = sampleRate;
    this->scaleFrequencies = scaleFrequencies;
    this->bufferSize = bufferSize;
    this->frequencyResolution = (float)samplerate/(float)bufferSize;
};


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
                return this->getClosest(notes[mid - 1], 
                                  notes[mid], target); 
            /* Repeat for left half */
            j = mid; 
        } 
        // If target is greater than mid 
        else { 
            if (mid < n - 1 && target < notes[mid + 1]) 
                return this->getClosest(notes[mid], 
                                  notes[mid + 1], target); 
            // update i 
            i = mid + 1;  
        } 
    } 
    // Only single element left after search 
    return notes[mid]; 
} 



float Vocoder::SampleToFreq(int sample) {
    float freq = (this->frequencyResolution) * sample;
    return freq;
};

void Vocoder::findPeak(){
    double max = 0;
    int maxIndex = 0;

    for (int i=0; i<FFT_BUFFER_SIZE; ++i) {
        if (this->fourierSpectrum[i][0] > max) {
            max = this->fourierSpectrum[i][0];
            maxIndex = i;
        }
    }
    this->peakFrequency = maxIndex * this->frequencyResolution;
}


void Vocoder::setFourierSpectrum(fftw_complex* fftSpectrum){
    for (int i=0; i<FFT_BUFFER_SIZE; ++i) {
        this->fourierSpectrum[i][0] = fftSpectrum[i][0];
        this->fourierSpectrum[i][1] = fftSpectrum[i][1];      
    }
}



int Vocoder::FrequencyToIndex(double frequency) {
    return round(frequency/this->frequencyResolution);
}



void Vocoder::pitchShift() {
    this->findPeak();
    this->closestNoteFrequency = this->findClosestNote(this->scaleFrequencies, 8, this->peakFrequency);
    this->difference = this->closestNoteFrequency - this->peakFrequency;
    this->binDifference = this->FrequencyToIndex(difference);
    int shift = this->binDifference;

    if (abs(shift > 5) ) return;
    if (shift >= 0) {
        memmove(this->fourierSpectrum+shift, this->fourierSpectrum, sizeof(fftw_complex)*(FFT_BUFFER_SIZE-shift));
    }
    else {
        memmove(this->fourierSpectrum, this->fourierSpectrum-shift, sizeof(fftw_complex)*(FFT_BUFFER_SIZE+shift)); 
    }
}
