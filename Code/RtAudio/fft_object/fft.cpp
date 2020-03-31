#include <fftw3.h>
#include <cstring>
#include <RtAudio.h>
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

// Static declared in header.
int Dispatch::caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) { 
    fft *fourier = (fft *) data;
    double *input = (double *) inputBuffer;
    fourier->executefft(input);
    memcpy(outputBuffer, inputBuffer, sizeof(double)*nBufferFrames);
    return 0;
};