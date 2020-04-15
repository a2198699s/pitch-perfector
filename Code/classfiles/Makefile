tester: fulltest.cpp
	g++ -c fulltest.cpp fft.cpp vocoder.cpp dispatch.cpp -I/usr/include/rtaudio
	g++ -o tester fulltest.o vocoder.o fft.o dispatch.o -lrtaudio -lfftw3
