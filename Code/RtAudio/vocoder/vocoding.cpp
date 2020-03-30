#include "RtAudio.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <fftw3.h>

using namespace std;

ofstream outputFile("audio_out.txt");
ofstream inputFile("input_data.txt");

//C major scale starting at C4 in Hertz (does this need to be referenced differently as a constant?)
const float C_Major[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

//make structure to hold pointers to multiple objects and pass pointer to structure
//void* ObjectPointers = {&, &, &};


class fft {
  public:
    int nBufferFrames;
    double *in;
    fftw_complex *out;
    fftw_complex* inverse_in;
    double* inverse_out;
    fftw_plan my_plan;
    fftw_plan inverse_plan;

    fft(int nBufferFrames) {
      in = (double *) fftw_malloc(sizeof(double)*nBufferFrames);
      out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*nBufferFrames);
      my_plan = fftw_plan_dft_r2c_1d(nBufferFrames, in, out, FFTW_MEASURE);

      inverse_in = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*nBufferFrames);
      inverse_out = (double *) fftw_malloc(sizeof(double)*nBufferFrames);
      inverse_plan = fftw_plan_dft_c2r_1d(nBufferFrames, inverse_in, inverse_out, FFTW_MEASURE);

      this->nBufferFrames = nBufferFrames; //need to set as object variable or it wont exist outwith this function!
    };

    void executefft(double *inputBuffer) {
      //find datasize in the constructor to reduce runtime calculations in stream
      memcpy(in, inputBuffer, sizeof(double)*nBufferFrames );
      fftw_execute(my_plan);
    };

    void executeInverse_fft(fftw_complex* fourierSpectrum){
      memcpy(inverse_in, fourierSpectrum, sizeof(fftw_complex)*nBufferFrames);
      fftw_execute(inverse_plan);
    }
};

// class inverse_fft {
//   // best to inherit a base fftclass with an abstract transform method?
//   // will add as seperate plan in fft class and add an execute inverse fft method

// };

//issue with void ptr probably, will need to check in testting!
class vocoder {
  public:
    float baseFreq;
    int baseSample;
    int samplerate;
    int bufferSize;
    int binDifference;
    float FreqRes;
    void* scaleFreqs;
    float newFreq;
    string Note;
    fftw_complex* FourierTransform;

    vocoder(int samplerate_input, int bufferSize_input, void* scaleFreqs_input) {
      this->samplerate = samplerate_input;
      this->scaleFreqs = scaleFreqs_input;
      this->bufferSize = bufferSize_input;
      //hertz per sample?
      this->FreqRes = samplerate/bufferSize;
    }


    static int binary_search(const float* NotesInKey, float* note, int highest_index, int lowest_index) {
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
    static int noteFinder(const float* NotesInKey, float* note) {

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


    float SampleToFreq(int sample) {
      float freq = this->FreqRes * sample;
      return freq;
    };

    // string FreqToNote(float Freq) {
    //   //convert frequncy to a numeric note

    //   return note
    // };

    int NearestNote(float* freq) {
      //find nearest note for and distance in which direction direction...
      //use binary search since list of frequencies is ordered! https://www.geeksforgeeks.org/find-closest-number-array/

      int newFrequency = noteFinder(C_Major, freq);
      return newFrequency;

    };

    //int peakfinder(fftw_complex* fftspect){
    //};

    // for sure this needs cleaning up
    void pitchShift_setup(fftw_complex* fft_spectrum) {
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
      binDifference = (int) difference*(this->FreqRes);

      //output note here?
      //std::cout << difference << '\n' << binDifference;

      //Find all peaks to preserve the envelope
      //peaks are defined as larger than the 2 bins on either side??

    };


    void pitchShift() {
      //perform pitch shift

      //without using phase vocoding this will distort signals but might be ok since adjuctments are small :)

      // //sudo code here:
      int size = sizeof(*FourierTransform)/sizeof(*FourierTransform[0]);
      // int zero_array[binDifference] = {0}; //for length of bindifference!
      // if (binDifference <= 0) {
      //   memcpy(*FourierTransform,*FourierTransform[binDifference:end]+zero_array, size);
      // }
      // else {
      //   memcpy(*FourierTransform,[zero_array]+*FourierTransform[0:end-binDifference]);
      // };

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


class dispatch  {
  public:
    fft* fourierPtr;
    vocoder* vocodePtr;

    dispatch(fft* fourier_obj, vocoder* vocoder_obj){
      fourierPtr = fourier_obj;
      vocodePtr = vocoder_obj;
    };

    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) {
      dispatch* dispatchPtr = (dispatch*) data;
      fft *fourier = (fft *) dispatchPtr->fourierPtr;
      vocoder* vocode = (vocoder*) dispatchPtr->vocodePtr;

      double *input = (double *) inputBuffer;
      fourier->executefft(input);

      //write fft to file
      for (int x=0; x < nBufferFrames; x++) {
        inputFile << *(fourier->out[x]) << ", ";
      }
      inputFile << "\n";

      vocode->pitchShift_setup(fourier->out);
      vocode->pitchShift();

      fourier->executeInverse_fft(vocode->FourierTransform);

      //return audio to output buffer for playback
      memcpy(outputBuffer, fourier->inverse_out, sizeof(double)*nBufferFrames);
      return 0;
    };
};


int main() {

 RtAudio adac;
  if ( adac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }
  // Set the same number of channels for both input and output.
  unsigned int bufferBytes, bufferFrames = 512; // samples/Fs = bufferTime
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = 3; // first available device
  iParams.nChannels = 1;
  oParams.deviceId = 0; // first available device
  oParams.nChannels = 1;

  // Instantiate FFT Class (and others)
  //do we need a dispatch object?
  int signed_bufferFrames = (int) bufferFrames;
  fft fourier = fft(signed_bufferFrames);
  vocoder vocode = vocoder(44100, bufferFrames, (void*) C_Major);
  dispatch dispatcher = dispatch(&fourier, &vocode);


  try {
    adac.openStream( &oParams, &iParams, RTAUDIO_FLOAT64, 44100, &bufferFrames, &dispatch::caller , (void *)&dispatcher );
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
    exit( 0 );
  }
  bufferBytes = bufferFrames * 2 * 4;
  try {
    adac.startStream();
    char input;
    std::cout << "\nRunning ... press <enter> to quit.\n";
    std::cin.get(input);
    // Stop the stream.
    adac.stopStream();
  }
  catch ( RtAudioError& e ) {
    e.printMessage();
    goto cleanup;
  }
 cleanup:
  if ( adac.isStreamOpen() ) adac.closeStream();
  outputFile.close();
  inputFile.close();

  //Delete our own objects and clean up here!!

  return 0;
}
