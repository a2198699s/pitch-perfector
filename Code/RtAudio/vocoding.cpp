#include <RtAudio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <fftw3.h>
// #include "fft.h"
using namespace std;

ofstream outputFile("audio_out.txt");
ofstream inputFile("input_data.txt");

class fft {        
  public:
    int nBufferFrames;
    double *in;
    fftw_complex *out;
    int flag;
    fftw_plan my_plan;

    fft(int nBufferFrames) {
      in = (double *) fftw_malloc(sizeof(double)*nBufferFrames);
      out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*nBufferFrames);
      my_plan = fftw_plan_dft_r2c_1d(nBufferFrames, in, out, FFTW_MEASURE);
      this->nBufferFrames = nBufferFrames;
    };

    void executefft(double *inputBuffer) {
      //find datasize in the constructor to reduce runtime calculations in stream
      memcpy(in, inputBuffer, sizeof(double)*nBufferFrames );
      fftw_execute(my_plan);
    };
};


class vocoder {
  public:
    float baseFreq;
    int samplerate;
    int bufferSize
    float FreqRes;
    int* scaleFreqs [8];
    float newFreq;
    string Note; 
    fftw_complex* FourierTransform;

    vocoder(int samplerate_input, int bufferSize_input, int* scaleFreqs_input [8]) {
      this->samplerate = samplerate_input;
      this->scaleFreqs = scaleFreqs_input;
      this->bufferSize = bufferSize_input;
      //hertz per sample?
      this->FreqRes = samplerate/bufferSize;
    };


    float SampleToFreq(int sample) {
      float freq = this->FreqRes * sample;
      return freq;
    };

    string FreqToNote(float Freq) {
      //convert frequncy to a numeric note

      return note
    };


    void pitchShift_setup(fftw_complex* fft_spectrum) {
      this->FourierTransform = fft_spectrum;
      this->baseFreq = SampleToFreq(distance(FourierTransform, max_element(FourierTransform+1, FourierTransform[bufferSize])));
      this->Note = FreqToNote(this->baseFreq);

      //peaks are defined as larger than the 2 bins on either side
      //peaks =

      //find nearest note (newFreq)

    };


    void pitchShift{
      //perform pitch shift??
    };
   
};


class dispatch  {
  public:
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) { 
      fft *fourier = (fft *) data;
      double *input = (double *) inputBuffer;
      fourier->executefft(input);

      //write fft to file
      for (int x=0; x < nBufferFrames; x++) {
        inputFile << *(fourier->out[x]) << ", ";
      }
      inputFile << "\n";

      //return audio to output buffer for playback
      memcpy(outputBuffer, inputBuffer, sizeof(double)*nBufferFrames);
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
  dispatch dispatcher;
  int signed_bufferFrames = (int) bufferFrames;
  fft fourier = fft(signed_bufferFrames);
  

  try {
    adac.openStream( &oParams, &iParams, RTAUDIO_FLOAT64, 44100, &bufferFrames, &dispatch::caller , (void *)&fourier );
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
