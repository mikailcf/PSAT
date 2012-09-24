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


/*This class treats a PSAT instance modified to have only integer parameters.
So, woking with k bits, all the assigned probabilities were rescaled from the real interval [0,1] to the integer interval [0,maxProb] and our solution will also be in the same range. To recover the original solution from the integer one,
we just need to divide the latter by 2^k-1. */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Integer_PSAT.h"
#include "ClauseGen.h"
#include "Solver.h"

//this function converts a long 'p' to its binary representation with k bits, 
//returning the vector bin
long *longToBin(long p, int k){
    int x = p;
    long *bin = new long[k ];
    for (int i = 0; i < k ; i++){
        bin[i] = x % 2;
        x = x / 2;
    }
    return bin;
}


//atribuites: number of variables, number of assigned probabilities, number of clauses,
//probabilities assigned (converted to integers), precision in bits, and a pointer to the file that contains Gamma.
Integer_PSAT::Integer_PSAT(int nV, int nPr, int nCl, long* piN, int kb, FILE* gam){
    int i;	
    nVar = nV;
    nProb = nPr;
    nClause = nCl;
    kbits=kb; //numero de bits do vetor pi
    gamma=gam;
    //dbits = detBits(nProb+1);  //numero de bits para o determinante
    //tbits = dbits + kbits;  //total de bits na solução y
	
    if(nProb>0){
        piNum = new long[nProb+1];
        piBin = new long*[nProb+1];	
        for(i=0;i<nProb+1;i++){
            piNum[i] = piN[i];
            piBin[i] = new long[kbits];
            piBin[i] = longToBin(piNum[i],kbits);
        }
    }
}

int SATSolve(Solver* solver, int gamma){
    if (!solver->simplify()){
        printf("UNSATISFIABLE\n");
        return 1;
    }
    
    vec<Lit> dummy;
    lbool ret = solver->solveLimited(dummy);
    printf(ret == l_True ? "SATISFIABLE\n" : ret == l_False ? "UNSATISFIABLE\n" : "INDETERMINATE\n");
    
    if(ret != l_True) return 1;
    
    if(gamma){        
        if(ret == l_True){
            for(int i = 0; i < solver->nVars(); i++)
                if(solver->model[i] != l_Undef)
                    printf("%s%s%d", (i==0)?"":" ", (solver->model[i]==l_True)?"":"-", i+1);
            printf(" 0\n");
        }
    }
    
    return 0;
}

//This function uses the solution from the sat solver, and recover the matrix A e probability distribution y;
void PSATSolve(int nProb, int nVar, int prec, long * p, Solver* solver){
    FILE *output;
    int sat, lit, *matrix, i, j, k;
    long den;
    double *y;

    matrix = new int[(nProb+1)*(nProb+1)];
    y = new double[nProb+1];

    den = pow(2,prec)-1;
    
    if(SATSolve(solver, 0)) return;  // in case the instance is not satisfiable
        
    k = 0;

    for(j=0;j<=nProb;j++){
        matrix[(nProb+1)*nProb+j]=1;
        for(i=0;i<nProb;i++){
            lit = (solver->model[k]==l_True) ? k+1 : -k-1;
            k++;
            if(lit>0)matrix[(nProb+1)*i+j]=1;
            else matrix[(nProb+1)*i+j]=0;
        }
        while(abs(lit)!=(nVar+1)*(j+1)){
            lit = (solver->model[k]==l_True) ? k+1 : -k-1;
            k++;
        }
    }
    for(j=0;j<=nProb;j++){
        y[j]=0;
        for(i=0;i<prec;i++){
            lit = (solver->model[k]==l_True) ? k+1 : -k-1;
            k++;
            if(lit>0)y[j]+=pow(2,i);
        }
        y[j]=y[j]/den;
    }

    printf("Matrix founded (bottom row forces the probabilities to sum up to 1):\n");	
    for(i=0;i<=nProb;i++){
        for(j=0;j<=nProb;j++)printf("%8d ",matrix[(nProb+1)*i+j]);
        printf("\n");
    }
    printf("Probability Distribuition:\n");
    for(i=0;i<=nProb;i++)printf("%.6lf ",y[i]);
    printf("\n");
    printf("Assigned probabilities, with precision limited to %d bits:\n",prec);
    for(i=0;i<=nProb;i++)printf("%.6f ",(float) p[i]/den);
}



void Integer_PSAT::solve(){
    // the idea is to find a matrix A and a probability distribution y such that A.y=pi,
    // where the column of A represents valuations, over the atoms in Psi, that are consistent with Gamma,
    // and pi is the vector with the probability assignment, converted to integers.

    int i,j,k;
    long **A,**y,***ay, ***by; //ay_j is a_ij.y_j; by_ij is the sum from ay_i1 to ay_ij
    ClauseGen* satIns;
    Solver* solver;
    
    Solver S;
    solver = &S;
    satIns = new ClauseGen(solver);

    // satIns->printGamma(nVar,nProb,nClause);//prints a Gamma instance for each column of A
    satIns->addGamma(nVar,nProb,nClause);//prints a Gamma instance for each column of A
    if(nProb == 0){ //Psi is empty; it's enough to verify the satisfiability of Gamma
        SATSolve(solver, 1);
        return ;
    }

    //the variables are initialized:
    A = satIns->initCoefMatrix(nProb+1, nVar);// matrix representing the valuations over the variables in Psi
    y = satIns->initVarDim2(nProb + 1, kbits);//the probability distribution we are looking for, scaled to integers, with 'kbits' (the precision) variables for each probability
    ay = satIns->initVarDim3(nProb+1, nProb+1, kbits);//matrix to store each element of A multiplied by the corresponding element of y, with 'kbits' (the precision) variables for each element
    by = satIns->initVarDim3(nProb+1, nProb, kbits);// acumulated sums of each row of the matrix ay.
    pi = satIns->initVarDim2(nProb+1,kbits); //assigned probabilities scaled to integers, with "kbits" variables for each probability 

    //clauses are printed:
    for(i=0;i<nProb+1;i++)
        for(k=0;k<kbits;k++)
            satIns->andN(1, (piBin[i][k]*2-1)*pi[i][k]);//pi is that from the input 
        
    for(i=0;i<nProb+1;i++)
        satIns->andN(1, A[nProb][i]);//bottom row with 1's
    
    for(i=0;i<nProb+1;i++)
        for(j=0;j<nProb+1;j++)
            for(k=0;k<kbits;k++)
                satIns->eq_and2(ay[i][j][k], A[i][j], y[j][k]);
    
    for(i=0;i<nProb+1;i++)
        satIns->eq_sum(by[i][0], ay[i][0], ay[i][1],kbits); 
    
    for(i=0;i<nProb+1;i++)
        for(j=1;j<nProb;j++)
            satIns->eq_sum(by[i][j], by[i][j-1], ay[i][j+1], kbits);
    
    for(i=0;i<nProb+1;i++)
        satIns->eq_sum(pi[i], by[i][nProb-2], ay[i][nProb], kbits);

    PSATSolve(nProb,nVar,kbits, piNum, solver);
}
