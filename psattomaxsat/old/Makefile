CPP = /usr/bin/g++
CC  = /usr/bin/gcc
CFLAGS_Release = -Wall -O3 -Wno-unused-result
CPP_Release = $(CPP) $(CFLAGS_Release)
CC_Release = $(CC) $(CFLAGS_Release)

all: PSATtoMaxSat geratestsMaxSat maxsat Makefile LA2

PSATtoMaxSat: main.cpp Input_Reading.cpp PSAT.cpp Matrix.cpp Inequality.cpp ClauseGen.cpp ClauseGen.h  Inequality.h  Input_Reading.h  Matrix.h  PSAT.h
	$(CPP_Release) main.cpp Input_Reading.cpp PSAT.cpp Matrix.cpp Inequality.cpp ClauseGen.cpp -o PSATtoMaxSat

geratestsMaxSat: geratestesMaxSat.cpp paramsMaxSat.h
	$(CPP_Release) $<  -o $@

LA2:
	$(CC_Release) LA.c -o ./LA2 -llapack -lblas

maxsat:
	$(CC_Release) maxsatz/maxsatz2009.c -o ./maxsat
clean:
	rm -f PSATtoMaxSat maxsat geratestsMaxSat LA2
