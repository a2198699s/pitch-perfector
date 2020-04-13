// duplex.cpp

#include <RtAudio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring> 
using namespace std;

ofstream outputFile("audio_out.txt");


// Pass-through function.
int scale(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data)
{

  // Note: do nothing here for pass through.
  double *my_buffer = (double *) outputBuffer;

  // Scale input data for output.
  for (int i=0; i<nBufferFrames; i++) {
    // Do for two channels.
    *my_buffer++ *= 0.5;
    *my_buffer++ *= 0.5;
  }

  float *temp_buffer  = (float *) inputBuffer;

  outputFile << my_buffer[0]  << "\n";

  ofstream outputFile("audio_out.txt");


  memcpy( outputBuffer, my_buffer, 512 );
  return 0;
}

int main()
{
  // int channels = 2;
  // int sampleRate = 44100;
  // int bufferSize = 256;  // 256 sample frames
  // int nBuffers = 4;      // number of internal buffers used by device
  // int device = 0;        // 0 indicates the default or first available device
  char input;
  RtAudio *audio = 0;

  // Open a stream during RtAudio instantiation
  try {
    audio = new RtAudio();
    if ( audio->getDeviceCount() < 1 ) {
        std::cout << "\nNo audio devices found!\n";
        exit( 0 );
  }
    // Set the same number of channels for both input and output.
    unsigned int bufferBytes, bufferFrames = 512;
    RtAudio::StreamParameters iParams, oParams;
    iParams.deviceId = 3; // first available device
    iParams.nChannels = 2;
    oParams.deviceId = 1; // first available device
    oParams.nChannels = 2;
    audio->openStream( &oParams, &iParams, RTAUDIO_SINT32, 44100, &bufferFrames, &scale, (void *)&bufferBytes );
  }
  catch (RtAudioError &error) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }

  try {

    // Start the stream
    audio->startStream();
  }
  catch (RtAudioError &error) {
    error.printMessage();
    goto cleanup;
  }

  std::cout << "\nRunning duplex ... press <enter> to quit.\n";
  std::cin.get(input);

  try {
    // Stop and close the stream
    audio->stopStream();
    audio->closeStream();
  }
  catch (RtAudioError &error) {
    error.printMessage();
  }

 cleanup:
  delete audio;
  outputFile.close();

  return 0;
}