#include <fftw3.h>

class fft {        
  public:
    int nBufferFrames;
    double *in;
    fftw_complex *out;
    int flag;
    fftw_plan my_plan;

    fft(int nBufferFrames) {
      in = (double *) fftw_malloc(sizeof(double)*nBufferFrames);
      out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*nBufferFrames);
      my_plan = fftw_plan_dft_r2c_1d(nBufferFrames, in, out, FFTW_MEASURE);
      this->nBufferFrames = nBufferFrames;
    };

    void executefft(double *inputBuffer) {
      memcpy(in, inputBuffer, sizeof(double)*nBufferFrames );
      fftw_execute(my_plan);
    };
};