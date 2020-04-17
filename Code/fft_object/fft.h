#pragma once

#include <RtAudio.h>
#include <fftw3.h>
#include "voxFilter.h"

const int AUDIO_BUFFER_SIZE = 512;
const int FFT_BUFFER_SIZE = AUDIO_BUFFER_SIZE/2 + 1;   

class VoxFilter;
class fft {
  public:
    int nBufferFrames;
    double in[AUDIO_BUFFER_SIZE];
    fftw_complex out[FFT_BUFFER_SIZE];
    fftw_complex inverse_in[FFT_BUFFER_SIZE];
    double inverse_out[AUDIO_BUFFER_SIZE];
    int samplingRate;
    fftw_plan my_plan;
    fftw_plan inverse_plan;
    VoxFilter* voxFilter;

    fft(int nBufferFrames, int samplingRate);
    void executefft(double* inputBuffer);
    void executeInverse_fft(fftw_complex* fourierSpectrum);
};

