#include "fftn.h"
#include "vocodern.h"
#include <RtAudio.h>
#include <cstdlib>
#include <cstring>

class dispatch  {
  public:
    fft* fourierObj;
    vocoder* vocoderObj;
    double fftAudioIn[512];
    fftw_complex* fftSpectrum;
    double* fftInverseOut;

    dispatch(fft* fourierPtr, Vocoder* vocoderPtr);
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data);
};
