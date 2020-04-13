#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <fftw3.h>
#include "vocoder.h"
#include "fft.h"

using namespace std;

ofstream timesignal("timesig.txt");
ofstream spectrumIn("spectIn.txt");
ofstream spectrumOut("spectOut.txt");

//test Key
const float C_Major[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

//generate sin waves and create sample dataset
const int buffersize = 50;

double* sinGen(){
  static double sinwave[buffersize] = {0};
  for (int i=0; i<buffersize; i++) {
    sinwave[i] = (double) (10*sin(i * 1)); //+ 40*sin(i * 50) + 60*sin(i*450) + 10*sin(i * 530));
  }
  return sinwave;
};

int main(){
  //set up
  double* testSignal = (double*) malloc(sizeof(double*)*(buffersize));
  testSignal = sinGen();
  fft fourier = fft(buffersize);
  fourier.executefft(testSignal);
  fftw_complex* testSpectrum = fourier.out;
  vocoder vocodeObj = vocoder(44100, buffersize, (void*) C_Major);

  vocodeObj.pitchShift_setup(testSpectrum);
  vocodeObj.pitchShift();
  fftw_complex* outSpectrum = vocodeObj.FourierTransform;

  //outputs

  for (int i = 0; i < buffersize; i++){
    timesignal << (testSignal[i]) << '\n';
    spectrumIn << (*testSpectrum[i]) << '\n';
    spectrumOut << (*outSpectrum[i]) << '\n';
  };

  //plot this
  //vocodeObj.FourierTransform;

  return 0;
}
