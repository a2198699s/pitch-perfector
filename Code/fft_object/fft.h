#include <RtAudio.h>
#include <fftw3.h>
const int AUDIO_BUFFER_SIZE = 512;
const int FFT_BUFFER_SIZE = AUDIO_BUFFER_SIZE/2 + 1;   
#pragma once

class fft {
  public:
    int nBufferFrames;
    double in[AUDIO_BUFFER_SIZE];
    fftw_complex out[FFT_BUFFER_SIZE];
    fftw_complex inverse_in[FFT_BUFFER_SIZE];
    double inverse_out[AUDIO_BUFFER_SIZE];
    fftw_plan my_plan;
    fftw_plan inverse_plan;

    fft(int nBufferFrames);
    void executefft(double* inputBuffer);
    void executeInverse_fft(fftw_complex* fourierSpectrum);
};

