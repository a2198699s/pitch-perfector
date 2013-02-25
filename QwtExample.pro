# Qt project file - qmake uses his to generate a Makefile

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QwtExample

LIBS += -lqwt -lm

HEADERS += window.h

SOURCES += main.cpp window.cpp
