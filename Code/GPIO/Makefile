all: readin

gpio-functions.o: gpio-functions.cpp readin.h
	gcc -c gpio-functions.cpp

readin: readin.cpp gpio-functions.o
	gcc -o readin gpio-functions.o readin.cpp

clean:
	rm -f readin gpio-functions.o
