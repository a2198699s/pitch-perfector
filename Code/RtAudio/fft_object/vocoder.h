#ifndef VOCODER_H
#define VOCODER_H

#include <cstdlib>
#include <fftw3.h>
#include <algorithm>
using namespace std;

class Vocoder {
  public:
    float baseFreq;
    int baseSample;
    int samplerate;
    int bufferSize;
    int binDifference;
    float FreqRes;
    const float* scaleFreqs;
    float newFreq;
    string Note;
    fftw_complex* FourierTransform;

    Vocoder(int samplerate_input, int bufferSize_input, const float scaleFreqs_input[]);
    int binary_search(const float* NotesInKey, float* note, int highest_index, int lowest_index);
    int noteFinder(const float* NotesInKey, float* note);
    float SampleToFreq(int sample);
    int NearestNote(float* freq);
    void pitchShift_setup(fftw_complex* fft_spectrum);
    void pitchShift();

};

#endif // VOCODER_H