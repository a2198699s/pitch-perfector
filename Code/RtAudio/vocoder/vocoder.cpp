#include <cstdlib>
#include <fstream>
#include <iostream>
#include "vocoder.h"
#include <fftw3.h>

//add a peak finder to this class?

vocoder::vocoder(int samplerate_input, int bufferSize_input, float* scaleFreqs_input) {
  this->samplerate = samplerate_input;
  this->scaleFreqs = scaleFreqs_input;
  this->bufferSize = bufferSize_input;
  //hertz per sample?
  this->FreqRes = samplerate/bufferSize;
};


static int vocoder::binary_search(const float* NotesInKey, float* note, int highest_index, int lowest_index) {
//recursive binary searching

  int midpoint = (lowest_index + highest_index)/2;

  //could give a rounding error here that means some frequencies are never evaluated? ie freqs that fall between the gaps of the catchment bins
  if (*note > (NotesInKey[midpoint] - ((NotesInKey[midpoint])-(NotesInKey[midpoint-1]))/2)  && *note < ((NotesInKey[midpoint]) + ((NotesInKey[midpoint+1]) - (NotesInKey[midpoint]))/2)) {
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

// uses binary search to find nearest note and catches initial edge cases - could be made into a method for the vocoder class
static int vocoder::noteFinder(const float* NotesInKey, float* note) {

  //initial values for recursion
  int highest_index = (sizeof(*NotesInKey)/sizeof(float))-1;
  int lowest_index = 0;

  //edge cases
  if (*note < NotesInKey[lowest_index]) {
    return lowest_index;
  };
  if (*note > NotesInKey[highest_index]) {
    return highest_index;
  };

  //use binary search to find nearest note
  return binary_search(NotesInKey, note, highest_index, lowest_index);
};


float vocoder::SampleToFreq(int sample) {
  float freq = this->FreqRes * sample;
  return freq;
};

int vocoder::NearestNote(float* freq) {
  //find nearest note for and distance to it
  //use binary search since list of frequencies is ordered! https://www.geeksforgeeks.org/find-closest-number-array/

  int newFrequency = noteFinder(C_Major, freq);
  return newFrequency;
};



// for sure this needs cleaning up
void vocoder::pitchShift_setup(fftw_complex* fft_spectrum) {
  this->FourierTransform = fft_spectrum;

  //find sample no of highest peak excluding first sample(DC component)

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!NEED TO CHECK THIS IS USING THE REAL COMPONENTS AND NOT THE COMPLEX ONES!!!!!!!!!!!!!!!!!!!!!!!
  this->baseSample = distance(FourierTransform, max_element(FourierTransform, FourierTransform + (sizeof(FourierTransform)/sizeof(FourierTransform[0]))));

  // find freqency of highest peak
  this->baseFreq = SampleToFreq(baseSample);

  // find nearest note and distance to it
  //THIS GIVES AN INDEX!!!
  this->newFreq = C_Major[NearestNote(&baseFreq)];
  float difference = (this->newFreq) - (this->baseFreq);
  //how many bins is this??
  //NEED to round this to int...
  int binDifference = (int) difference*(this->FreqRes);

  //output note here?
  //std::cout << difference << '\n' << binDifference;

  //Find all peaks to preserve the envelope
  //peaks are defined as larger than the 2 bins on either side??

};


void vocoder::pitchShift(int binDifference) {
  //perform pitch shift
  //without using phase vocoding this will distort signals but might be ok since adjuctments are small :)

  int size = sizeof(*FourierTransform)/sizeof(*FourierTransform[0]);

  //alternatively use a pointer reference and edit that to change where the fft is read from to change index? more efficient
  if (binDifference <= 0) {
    FourierTransform = FourierTransform+binDifference;
    for (int i = size-binDifference; i < size; i++) {
      *FourierTransform[i] = 0;
    };
  }
  else {
    FourierTransform = FourierTransform-binDifference;
    for (int i = 0; i < 0-binDifference ; i++) {
      *FourierTransform[i] = 0;
    };
  };
};

};
