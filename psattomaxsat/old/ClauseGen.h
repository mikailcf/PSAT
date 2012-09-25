#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>
#include"Inequality.h"
class ClauseGen{
	private: 
	long topAtom;
	long countClause;
	long varGamma;
	FILE* cnf;

	public:
	long gettopAtom(){return topAtom;}
	long getcountClause(){return countClause;}
	FILE* getcnf(){return cnf;}

	ClauseGen();
	~ClauseGen();
	
	void restart();
	void printGamma();
	void printGamma(long top);
	void xor2( long a, long b);
	void eq_xor2( long r, long a, long b);
	void orN( int n, ...);
	void orNlist( int n, long *list);
	void or2( long a,long  b);
	void eq_orN( int n, long r, ...);
	void eq_or2( long r, long a, long b);
	void andN( int n, ...);
	void and2( long a, long b);
	void eq_andN( int n, long r, ...);
	void eq_and2( long r, long a, long b);
	void eq_equiv2(long r, long a, long b);
	void eq_sum( long* sum, long * p1, long *p2, int bits);
	//void eq_mul( long* product, long * p1, long *p2, int bits1, int bits2);
	void geq(long* p1, long* p2, int bits);//p1 maior ou igual a p2 em binario
	long initVarDim0();
	long* initVarDim1(int size);
	long** initVarDim2(int nRow, int nCol);
	long*** initVarDim3(int n1, int n2, int n3);
	long** initCoefMatrix(int nRow, int nCol);
	void finalizeCNF();
	void finalizeWCNF(long top);
	void ineqtoCNF(Inequality *ineq);
	void ineqtoWCNF(Inequality *ineq, int *signal);
};
