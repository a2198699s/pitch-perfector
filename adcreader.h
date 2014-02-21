#ifndef ADCREADER
#define ADCREADER

#include <QThread>

class ADCreader : public QThread
{
public:
	ADCreader() {running = 0;};
	void quit();
	void run();
private:
	bool running;

};

#endif
