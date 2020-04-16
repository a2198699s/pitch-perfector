/** \class vocoder
* \brief Performs the analysis and pitch sifting of input frequencies
*
* vocoder objects recieve frequency spectrums from the fft class and
* analyse them to find the fundamental frequency. Then the frequency
* spectrum is shifted to move the fundamental frequency to one of the
* notes in the key provided to the object's constructor.
*/


#include <fftw3.h>

#pragma once

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
