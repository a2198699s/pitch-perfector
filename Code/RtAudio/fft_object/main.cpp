#include <window.h>
#include <QApplication>


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	// create the window
	Window window;
	window.showMaximized();

	// call the window.timerEvent function every 10 ms
	window.startTimer(10);

	// execute the application
	return app.exec();
}