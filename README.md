QwtExample
===========

A simple example program using Qt/Qwt widgets to be used as a base for students doing Raspberry Pi data acquisition.

Qt is a cross platform framework for developing graphical applications, for more information please visit the links below:
* [Qt Homepage](http://qt-project.org/)
* [Qt 4.8 Class List](http://qt-project.org/doc/qt-4.8/classes.html)
* [Wikpedia](http://en.wikipedia.org/wiki/Qt_(framework))
* [Signals and Slots](http://qt-project.org/doc/qt-4.8/signalsandslots.html)

Qwt is a technical widget library based on Qt, please see:
* [Qwt Hompage](http://qwt.sourceforge.net/)


Making it work
--------------

To clone the git repository:
    git clone https://github.com/glasgow-bio/qwt-example

To build:
    cd qwt-example
    qmake
    make

To run (assuming you are logged into the RPi over ssh and no X-server is running):
    startx ./QwtExample
