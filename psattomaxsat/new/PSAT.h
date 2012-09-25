#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>
#include"Matrix.h"
#include"ClauseGen.h"
//||

class PSAT{
	private:
	int nVar;
	int nProb;
	int nClause;
	double* probabilities;
	//double ** basis; 
	int precision;
	FILE* gamma;
	
	public:	
	int getnVar(){return nVar;}
	int getnProb(){return nProb;}
	int getnClause(){return nClause;}
	double getprobabilities(int i){return probabilities[i];}
	int getprecision(){return precision;}
	FILE* getgamma(){return gamma;}
	PSAT(int nVar, int nProb, int nClause, double* probabilities, int precision, FILE* gamma); 
	~PSAT();
	void solve();
	int setInequalities(int prec, Matrix *inv, double *basicSol, int column, Inequality **ineq);
	int setSimplexInequality(FILE *saida, Matrix *inv, double *basicSolution, int *notinGamma, Inequality **ineq);
};
