#include "RtAudio.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <fftw3.h>
#include "vocoder.h"
#include "dispatch.h"
#include "fft.h"

using namespace std;

// ofstream outputFile("audio_out.txt");
// ofstream inputFile("input_data.txt");

//C major scale starting at C4 in Hertz (does this need to be referenced differently as a constant?)
const float C_Major[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};


int main() {

//set up rtaudio
 RtAudio adac;
  if ( adac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }
  // Set the same number of channels for both input and output.
  unsigned int bufferBytes, bufferFrames = 1024; // samples/Fs = bufferTime
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = 0; // first available device
  iParams.nChannels = 1;
  oParams.deviceId = 1; // first available device
  oParams.nChannels = 1;

//Instantiate Classes
  int signed_bufferFrames = (int) bufferFrames;
  fft fourier = fft(signed_bufferFrames);
  vocoder vocode = vocoder(44100, signed_bufferFrames, C_Major);
  dispatch dispatcher = dispatch(&fourier, &vocode);

//start the stream
  try {
    adac.openStream( &oParams, &iParams, RTAUDIO_FLOAT64, 44100, &bufferFrames, &dispatch::caller , (void*)&dispatcher );
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
  // outputFile.close();
  // inputFile.close();

  //Delete our own objects and clean up here!!

  return 0;
}
