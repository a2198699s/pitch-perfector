#include <fftw3.h>
#include <cstring>
#include <RtAudio.h>
#include <algorithm>
#include <math.h>
#include "fft.h"
#include "vocoder.h"
#include <functional>
#include <numeric>
#include "helper.h"


fft::fft(int nBufferFrames) {
    my_plan = fftw_plan_dft_r2c_1d(nBufferFrames, in, out, FFTW_MEASURE);
    inverse_plan = fftw_plan_dft_c2r_1d(nBufferFrames, inverse_in, inverse_out, FFTW_MEASURE);
    frequencyResolution = 86;
    this->nBufferFrames = nBufferFrames; //need to set as object variable or it wont exist outwith this function!
    double maxinput = 0;
};

void fft::executefft(double *inputBuffer) {
    memcpy(in, inputBuffer, sizeof(double)*nBufferFrames );
    fftw_execute(my_plan);
    double frequencyResolution = 44100/nBufferFrames;
    int scaler = 5;
    int cut100Hz = 100/frequencyResolution;
    int cut2000Hz = 2000/frequencyResolution;
    int cut7000Hz = 7000/frequencyResolution;
    int cut9000Hz = 9000/frequencyResolution;
    int cut10000Hz = 10000/frequencyResolution;



    // cout << cut100Hz << " " << cut2000Hz <<" " << cut7000Hz << " " << cut9000Hz << " " << cut10000Hz << "\n";

    // 0Hz to 100Hz is 0.
    for (int i=0; i<=cut100Hz; ++i) {
        this->out[i][0] = 0;
        this->out[i][1] = 0;
    }

    // 100Hz to 2000Hz is kept the same.
    for (int i=cut100Hz; i<=cut2000Hz; ++i) {
        this->out[i][0] = this->out[i][0] / scaler;
        this->out[i][1] = this->out[i][1] / scaler;
    }    

    // 2000Hz to 7000Hz is multipled by 1.5. 
    for (int i=cut2000Hz; i<=cut7000Hz; ++i) {
        this->out[i][0] = this->out[i][0] * 3 / scaler;
        this->out[i][1] = this->out[i][1] * 3 / scaler;
    }    

    // 7000Hz to 9000Hz is kept the same. 
    for (int i=cut7000Hz; i<=cut9000Hz; ++i) {
        this->out[i][0] = this->out[i][0] / scaler;
        this->out[i][1] = this->out[i][1] / scaler;
    }    

    // 9000Hz to 10000Hz is multipled by 1.5. 
    for (int i=cut9000Hz; i<=cut10000Hz; ++i) {
        this->out[i][0] = this->out[i][0] * 3 / scaler;
        this->out[i][1] = this->out[i][1] * 3 / scaler;
    }    


    // 10000Hz to END is set to 0. 
    for (int i=cut10000Hz; i<=FFT_BUFFER_SIZE; ++i) {
        this->out[i][0] = 0;
        this->out[i][1] = 0;
    }        
};

void fft::executeInverse_fft(fftw_complex* fourierSpectrum){
    memcpy(inverse_in, fourierSpectrum, sizeof(fftw_complex)*nBufferFrames);
    fftw_execute(inverse_plan);

};



// 80 to 1000Hz is vocal range. 
// 
int fft::peakFinder(){

    // double* realSpectrum = this->removeComplexPart(fourierSpectrum, nBufferFrames);
    double max = 0;
    int maxIndex = 0;

    for (int i=0; i<FFT_BUFFER_SIZE; ++i) {
        if (this->out[i][0] > max) {
            max = this->out[i][0];
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

int fft::FrequencyToIndex(double frequency) {
    float frequencyResolution = 44100/nBufferFrames;
    return round(frequency/frequencyResolution);
}

void fft::shiftFrequencySpectrum(int shift) { 
    if (abs(shift > 5) ) return;
    if (shift >= 0) {
        memmove(this->out+shift, this->out, sizeof(fftw_complex)*(nBufferFrames-shift));
    }
    else {
        memmove(this->out, this->out-shift, sizeof(fftw_complex)*(nBufferFrames+shift)); 
    }
}

Dispatch::Dispatch(fft* fourier_obj){
    fourierPtr = fourier_obj;
};

// Static declared in header.
int Dispatch::caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) {
    Dispatch* dispatchPtr = (Dispatch*) data;
    fft *fourier = dispatchPtr->fourierPtr;
	double cMajor[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
    memcpy(fourier->in, inputBuffer, sizeof(double)*AUDIO_BUFFER_SIZE);
    fourier->executefft(fourier->in);
    double peakFrequency = fourier->peakFinder();
    double closestNoteFrequency = fourier->findClosestNote(cMajor, 8, peakFrequency);
    float difference = closestNoteFrequency - peakFrequency;
    int differenceIndex = fourier->FrequencyToIndex(difference);
    cout << "Peak: " << peakFrequency << " Closest: " << closestNoteFrequency << " Difference: " << difference << " Freq Difference: " << differenceIndex << "\n";
    fourier->shiftFrequencySpectrum(differenceIndex);
    fourier->executeInverse_fft(fourier->out);
    memcpy(outputBuffer, fourier->inverse_out, sizeof(double)*nBufferFrames);

    return 0;
};