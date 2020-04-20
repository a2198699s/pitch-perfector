#include "window.h"
#include "audioStreamer.h"
#include <iostream>  
#include <string>  
#include <cmath>  // for sine stuff
#include "helper.h"

Window::Window() : gain(5), count(0)
{
	// knob = new QwtKnob;
	// set up the gain knob
	// knob->setValue(gain);

	// use the Qt signals/slots framework to update the gain -
	// every time the knob is moved, the setGain function will be called
	// connect( knob, SIGNAL(valueChanged(double)), SLOT(setGain(double)) );

	
	//thermo->setRange(0, 20);
	int Fs = 44100;

	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index] = index;
		yData[index] = gain * sin( M_PI * index/50 );
	}
	for( int index=0; index<fftPlotDataSize; ++index )
	{
		xData2[index] = index; 
		yData2[index] = 0;
	}

	currentNoteText = new QwtTextLabel();
	currentNoteText->setText("HELLO");


	needle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow);
	roundScale = new QwtRoundScaleDraw();
	dial = new QwtDial;
	dial->setScaleArc(270, 90);
	dial->setScale(-1, 1);
	dial->setMode(QwtDial::RotateNeedle);
	// dial->setScaleDraw(roundScale);
	// needle->draw()
	dial->setNeedle(needle);
	dial->setReadOnly(true);
	dial->show();




	curve = new QwtPlotCurve;
	plot = new QwtPlot;
	curve2 = new QwtPlotCurve;
	plot2 = new QwtPlot;
	// make a plot curve from the data and attach it to the plot
	curve->setSamples(xData, yData, plotDataSize);
	curve->attach(plot);

	QwtText title("Audio Input");
	plot->setTitle(title);
	plot->replot();
	plot->show();

	curve2->setSamples(xData2, yData2, plotDataSize);
	curve2->attach(plot2);

	QwtText title2("Frequency Spectrum");
	plot2->setTitle(title2);
	plot2->replot();
	plot2->show();

	dialLayout = new QVBoxLayout;
	dialLayout->setSizeConstraint(QLayout::SetMinimumSize);
	dialLayout->addWidget(currentNoteText);
	dialLayout->addWidget(dial);	

	// plot to the left of knob and thermometer
	hLayout = new QHBoxLayout;
	hLayout->addWidget(plot);

	hLayout->addWidget(plot2);
	hLayout->addLayout(dialLayout);



	setLayout(hLayout);

	// starts the audio streamer thread
	aStreamer = new audioStreamer();
	aStreamer->start();
}

Window::~Window() {
	// tells the thread to no longer run its endless loop
	aStreamer->quit();
	// wait until the run method has terminated
	aStreamer->wait();
	delete aStreamer;
}

void Window::timerEvent( QTimerEvent * )
{
	int lowerVal = round(getFrequencyBelow(*(aStreamer->currentNoteFrequency)));
	int  higherVal  = round(getFrequencyAbove(*(aStreamer->currentNoteFrequency)));
	dial->setScale(lowerVal, higherVal);
	dial->setValue(*(aStreamer->currentNoteFrequency));
	currentNoteText->setText(aStreamer->currentNote);

	// currentNoteText->setText(std::to_string(count).c_str());
	// add the new input to the plot
	memmove( yData, yData+1, (plotDataSize-1) * sizeof(double) );
	yData[plotDataSize-1] = aStreamer->inputData[plotDataSize-1];
	curve->setSamples(xData, yData, plotDataSize);
	plot->replot();

	// Fill ydata2 up with the real part of fftw_complex.
	for ( int i=0; i<fftPlotDataSize; ++i ) {
		yData2[i] = aStreamer->outputData[i][0] * aStreamer->outputData[i][0];
	}
	curve2->setSamples(xData2, yData2, fftPlotDataSize);
	plot2->replot();
}


// this function can be used to change the gain of the A/D internal amplifier
void Window::setGain(double gain)
{
	// for example purposes just change the amplitude of the generated input
	this->gain = gain;
}
