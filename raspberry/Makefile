# http://stackoverflow.com/questions/2145590/what-is-the-purpose-of-phony-in-a-makefile
.phony: clean

all: shuttercontrol

clean:
	rm *.o

build: shuttercontrol.o
	gcc -o shuttercontrol \
	    shuttercontrol.o

shuttercontrol.o: shuttercontrol.c
	gcc $(CFLAGS) -c shuttercontrol.c -o shuttercontrol.o

