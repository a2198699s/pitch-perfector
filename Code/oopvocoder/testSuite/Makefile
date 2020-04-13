tester: test_vocoder.cpp
	g++ -c test_vocoder.cpp -I/usr/local/include/rtaudio
	g++ -c fft.cpp -I/usr/local/include/rtaudio
	g++ -c vocoder.cpp -I/usr/local/include/rtaudio
	g++ -o tester test_vocoder.o fft.o vocoder.o -lfftw3 -lrtaudio
