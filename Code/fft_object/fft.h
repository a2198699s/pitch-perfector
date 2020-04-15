#include <RtAudio.h>
// # include "vocoder.h"
const int AUDIO_BUFFER_SIZE = 512;
const int FFT_BUFFER_SIZE = AUDIO_BUFFER_SIZE/2 + 1;   

class fft {        

  public: 
    int nBufferFrames;
    double in[AUDIO_BUFFER_SIZE];
    fftw_complex out[FFT_BUFFER_SIZE];
    fftw_complex inverse_in[FFT_BUFFER_SIZE];
    double inverse_out[AUDIO_BUFFER_SIZE];
    fftw_plan my_plan;
    fftw_plan inverse_plan;
    int frequencyResolution;


    fft(int nBufferFrames);
    void executefft(double* inputBuffer);
    void executeInverse_fft(fftw_complex* fourierSpectrum);
    int peakFinder(fftw_complex* fourierSpectrum);
    double* removeComplexPart(fftw_complex* fourierSpectrum, int size);
    double findClosestNote(double notes[], int n, double target);
    double getClosest(double val1, double val2, double target);
    int FrequencyToIndex(double frequency);
    void shiftFrequencySpectrum(int shift);
};


class Dispatch  {
  public:
    fft* fourierPtr;
    // Vocoder* vocodePtr;

    Dispatch(fft* fourier_obj);
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data);
};


