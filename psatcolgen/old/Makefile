CPP = /usr/bin/g++
CC  = /usr/bin/gcc
CFLAGS_Release = -Wall -O3 -Wno-unused-result
CPP_Release = $(CPP) $(CFLAGS_Release)
CC_Release = $(CC) $(CFLAGS_Release)

all: PsatColGen

LA: LA.c
	$(CC_Release) LA.c -o LA -llapack -lblas

PsatColGen: main.cpp Input_Reading.cpp PSAT.cpp Matrix.cpp Inequality.cpp ClauseGen.cpp verbosity.h LA
	$(CPP_Release) main.cpp Input_Reading.cpp PSAT.cpp Matrix.cpp Inequality.cpp ClauseGen.cpp -o PsatColGen

clean:
	rm -f LA PsatColGen
