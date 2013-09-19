# http://stackoverflow.com/questions/2145590/what-is-the-purpose-of-phony-in-a-makefile

DEBUG   = -O3                                                                   
CC      = gcc                                                                   
INCLUDE = -I/usr/local/include                                                  
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe                              
                                                                                
LDFLAGS = -L/usr/local/lib                                                      
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm 


.phony: clean

all: shuttercontrol

clean:
	rm shuttercontrol *.o

shuttercontrol: shuttercontrol.o
	@$(CC) -o $@ shuttercontrol.o $(LDFLAGS) $(LDLIBS) 

shuttercontrol.o: shuttercontrol.c
	@$(CC) -c shuttercontrol.c -o $@

