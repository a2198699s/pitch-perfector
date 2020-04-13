#include "fft.h"
#include <fftw3.h>
#include <cstring>
#include <RtAudio.h>

fft::fft(int nBufferFrames) {
    in = (double *) fftw_malloc(sizeof(double)*nBufferFrames);
    out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*nBufferFrames);
    my_plan = fftw_plan_dft_r2c_1d(nBufferFrames, in, out, FFTW_MEASURE);
    this->nBufferFrames = nBufferFrames;
};

void fft::executefft(double *inputBuffer) {
    memcpy(in, inputBuffer, sizeof(double)*nBufferFrames );
    fftw_execute(my_plan);
};

// Static declared in header.
int Dispatch::caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) {
    fft *fourier = (fft *) data;
    double *input = (double *) inputBuffer;
    fourier->executefft(input);
    memcpy(outputBuffer, inputBuffer, sizeof(double)*nBufferFrames);
    return 0;
};
