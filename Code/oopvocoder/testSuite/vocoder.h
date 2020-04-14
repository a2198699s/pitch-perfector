#include <cstdlib>
#include <algorithm>
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
    int binDifference;
    //string Note;
    fftw_complex* FourierTransform;

    vocoder(int samplerate_input, int bufferSize_input, const float* scaleFreqs_input);
    static int binary_search(const float* NotesInKey, float* note, int highest_index, int lowest_index);
    static int noteFinder(const float* NotesInKey, float* note);
    float SampleToFreq(int sample);
    //int NearestNote(float* freq);
    void pitchShift_setup(fftw_complex* fft_spectrum);
    void pitchShift();
};
