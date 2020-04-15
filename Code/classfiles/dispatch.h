#include "fft.h"
#include "vocoder.h"
#include <RtAudio.h>

#pragma once

class dispatch  {
  public:
    fft* fourierObj;
    vocoder* vocoderObj;

    dispatch(fft* fourierPtr, vocoder* vocoderPtr);
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data);
};
