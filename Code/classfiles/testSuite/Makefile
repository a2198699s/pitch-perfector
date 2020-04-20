test: test_vocoder.cpp
	g++ -c test_vocoder.cpp fft.cpp vocoder.cpp -I/usr/local/include/rtaudio
	g++ -o test test_vocoder.o fft.o vocoder.o -lfftw3 -lrtaudio
