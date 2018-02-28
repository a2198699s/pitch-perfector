# QwtExample

A simple example program using Qt/Qwt widgets to be used as a base for students doing Raspberry Pi data acquisition. It also creates a Thread which could be used to acquire ADC data with the help of poll/select and storing it in a ringbuffer.

![alt tag](screenshot.png)

## Making it work

Install the QT5 and Qwt development packages:

```
    apt-get install qtdeclarative5-dev-tools
    apt-get install libqwt-qt5-dev
```

To clone the git repository:

```
    git clone https://github.com/glasgow-bio/qwt-example
```

To build:

```
    cd qwt-example
    qmake
    make
```

To run (assuming you are logged into the RPi over ssh and no X-server is running):

```
    startx ./QwtExample
```

or with X running

```
    ./QwtExample
```
