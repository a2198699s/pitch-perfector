
#ifndef AUDIOSTREAMER_H
#define AUDIOSTREAMER_H

#include <QThread>

class audioStreamer : public QThread
{
public:
	audioStreamer() {running = 0; dataPoint=1;};
	void quit();
	void run();
    int dataPoint;

private:
	bool running;

};

#endif