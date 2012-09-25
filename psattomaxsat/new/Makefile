CPP = /usr/bin/g++
CC  = /usr/bin/gcc
CFLAGS_Release = -O3 -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS
CPP_Release = $(CPP) $(CFLAGS_Release)
CC_Release = $(CC) $(CFLAGS_Release)

all: PSATtoMaxSat

PSATtoMaxSat: main.cpp Input_Reading.cpp PSAT.cpp Matrix.cpp Inequality.cpp ClauseGen.cpp ClauseGen.h  Inequality.h  Input_Reading.h  Matrix.h  PSAT.h LA.c maxsatz2012.c Solver.cc
	$(CC_Release) maxsatz2012.c -c
	$(CC_Release) LA.c -c
	$(CPP_Release) main.cpp Input_Reading.cpp PSAT.cpp Matrix.cpp Inequality.cpp ClauseGen.cpp Solver.cc -c
	$(CPP_Release) main.o Input_Reading.o PSAT.o Matrix.o Inequality.o ClauseGen.o LA.o maxsatz2012.o Solver.o -o PSATtoMaxSat -llapack -lblas

clean:
	rm -f PSATtoMaxSat maxsat LA2 main.o Input_Reading.o PSAT.o Matrix.o Inequality.o ClauseGen.o LA.o maxsatz2012.o Solver.o geratestesMaxSat
