#include "fft.h"
#include "vocoder.h"
#include <RtAudio.h>

class Dispatch  {
  public:
    fft* fourierObj
    vocoder* vocoderObj

    Dispatch(fft* fourierPtr, vocoder* vocoderPtr);
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data);
};
