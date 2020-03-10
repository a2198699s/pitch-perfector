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
