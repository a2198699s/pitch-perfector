#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <fftw3.h>
#include "vocoder.h"
#include "fft.h"

using namespace std;


//test Key
const float C_Major[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

//generate sin waves and create sample dataset
double* sinGen(){
  double sinwave[512] = {0};
  for (int i=0; i<512; i++) {
    sinwave[i] = (double)(sin(i * 1) + 40*sin(i * 50) + 60*sin(i*450) + 10*sin(i * 530));
  }
  return sinwave;
}

//find spectrums of
fftw_complex spectrumGen(double* signalIn){
  fftw_complex fftSpect;
  fft fourier = fft(512);
  fourier.excecutefft(signalIn);
  return fourier.out;
};

int main(){
  //set up
  float testSignal = sinGen();
  fftw_complex testSpectrum = spectrumGen(testSignal);
  vocoder vocodeObj = vocoder(44100, 512, (void*) C_Major);

  vocodeObj.pitchShift_setup((fftw_complex*) testSpectrum);
  vocodeObj.pitchShift();

  //plot this
  //vocodeObj.FourierTransform;

  return 0;
}
