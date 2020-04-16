#include <fftw3.h>
#include "fft.h"
#pragma once

class Vocoder {
  public:
    float baseFreq;
    int baseSample;
    int samplerate;
    int bufferSize;
    float frequencyResolution;
    const double* scaleFrequencies;
    float newFreq;
    float difference;
    int binDifference;
    fftw_complex fourierSpectrum[257];
    double peakFrequency;
    double closestNoteFrequency;

    Vocoder(int sampleRate, int bufferSize, const double* scaleFrequencies);
    float binary_search(const float* NotesInKey, float* note, int highest_index, int lowest_index);
    float noteFinder(const float* NotesInKey, float* note);
    float SampleToFreq(int sample);
    void pitchShift_setup(fftw_complex* fft_spectrum);
    void pitchShift();
    void findPeak();
    double getClosest(double val1, double val2, double target);
    double findClosestNote(const double notes[], int n, double target);
    int FrequencyToIndex(double frequency);
    void setFourierSpectrum(fftw_complex* fftSpectrum);
};
