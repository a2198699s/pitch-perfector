#include <RtAudio.h>
#include <fftw3.h>

class fft {
  public:
    int nBufferFrames;
    double *in;
    fftw_complex *out;
    int flag;
    fftw_plan my_plan;

    fft(int nBufferFrames);
    void executefft(double *inputBuffer);
};

class Dispatch  {
  public:
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data);
};
