#include "fft.h"
#include "vocoder.h"
#include <RtAudio.h>
#include <cstdlib>
#include <cstring>

class dispatch  {
  public:
    fft* fourierObj;
    Vocoder* vocoderObj;
    double fftAudioIn[512];
    fftw_complex* fftSpectrum;
    double* fftInverseOut;
    std::string currentNote;
    
    dispatch(fft* fourierPtr, Vocoder* vocoderPtr);
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data);
};
