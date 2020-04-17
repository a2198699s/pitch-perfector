# Qt project file - qmake uses his to generate a Makefile

QT       += core gui

CONFIG          += qt warn_on debug

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PitchPerfector

greaterThan(QT_MAJOR_VERSION, 4): LIBS += -lqwt-qt5 -lm -lrtaudio -lfftw3
lessThan(QT_MAJOR_VERSION, 5): LIBS += -lqwt -lm -lrtaudio -lfftw3

INCLUDEPATH += /usr/include/rtaudio

HEADERS += window.h audioStreamer.h fft.h vocoder.h helper.h dispatch.h voxFilter.h
SOURCES += main.cpp window.cpp audioStreamer.cpp fft.cpp vocoder.cpp helper.cpp dispatch.cpp voxFilter.cpp
