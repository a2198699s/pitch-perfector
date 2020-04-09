#include "audioStreamer.h"
#include <QDebug>
#include <RtAudio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <fftw3.h>
#include "fft.h"
#include "vocoder.h"
using namespace std;

void audioStreamer::run()
{
	const float cMajor[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

	RtAudio adac;
	if ( adac.getDeviceCount() < 1 ) {
		std::cout << "\nNo audio devices found!\n";
		exit( 0 );
	}

	unsigned int bufferFrames = 512; // samples/Fs = bufferTime 
	RtAudio::StreamParameters iParams, oParams;
	iParams.deviceId = 3; // first available device
	iParams.nChannels = 1;
	oParams.deviceId = 0; // first available device
	oParams.nChannels = 1;

	// Instantiate FFT Class
	int signed_bufferFrames = (int) bufferFrames;
	int samplingRate = 44100;

	fft fourier(signed_bufferFrames);
	// Vocoder cVocoder(samplingRate, signed_bufferFrames, cMajor);
  	Dispatch dispatcher = Dispatch(&fourier);
	running = true;
	try {
		adac.openStream( &oParams, &iParams, RTAUDIO_FLOAT64, 44100, &bufferFrames, &Dispatch::caller, (void *)&dispatcher );
		cout << "opened\n";
	}
	catch ( RtAudioError& e ) {
		cout << "couldn't open\n";
		e.printMessage();
		exit( 0 );
	}
	try {
		adac.startStream();
		char input;
		std::cout << "\nRunning ... press <enter> to quit.\n";
		inputData = fourier.in;
		outputData = fourier.out;
		inverseOut = fourier.inverse_out;
		std::cin.get(input);
		// Stop the stream.
		if (!running) adac.stopStream();
	}
	catch ( RtAudioError& e ) {
		cout << "couldn't start stream \n";
		e.printMessage();
		goto cleanup;
	}
	cleanup:
		cout << "cleaning up \n";
		running = false;
		if ( adac.isStreamOpen() ) adac.closeStream();
}

void audioStreamer::quit()
{
	running = false;
	exit(0);
}





