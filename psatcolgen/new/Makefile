CPP = /usr/bin/g++
CC  = /usr/bin/gcc
CFLAGS_Release = -O3 -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS
CPP_Release = $(CPP) $(CFLAGS_Release)
CC_Release = $(CC) $(CFLAGS_Release)

all: PsatColGen

PsatColGen: main.cpp Input_Reading.cpp PSAT.cpp Matrix.cpp Inequality.cpp ClauseGen.cpp verbosity.h LA.c Solver.cc
	$(CC_Release) LA.c -c
	$(CPP_Release) main.cpp Input_Reading.cpp PSAT.cpp Matrix.cpp Inequality.cpp ClauseGen.cpp Solver.cc -c
	$(CPP_Release) main.o Input_Reading.o PSAT.o Matrix.o Inequality.o ClauseGen.o LA.o Solver.o -o PsatColGen -llapack -lblas

clean:
	rm -f main.o Input_Reading.o PSAT.o Matrix.o Inequality.o ClauseGen.o LA.o Solver.o PsatColGen
