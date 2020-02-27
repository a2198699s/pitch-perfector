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
ofstream outData("out_data.txt");

int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data )
{
  fftw_complex *out;
  double *in;
  fftw_plan my_plan;
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
  unsigned long *bytes = (unsigned long *) data;
  double *audio_in  = (double *) inputBuffer;
  // float *audio_out = (float *) outputBuffer; 
  int signedNBufferFrames = (int) nBufferFrames;


  in = (double *) fftw_malloc(sizeof(double)*signedNBufferFrames);
  out = (fftw_complex *) fftw_malloc(sizeof(fftw_complex)*signedNBufferFrames);
  my_plan = fftw_plan_dft_r2c_1d(signedNBufferFrames, in, out, FFTW_ESTIMATE);



  memcpy(in, audio_in, *bytes);
  fftw_execute(my_plan);


    for (int x=0; x < nBufferFrames; x++) {
    outputFile << out[x][0] << " ";
  }
  outputFile << "\n" << endl;
  // fftw_free(in)
  // fftw_destroy_plan(my_plan);



  // for (int x=0; x < nBufferFrames; x++) {
  //   outputFile << in[x];
  //   outData << out[x];

  // }
  // outputFile << endl;
  // outData << endl;

  // //destroy plan and buffers?
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
  unsigned int bufferBytes, bufferFrames = 512;
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = 3; // first available device
  iParams.nChannels = 2;
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
  outData.close();
  return 0;
}