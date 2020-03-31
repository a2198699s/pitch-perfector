vocode: vocoding.cpp
	g++ -c vocoding.cpp -I/usr/local/include/rtaudio
	g++ -c vocoder.cpp
	g++ -o vocode vocoding.o vocoder.o -lrtaudio -lfftw3
