/*
    Copyright (C) 2011 Glauber De Bona

    This file is part of PSATtoSAT.

    PSATtoSAT is free software: you can redistribute it and/or modify
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


/*This class describes a PSAT instance. In order to reduce this instance to a SAT instance, the parameters are brought to the integer domain, using a limited precision in bits. So, this Integer_PSAT object contains an instance that is reduced   to SAT, to finally decide the satisfiability.*/



#include "PSAT.h"
#include "Integer_PSAT.h"

/*atributes:
nVar is the total number of variables in Gamma, including that in Psi.
nProb is the number of variables in Psi, with an assigned probability.
nClause is the number of clauses in Gamma.
probabilities is an array containing the probabilities assigned in Psi.
precision is the number of bits used, for each probability, in the reduction to SAT.
gamma is a pointer to a file containing the clauses of Gamma. 

*/
PSAT::PSAT(int nV, int nPr, int nCl, double * prob, int prec, FILE * gam){
    int i;	
    nVar = nV;
    nProb = nPr;
    nClause = nCl;
    if(nProb>0){
        probabilities = new double[nProb];
        for(i=0;i<nProb;i++)probabilities[i] = prob[i];
    }
    else probabilities = NULL;	
    precision = prec;
    gamma = gam;
}

PSAT::~PSAT(){
    delete probabilities;
}

//This function first creat an Integer_PSAT object, where the satisfiability is decided.
void PSAT::solve(){
    int i, kbits;
    long maxProb,*pi_Int; 
    Integer_PSAT *intPSAT;
    if (nProb == 0){ 
        intPSAT = new Integer_PSAT(nVar, nProb, nClause, pi_Int, kbits, gamma);
        intPSAT->solve();
        return ;
    } 	
    maxProb=pow(2,precision)-1; //maxProb is the probability 1 converted to an integer
    //all the assigned probabilities are rescaled from the real interval [0,1] to the integer interval [0,maxProb]
    kbits=precision;//number of bits used while converting the assigned probabilities to integers
    pi_Int= new long[nProb+1]; //pi_Int will store these assigned probabilities converted to integers 
    for(i=0;i<nProb;i++)pi_Int[i]=(long)round(maxProb*probabilities[i]);
    pi_Int[nProb] = maxProb; //we add a probability 1 to force the probabilities to sum up to one
    intPSAT = new Integer_PSAT(nVar, nProb, nClause, pi_Int, kbits, gamma);
    intPSAT->solve();
}
