/*
    Copyright (C) 2011 Glauber De Bona

    This file is part of PsatColGen.

    PsatColGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/


#include"Matrix.h"
#include"ClauseGen.h"

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
	int setInequalities(int prec, Matrix *inv, double *basicSol, int column,int *notinGamma, Inequality **ineq);
	int setSimplexInequality(Matrix *inv, double * basicSolution, int *notinGamma, Inequality **ineq);
};
