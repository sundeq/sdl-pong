CC=gcc
CFLAGS=-I

pongmake: pong.c
	$(CC) -o pong.o pong.c -lSDL2 -lm
