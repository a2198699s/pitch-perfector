#include "window.h"
#include "audioStreamer.h"

#include <cmath>  // for sine stuff


Window::Window() : gain(5), count(0)
{
	knob = new QwtKnob;
	// set up the gain knob
	knob->setValue(gain);

	// use the Qt signals/slots framework to update the gain -
	// every time the knob is moved, the setGain function will be called
	connect( knob, SIGNAL(valueChanged(double)), SLOT(setGain(double)) );

	// set up the thermometer
	thermo = new QwtThermo; 
	thermo->setFillBrush( QBrush(Qt::red) );
	//thermo->setRange(0, 20);
	thermo->show();


	// set up the initial plot data
	for( int index=0; index<plotDataSize; ++index )
	{
		xData[index] = index;
		yData[index] = gain * sin( M_PI * index/50 );
		xData2[index] = index; 
		yData2[index] = 0;
	}

	curve = new QwtPlotCurve;
	plot = new QwtPlot;
	curve2 = new QwtPlotCurve;
	plot2 = new QwtPlot;
	// make a plot curve from the data and attach it to the plot
	curve->setSamples(xData, yData, plotDataSize);
	curve->attach(plot);

	plot->replot();
	plot->show();

	curve2->setSamples(xData2, yData2, plotDataSize);
	curve2->attach(plot2);

	plot2->replot();
	plot2->show();

	// set up the layout - knob above thermometer
	vLayout = new QVBoxLayout;
	vLayout->addWidget(knob);
	vLayout->addWidget(thermo);

	// plot to the left of knob and thermometer
	hLayout = new QHBoxLayout;
	hLayout->addLayout(vLayout);
	hLayout->addWidget(plot);

	hLayout->addWidget(plot2);


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
	++count;

	// add the new input to the plot
	memmove( yData, yData+1, (plotDataSize-1) * sizeof(double) );
	yData[plotDataSize-1] = aStreamer->inputData[plotDataSize-1];
	curve->setSamples(xData, yData, plotDataSize);
	plot->replot();

	// Fill ydata2 up with the real part of fftw_complex.
	for ( int i=0; i<plotDataSize; ++i ) {
		yData2[i] = aStreamer->outputData[i][0];
	}
	curve2->setSamples(xData2, yData2, plotDataSize);
	plot2->replot();

	// set the thermometer value
	// thermo->setValue( inVal + 10 );
}


// this function can be used to change the gain of the A/D internal amplifier
void Window::setGain(double gain)
{
	// for example purposes just change the amplitude of the generated input
	this->gain = gain;
}
