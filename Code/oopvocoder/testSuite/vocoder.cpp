#include <cstdlib>
#include <algorithm>
#include "vocoder.h"
#include <fftw3.h>

using namespace std;

//add a peak finder to this class?

vocoder::vocoder(int samplerate_input, int bufferSize_input, const float* scaleFreqs_input) {
  this->samplerate = samplerate_input;
  this->scaleFreqs = scaleFreqs_input;
  this->bufferSize = bufferSize_input;
  this->FreqRes = samplerate/bufferSize;
  this->RealFourier = (double*) malloc(sizeof(double)*this->bufferSize);
};


//Recursive binary searching
float vocoder::binary_search(const float* NotesInKey, float* note, int highest_index, int lowest_index) {
  int midpoint = (lowest_index + highest_index)/2;

  //could give a rounding error here that means some frequencies are never evaluated? ie freqs that fall between the gaps of the catchment bins
  if (*note > (NotesInKey[midpoint] - ((NotesInKey[midpoint])-(NotesInKey[midpoint-1]))/2)  && *note < ((NotesInKey[midpoint]) + ((NotesInKey[midpoint+1]) - (NotesInKey[midpoint]))/2)){
    return NotesInKey[midpoint];
  }
  else {
    if (*note < NotesInKey[midpoint]) {
      highest_index = midpoint;
    };
    if (*note > NotesInKey[midpoint]) {
      lowest_index = midpoint;
    };
    return binary_search(NotesInKey, note, highest_index, lowest_index);
  };
};

//Uses binary search to find nearest note and catches initial edge cases {use binary search since list of frequencies is ordered! https://www.geeksforgeeks.org/find-closest-number-array/}
float vocoder::noteFinder(const float* NotesInKey, float* note) {
  //initial values for recursion
  int highest_index = 7;
  int lowest_index = 0;

  //corner cases
  if (*note < NotesInKey[lowest_index]) {
    return NotesInKey[lowest_index];
  };
  if (*note > NotesInKey[highest_index]) {
    return NotesInKey[highest_index];
  };

  //use binary search to find nearest note
  return binary_search(NotesInKey, note, highest_index, lowest_index);
};


float vocoder::SampleToFreq(int sample) {
  float freq = this->FreqRes * sample;
  return freq;
};


// for sure this needs cleaning up
void vocoder::pitchShift_setup(fftw_complex* fft_spectrum) {
  this->FourierTransform = fft_spectrum;
  //take real part
  for (int i = 0; i < this->bufferSize; i++){
    this->RealFourier[i] = fft_spectrum[i][0];
  };

  //find sample no of highest peak excluding first sample(DC component)
  this->baseSample = distance(this->RealFourier, max_element(this->RealFourier, this->RealFourier + this->bufferSize));

  // find freqency of highest peak
  this->baseFreq = SampleToFreq(this->baseSample);

  // find nearest note and distance to it
  //THIS GIVES AN INDEX!!!
  this->newFreq = noteFinder(this->scaleFreqs, &this->baseFreq);
  this->difference = (this->newFreq) - (this->baseFreq);
  //how many bins is this??
  //NEED to round this to int...
  this->binDifference = (int) difference/(this->FreqRes);

  //output note here?
  //std::cout << difference << '\n' << binDifference;

  //Find all peaks to preserve the envelope
  //peaks are defined as larger than the 2 bins on either side??

};


void vocoder::pitchShift() {
  //perform pitch shift
  //without using phase vocoding this will distort signals but might be ok since adjuctments are small :)

  //int size = sizeof(*FourierTransform)/sizeof(*FourierTransform[0]);

  //alternatively use a pointer reference and edit that to change where the fft is read from to change index? more efficient
  if (binDifference <= 0) {
    FourierTransform = FourierTransform+this->binDifference;
    for (int i = bufferSize-this->binDifference; i < bufferSize; i++) {
      *FourierTransform[i] = 0;
    };
  }
  else {
    FourierTransform = FourierTransform-this->binDifference;
    for (int i = 0; i < 0-this->binDifference ; i++) {
      *FourierTransform[i] = 0;
    };
  };
};
