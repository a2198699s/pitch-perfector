#include <fftw3.h>

class vocoder {
  public:
    float baseFreq;
    int baseSample;
    int samplerate;
    int bufferSize;
    float FreqRes;
    const float* scaleFreqs;
    float newFreq;
    float difference;
    int binDifference;
    fftw_complex* FourierTransform;
    double* RealFourier;

    vocoder(int samplerate_input, int bufferSize_input, const float* scaleFreqs_input);
    float binary_search(const float* NotesInKey, float* note, int highest_index, int lowest_index);
    float noteFinder(const float* NotesInKey, float* note);
    float SampleToFreq(int sample);
    void pitchShift_setup(fftw_complex* fft_spectrum);
    void pitchShift();
};
