# http://stackoverflow.com/questions/2145590/what-is-the-purpose-of-phony-in-a-makefile

DEBUG   = -O3                                                                   
CC      = gcc                                                                   
INCLUDE = -I/usr/local/include                                                  
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe                              
                                                                                
LDFLAGS = -L/usr/local/lib                                                      
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lmosquitto


.phony: clean

all: doorstate

clean:
	rm doorstate *.o

doorstate: doorstate.o
	@$(CC) -o $@ doorstate.o $(LDFLAGS) $(LDLIBS) 

doorstate.o: doorstate.c
	@$(CC) -c doorstate.c -o $@

