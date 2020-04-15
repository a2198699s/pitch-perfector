#include "fft.h"
#include <RtAudio.h>

class Dispatch  {
  public:
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data);
};
