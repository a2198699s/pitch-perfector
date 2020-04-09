#include <RtAudio.h>
// # include "vocoder.h"

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


class Dispatch  {
  public:
    fft* fourierPtr;
    // Vocoder* vocodePtr;

    Dispatch(fft* fourier_obj);
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data);
};


