#include "adcreader.h"
#include <QDebug>

void ADCreader::run()
{
	running = true;
	while (running) {
		qDebug() << "Tick";
		sleep(1);
	}
}

void ADCreader::quit()
{
	running = false;
	exit(0);
}
