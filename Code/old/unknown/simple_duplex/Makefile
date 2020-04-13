# To make a specific file call make with the filename.
# Eg. make mit_duplex

duplex: duplex_rtaudio.cpp
	g++ -c duplex_rtaudio.cpp -I/usr/include/rtaudio
	g++ duplex_rtaudio.o -o duplex -lrtaudio
	
mit_duplex: mit_duplex.cpp
	g++ -c mit_duplex.cpp -I/usr/include/rtaudio
	g++ mit_duplex.o -o mit_duplex -lrtaudio