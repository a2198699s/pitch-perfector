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
ofstream inputFile("input_data.txt");


int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data )
{
  fftw_complex *out;
  double *in;
  fftw_plan my_plan;
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
  unsigned long *bytes = (unsigned long *) data;
  double *audio_in  = (double *) inputBuffer;
  int signedNBufferFrames = (int) nBufferFrames;

  in = (double *) fftw_malloc(sizeof(double)*signedNBufferFrames);
  out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*signedNBufferFrames);
  my_plan = fftw_plan_dft_r2c_1d(signedNBufferFrames, in, out, FFTW_ESTIMATE  );

  memcpy(in, audio_in, *bytes);
  fftw_execute(my_plan);

  for (int x=0; x < nBufferFrames; x++) {
      inputFile << audio_in[x] << "\n";
  }
  for (int x=0; x < nBufferFrames; x++) {
        outputFile << *out[x] << " ";
  }
  outputFile << "\n" << endl;

  /// Destroys fft plan each iteration. Needs to be removed.
  fftw_destroy_plan(my_plan);
  fftw_free(in); fftw_free(out);

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
  unsigned int bufferBytes, bufferFrames = 1024;
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = 3; // first available device
  iParams.nChannels = 1;
  oParams.deviceId = 0; // first available device
  oParams.nChannels = 1;

  try {
    adac.openStream( &oParams, &iParams, RTAUDIO_FLOAT64, 44100, &bufferFrames, &inout, (void *)&bufferBytes );
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
