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
const int buffersize = 1024;

double* sinGen(){
  static double sinwave[buffersize] = {0};
  for (int i=0; i<buffersize; i++) {
    sinwave[i] = (double) (10*sin(i * 1)); //+ 40*sin(i * 50) + 60*sin(i*450) + 10*sin(i * 530));
  }
  return sinwave;
};

int main(){

// Set up:
  //generate signal
  double* testSignal = (double*) malloc(sizeof(double*)*(buffersize));
  testSignal = sinGen();
  //make fourier object and find spectrum
  fft fourier = fft(buffersize);
  fourier.executefft(testSignal);
  fftw_complex* testSpectrum = fourier.out;

  //make the vocoder object
  vocoder vocodeObj = vocoder(44100, buffersize, C_Major);


// Testing...

//test noteFinder & Binary search
cout << '\n' << "Note Finder Tests" << '\n';

  float testNote1 = 341;
  float newNote1 = vocodeObj.noteFinder(C_Major, &testNote1);
  cout << "mid note 1: " << newNote1 << '\n';

  float testNote2 = 300;
  float newNote2 = vocodeObj.noteFinder(C_Major, &testNote2);
  cout << "mid note 2: " << newNote2 << '\n';

  float testNote3 = 200;
  float newNote3 = vocodeObj.noteFinder(C_Major, &testNote3);
  cout << "low note (corner): " << newNote3 << '\n';

  float testNote4 = 550;
  float newNote4 = vocodeObj.noteFinder(C_Major, &testNote4);
  cout << "high note (corner): " << newNote4 << '\n';

  float zeroNote = 0;
  float OutputOfzeroNote = vocodeObj.noteFinder(C_Major, &zeroNote);
  cout << "zero note: " << OutputOfzeroNote << '\n';

//test sample to frequency
cout << '\n' << "Sample to Freq Test" << '\n';
  int testSample = 1024;
  float sampToFreq = vocodeObj.SampleToFreq(testSample);
  cout << "Freq: " << sampToFreq << '\n';
  cout << "Freq Resolution: " << vocodeObj.FreqRes << '\n';

//test pitchShift_setup - problems dealing with fft_complex type here...
cout << '\n' << "Test Pitch Shift Setup" << '\n';
  vocodeObj.pitchShift_setup(testSpectrum);

  cout << "base sample: " << vocodeObj.baseSample << '\n';


  // vocodeObj.pitchShift_setup(testSpectrum);
  // vocodeObj.pitchShift();
  // fftw_complex* outSpectrum = vocodeObj.FourierTransform;




  //outputs
  for (int i = 0; i < buffersize; i++){
    timesignal << (testSignal[i]) << '\n';
    spectrumIn << (*testSpectrum[i]) << '\n';
    //spectrumOut << (*outSpectrum[i]) << '\n';
  };
  //cout << **outSpectrum << '\n';

  return 0;
}
