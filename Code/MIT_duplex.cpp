// duplex.cpp

#include <iostream>
#include "RtAudio.h"

// Pass-through function.
int scale(char *buffer, int bufferSize, void *)
{
  // Note: do nothing here for pass through.
  double *my_buffer = (double *) buffer;

  // Scale input data for output.
  for (int i=0; i<bufferSize; i++) {
    // Do for two channels.
    *my_buffer++ *= 0.5;
    *my_buffer++ *= 0.5;
  }

  return 0;
}

int main()
{
  int channels = 2;
  int sampleRate = 44100;
  int bufferSize = 256;  // 256 sample frames
  int nBuffers = 4;      // number of internal buffers used by device
  int device = 0;        // 0 indicates the default or first available device
  char input;
  RtAudio *audio = 0;

  // Open a stream during RtAudio instantiation
  try {
    audio = new RtAudio(device, channels, device, channels, RTAUDIO_FLOAT64,
                        sampleRate, &bufferSize, nBuffers);
  }
  catch (RtError &error) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }

  try {
    // Set the stream callback function
    audio->setStreamCallback(&scale, NULL);

    // Start the stream
    audio->startStream();
  }
  catch (RtError &error) {
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
  catch (RtError &error) {
    error.printMessage();
  }

 cleanup:
  delete audio;

  return 0;
}
