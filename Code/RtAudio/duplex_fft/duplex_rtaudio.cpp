#include <RtAudio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
//real 1D fft - faster but could be wrong usage 
#include <fftw3.h>
using namespace std;
// Pass-through function.

ofstream outputFile("audio_out.txt");




int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data )
{

  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
  unsigned long *bytes = (unsigned long *) data;
  float *buffer  = (float *) inputBuffer;
  float *buffer2 = (float *) outputBuffer;

  // fftw_complex *in, *out;
  // fftw_plan = my_plan
  // in = (fftw_complex*) inputBuffer;
  // fftw_complex *fft_out = (fftw_complex*) outputBuffer;

  int i = 0;
  int N = *bytes;
  fftw_complex *in, *out;
  fftw_plan my_plan;

  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
  memcpy(in, inputBuffer, *bytes);
  my_plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(my_plan);

  for (int x=0; x < N; x++) {
    outputFile << in[x];
  }
  outputFile << endl;

  memcpy( outputBuffer, inputBuffer, *bytes );
  return 0;
}
int main()
{
 RtAudio adac;
  if ( adac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }
  // Set the same number of channels for both input and output.
  unsigned int bufferBytes, bufferFrames = 512;
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = 3; // first available device
  iParams.nChannels = 1;
  oParams.deviceId = 0; // first available device
  oParams.nChannels = 1;
  try {
    adac.openStream( &oParams, &iParams, RTAUDIO_FLOAT32, 44100, &bufferFrames, &inout, (void *)&bufferBytes );
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
  return 0;
}
