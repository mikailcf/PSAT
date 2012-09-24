CPP = /usr/bin/g++
CC  = /usr/bin/gcc
CFLAGS_Release = -Wall -O3
CPP_Release = $(CPP) $(CFLAGS_Release)
CC_Release = $(CC) $(CFLAGS_Release)

all: PSATtoSAT

PSATtoSAT: main.cpp Input_Reading.cpp PSAT.cpp Integer_PSAT.cpp ClauseGen.cpp
	$(CPP_Release) main.cpp Input_Reading.cpp PSAT.cpp Integer_PSAT.cpp ClauseGen.cpp -o PSATtoSAT

clean:
	rm -f PSATtoSAT
