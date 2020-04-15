#include <RtAudio.h>
#include <fftw3.h>

#pragma once

class fft {
  public:
    int nBufferFrames;
    double* in;
    fftw_complex *out;
    fftw_complex* inverse_in;
    double* inverse_out;
    fftw_plan my_plan;
    fftw_plan inverse_plan;

    fft(int nBufferFrames);
    void executefft(double* inputBuffer);
    void executeInverse_fft(fftw_complex* fourierSpectrum);
};
