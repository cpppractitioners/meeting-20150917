.PHONY: all clean

all: test

clean:
	rm -f test.o test

test.o: test.cpp
	g++ -std=c++11 -I../include -c test.cpp -o test.o 

test: test.o
	g++ -std=c++11 test.o -o test -lpthread
