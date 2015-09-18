.PHONY: all clean

all: main 

clean:
	rm -f main.o main

main.o: main.cpp
	g++ -std=c++11 -I../include -c main.cpp -o main.o 

main: main.o
	g++ -std=c++11 main.o -o main -lpthread
