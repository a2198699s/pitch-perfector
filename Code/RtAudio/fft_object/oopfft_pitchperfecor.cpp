#include <RtAudio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <fftw3.h>
// #include "fft.h"
using namespace std;
// Pass-through function.

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
      memcpy(in, inputBuffer, sizeof(double)*nBufferFrames );
      fftw_execute(my_plan);
    };
};

class dispatch  {
  public:
    static int caller(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus status, void *data) { 
      fft *fourier = (fft *) data;
      double *input = (double *) inputBuffer;
      fourier->executefft(input);
      for (int x=0; x < nBufferFrames; x++) {
        inputFile << *(fourier->out[x]) << ", ";
      }
      inputFile << "\n";

      memcpy(outputBuffer, inputBuffer, sizeof(double)*nBufferFrames);
      return 0;
    };
};

int main()
{
 dispatch dispatcher;
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

  // Instantiate FFT Class
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
  return 0;
}
