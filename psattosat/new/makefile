PSATtoSAT: main.o Input_Reading.o PSAT.o Integer_PSAT.o ClauseGen.o Solver.o
	g++ main.o Input_Reading.o PSAT.o Integer_PSAT.o ClauseGen.o Solver.o -o PSATtoSAT
main.o: Input_Reading.h main.cpp
	g++ main.cpp -c
Input_Reading.o: Input_Reading.h Input_Reading.cpp
	g++ Input_Reading.cpp -c
PSAT.o: Integer_PSAT.h PSAT.h PSAT.cpp
	g++ PSAT.cpp -c
Integer_PSAT.o: ClauseGen.h Solver.h Integer_PSAT.h Integer_PSAT.cpp
	g++ Integer_PSAT.cpp -c
ClauseGen.o: Solver.h SolverTypes.h ClauseGen.h ClauseGen.cpp
	g++ ClauseGen.cpp -c
Solver.o: Sort.h  Solver.h Solver.cc
	g++ Solver.cc -c

clean:
	rm -f main.o Input_Reading.o PSAT.o Integer_PSAT.o ClauseGen.o Solver.o
clean-all:
	rm -f main.o Input_Reading.o PSAT.o Integer_PSAT.o ClauseGen.o Solver.o PSATtoSAT