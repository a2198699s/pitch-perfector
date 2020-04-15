#include "fft.h"
#include <RtAudio.h>

int Dispatch::caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) {
    fft *fourier = (fft *) data;
    double *input = (double *) inputBuffer;
    fourier->executefft(input);
    memcpy(outputBuffer, inputBuffer, sizeof(double)*nBufferFrames);
    return 0;
};
