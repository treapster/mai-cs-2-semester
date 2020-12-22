CC=gcc
LD=gcc
CCFLAGS=-g -Wall -pedantic -std=c99
lab25: main.o udt.o
	$(CC) -o lab25 main.o udt.o
main.o: main.c
	$(CC) -c main.c
udt.o: udt.c 
	$(CC) -c udt.c
clean:
	rm main.o udt.o lab25

