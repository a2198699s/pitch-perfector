#include "fft.h"
#include <fftw3.h>
#include <cstring>
#include <RtAudio.h>

fft::fft(int nBufferFrames) {
    my_plan = fftw_plan_dft_r2c_1d(AUDIO_BUFFER_SIZE, in, out, FFTW_MEASURE);
    inverse_plan = fftw_plan_dft_c2r_1d(AUDIO_BUFFER_SIZE, inverse_in, inverse_out, FFTW_MEASURE);
};

void fft::executefft(double inputBuffer[AUDIO_BUFFER_SIZE]) {
    memcpy(in, inputBuffer, sizeof(double)*AUDIO_BUFFER_SIZE ); 
    fftw_execute(my_plan);
};

void fft::executeInverse_fft(fftw_complex* fourierSpectrum){
    memcpy(inverse_in, fourierSpectrum, sizeof(fftw_complex)*AUDIO_BUFFER_SIZE);
    fftw_execute(inverse_plan);
};
