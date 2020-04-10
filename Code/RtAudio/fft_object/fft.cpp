#include <fftw3.h>
#include <cstring>
#include <RtAudio.h>
#include <algorithm>
#include "fft.h"
#include "vocoder.h"


fft::fft(int nBufferFrames) {
    in = (double *) fftw_malloc(sizeof(double)*nBufferFrames);
    out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*nBufferFrames);
    my_plan = fftw_plan_dft_r2c_1d(nBufferFrames, in, out, FFTW_MEASURE);

    inverse_in = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*nBufferFrames);
    inverse_out = (double *) fftw_malloc(sizeof(double)*nBufferFrames);
    inverse_plan = fftw_plan_dft_c2r_1d(nBufferFrames, inverse_in, inverse_out, FFTW_MEASURE);

    this->nBufferFrames = nBufferFrames; //need to set as object variable or it wont exist outwith this function!
};

void fft::executefft(double *inputBuffer) {
    memcpy(in, inputBuffer, sizeof(double)*nBufferFrames );
    fftw_execute(my_plan);
};

void fft::executeInverse_fft(fftw_complex* fourierSpectrum){
    memcpy(inverse_in, fourierSpectrum, sizeof(fftw_complex)*nBufferFrames);
    fftw_execute(inverse_plan);
};


double* fft::removeComplexPart(fftw_complex* fourierSpectrum, int size){
    double* realPart = (double *) fftw_malloc(sizeof(double)*size);
    for (int i=0; i<size; ++i){
        realPart[i] = fourierSpectrum[i][0];
    }
    return realPart;
}

int fft::peakFinder(fftw_complex* fourierSpectrum){
    double* realSpectrum = this->removeComplexPart(fourierSpectrum, nBufferFrames);
    int max = 0;
    int maxIndex = 0;
    double frequencyResolution = 44100/512;
    for (int i=0; i<nBufferFrames; ++i) {
        if (realSpectrum[i] > max) {
            max = realSpectrum[i];
            maxIndex = i;
        }
    }
    return maxIndex * frequencyResolution;
}

double fft::getClosest(double val1, double val2, double target) { 
    if (target - val1 >= val2 - target) 
        return val2; 
    else
        return val1; 
} 

// Adapted from https://www.geeksforgeeks.org/find-closest-number-array/
// Returns the note closest to target in notes[] 
double fft::findClosestNote(double notes[], int n, double target) { 
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



Dispatch::Dispatch(fft* fourier_obj){
    fourierPtr = fourier_obj;
    // vocodePtr = vocoder_obj;
};

// Static declared in header.
int Dispatch::caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) {
    Dispatch* dispatchPtr = (Dispatch*) data;
    fft *fourier = dispatchPtr->fourierPtr;
    // Vocoder* vocode = dispatchPtr->vocodePtr;
	double cMajor[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

    double *input = (double *) inputBuffer;
    fourier->executefft(input);
    double peakFrequency = fourier->peakFinder(fourier->out);
    // cout << peakFrequency << "\n";
    double closestNoteFrequency = fourier->findClosestNote(cMajor, 8, peakFrequency);
    float difference = closestNoteFrequency - peakFrequency;
    cout << difference << "\n";
    fourier->executeInverse_fft(fourier->out);
    memcpy(outputBuffer, fourier->inverse_out, sizeof(double)*nBufferFrames);

    // vocode->pitchShift_setup(fourier->out);
    // vocode->pitchShift();

    // fourier->executeInverse_fft(fourier->out);

    //return audio to output buffer for playback
    // memcpy(outputBuffer, fourier->inverse_out, sizeof(double)*nBufferFrames);
    return 0;
};