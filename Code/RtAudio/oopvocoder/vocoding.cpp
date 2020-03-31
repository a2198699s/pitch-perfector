#include "RtAudio.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <fftw3.h>
#include "vocoder.h"

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
