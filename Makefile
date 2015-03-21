CC = g++
all :   ready
ready : main.o functions.o
	$(CC) -o ready main.o functions.o
main.o :main.cpp
	$(CC) -c main.cpp
functions.o :functions.cpp
	$(CC) -c functions.cpp

clean :
	rm -f ready main.o functions.o

