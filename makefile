CC=g++
CFLAGS=-c -Wall

all: customwm

customwm: customwm.o
	$(CC) customwm.o -lX11 -o customwm

customwm.o: customwm.c
	$(CC) $(CFLAGS) customwm.c

clean:
	rm *o customwm

