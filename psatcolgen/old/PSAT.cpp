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


/*This class represents the PSAT instance itself, and decides its satisfiability.*/

#include <iostream>
#include<stdlib.h>
#include<math.h>
#include "PSAT.h"
#include "verbosity.h"

using namespace std;

PSAT::PSAT(int nV, int nPr, int nCl, double * prob, int prec, FILE * gam){
  int i/*,j*/;	
	nVar = nV;
	nProb = nPr; 
	nClause = nCl;
	if(nProb>0){
		probabilities = new double[nProb+1];
		for(i=0;i<nProb;i++)probabilities[i+1] = prob[i];
		probabilities[0]=1; //artificial assigned probability to TRUE formula; 
	}
	else probabilities = NULL;	
	precision = prec;//maximum number of digits used to assign probabilities 
	gamma = gam;
	
}

PSAT::~PSAT(){
	delete probabilities;
}
//set inequalities to force one inconsistent column to be replaced in the basis
int PSAT::setInequalities(int prec, Matrix *inv, double *basicSol, 
						int column, int *notinGamma, Inequality **ineq){
	int det, i, j,k,count=0;
	det=round(1/inv->getDet());
	for(i=0;i<inv->getSize();i++)
		for(j=0;j<inv->getSize();j++)
			inv->set(i,j,round(inv->get(i,j)*det));	
	for(j=0;j<inv->getSize();j++)if(column!=j && notinGamma[j]==0){
		for(k=0;k<inv->getSize();k++)
			ineq[count]->set(k,inv->get(column,k)*round(det*basicSol[j]*pow(10,prec))-inv->get(j,k)*round(det*basicSol[column]*pow(10,prec)));
		if(basicSol[column]==0)ineq[count]->set(0,ineq[count]->get(0)-1);
		count++;
	}
	for(i=0;i<inv->getSize();i++)
		for(j=0;j<inv->getSize();j++)
			inv->set(i,j,inv->get(i,j)/det);	
	return count;//number of inequalities
}
//set a inequality correponding a simplex column generation, looking for a column that doesn't increase the cost 
int PSAT::setSimplexInequality(Matrix *inv,double *basicSolution, int *notinGamma, Inequality **ineq){
	int det, i, j,k,count=1;
	det=round(1/inv->getDet());
	for(i=0;i<inv->getSize();i++)
		for(j=0;j<inv->getSize();j++)
			inv->set(i,j,round(inv->get(i,j)*fabs(det)));	
	ineq[0]->set(0,-1);
	for(k=1;k<inv->getSize();k++)ineq[0]->set(k,0);

	for(j=0;j<inv->getSize();j++)if(notinGamma[j]==1)
		for(k=0;k<inv->getSize();k++)
			ineq[0]->set(k,ineq[0]->get(k)+inv->get(j,k));
	
	for(i=0;i<inv->getSize();i++)
		for(j=0;j<inv->getSize();j++)
			inv->set(i,j,inv->get(i,j)/fabs(det));		
	return count;
}

//recover a column from the Sat Solver output, with a negative first element when the SAT instance was Unsatisfiable
void getColumn(int *column, int size){
	FILE *output;
	int sat, i,lit;
	char *lido;
	lido = new char[6];
	output=fopen("saidazc.out","r"); //zchaff's output
	while (!feof(output))fscanf(output, "%6c", lido);
	sat=0;
	for(i=0;i<=5;i++)if(lido[i]==':')sat=1;
	rewind(output);
	if(sat==0){
		column[0]=-1;
		return ;
	}
	while ((lido[0]!='l' || lido[1]!='e')){
		lido[0]=lido[1];	
		fscanf(output, "%c", &lido[1]);
	}
	column[0]=1;
	for(i=1;i<=size-1;i++){
		fscanf(output,"%d",&lit);
		
		if(lit>0)column[i]=1;
		else column[i]=0;
	}
	fclose(output);	
}

//checks whether the valuations corresponding to the columns in the initial solution are consistent with Gamma 
void checkConsistency(int* notinGamma,int size){
	int i,j, *cons;
	ClauseGen* satInst;
	satInst = new ClauseGen();
	satInst->finalizeCNF();
	cons = new int[size];
	for(i=0;i<size;i++){
		satInst->restart();
		satInst->printGamma();	
		for(j=1;j<size;j++){
			if(j<=i)satInst->andN(1,j);
			else satInst->andN(1,-j);
		}
		satInst->finalizeCNF();
        //		system("./zchaff clauses.cnf | tee saidazc.out");
		system("./zchaff clauses.cnf > saidazc.out");
		getColumn(cons, size);
		if(cons[0]!=-1)notinGamma[i]=0;
	}
		
}

//given a basis and a column to enter, this funtions returns the index of the column to be replaced, in 
//order to keep the solution feasibility.
int columnOut(Matrix *basis, Matrix *inv,int * newColumn,int *notinGamma, double *basicSolution,int simplex){
	int i,j,out=-1,size;
	double *coef, max, epsilonCoef;
	size=basis->getSize();
	epsilonCoef = pow(2,size)/pow(size+1,(size+1)/2);
	coef=new double[size];
	for(i=0;i<size;i++)coef[i]=0;
	for(i=0;i<size;i++)for(j=0;j<size;j++)
		coef[i]+=inv->get(i,j)*newColumn[j];
	for(i=0;i<size;i++)if(fabs(coef[i])<epsilonCoef)coef[i]=0;
	max=0;	
	
	for(i=0;i<size && max>=0;i++)if(notinGamma[i]==1){
		if(basicSolution[i]!=0){
			if(coef[i]/basicSolution[i]>max){
				out=i;
				max=coef[i]/basicSolution[i];
			}
		}
		else if(coef[i]>0){
			max=-1;
			out=i;
		}
	}
	if(simplex==1)for(i=0;i<size && max>=0;i++)if(notinGamma[i]==0){
		if(basicSolution[i]!=0){
			if(coef[i]/basicSolution[i]>max){
				out=i;
				max=coef[i]/basicSolution[i];
			}
		}
		else if(coef[i]>0){
			max=-1;
			out=i;
		}
	}
	return out;
}

//given a basis and a column to enter, this funtion susbstitutes a column in the basis in 
//order to keep the solution feasibility.

void pivoting(Matrix *basis, Matrix *inv,int * newColumn, double *basicSolution,int *notinGamma,double epsilon){
	int i,j,out=-1,size;
	double *coef, max, epsilonCoef;
	size=basis->getSize();
	epsilonCoef = pow(2,size)/pow(size+1,(size+1)/2);
	coef=new double[size];
	for(i=0;i<size;i++)coef[i]=0;
	for(i=0;i<size;i++)for(j=0;j<size;j++)
		coef[i]+=inv->get(i,j)*newColumn[j];
	for(i=0;i<size;i++)if(fabs(coef[i])<epsilonCoef)coef[i]=0;
	max=0;	
	for(i=0;i<size && max>=0;i++){
		if(basicSolution[i]!=0){
			if(coef[i]/basicSolution[i]>max){
				out=i;
				max=coef[i]/basicSolution[i];
			}
		}
		else if(coef[i]>0){
			max=-1;
			out=i;
		}
	}
    // mfinger: assert(out >= 0)
 
	notinGamma[out]=0;
	for(i=1;i<size;i++)basis->set(i,out,newColumn[i]);
	if(basicSolution[out]!=0)basicSolution[out]=1/max;	
	for(i=0;i<size;i++)if(i!=out)basicSolution[i]+=-basicSolution[out]*coef[i];	
	for(i=0;i<size;i++)if(fabs(basicSolution[i])<epsilon)basicSolution[i]=0;
}

//Given a column to enter the basis and the index of the column to be replaced, 
//this function updates the basis.
void subsColumn(Matrix *basis, Matrix *inv,int * newColumn, double *
basicSolution, int* notinGamma, int out, double epsilon){
	int i,j,size;
	double *coef;
	size=basis->getSize();
	coef=new double[size];
	for(i=0;i<size;i++)coef[i]=0;
	for(i=0;i<size;i++)for(j=0;j<size;j++)
		coef[i]+=inv->get(i,j)*newColumn[j];
	notinGamma[out]=0;
	for(i=1;i<size;i++)basis->set(i,out,newColumn[i]);
	basicSolution[out]=basicSolution[out]/coef[out];	
	for(i=0;i<size;i++)if(i!=out)basicSolution[i]+=-basicSolution[out]*coef[i];	
	for(i=0;i<size;i++)if(fabs(basicSolution[i])<epsilon)basicSolution[i]=0;
}

/*This function decides the satisfiability of the PSAT instance. It first generates a SAT instance looking for a column that doesn't increase the sum of the probabilities of inconsistent columns (simplex). If there's no such a column, the PSAT instance is unsatisfiable. If the column founded substitutes for an inconsistent column (corresponding to a valuation inconsistent with Gamma), it will enter the basis and another iteration begins; Otherwise, it will be stored, and another SAT instance will be generated, looking for a column that could replace an inconsistent column. When there is such a column, it enters the basis; otherwise, we insert the column stored by the previous method. When there is no column inconsistent with Gamma whose probability is non-zero, a solution is returned.*/
void PSAT::solve(){
    int i,j,/*lixo,*/ *notinGamma,outCol, count,*columnSimplex,*newColumn, sat, nig=0, it=0;
    //long temp;
	double *basicSolution, cost=0, epsilon;
	Inequality ** inequalities;
	//FILE *out;
	ClauseGen* satIns;	
	
	inequalities = (Inequality **) malloc(sizeof(Inequality *)*(nProb));
	for(i=0;i<nProb;i++)inequalities[i] = new Inequality(nProb+1);
	/*out =*/ fopen("origMatrix.txt","w");	
	Matrix *basis, *inverse;
	basis= new Matrix(nProb+1);
	inverse = new Matrix(nProb+1);
	newColumn = new int[nProb+1];
	columnSimplex = new int[nProb+1];
	notinGamma = new int[nProb+1];
	for(i=0;i<nProb+1;i++)notinGamma[i]=1;
	checkConsistency(notinGamma, nProb+1);	
	for(i=0;i<nProb+1;i++)nig+=notinGamma[i];
	basicSolution = new double[nProb+1];
	basis->initialize();
	basicSolution[nProb] = probabilities[nProb];
	for(j=nProb-1;j>=0;j--)
	basicSolution[j]=probabilities[j]-probabilities[j+1];
	for(i=0;i<nProb+1;i++)if(notinGamma[i]==1)cost+=basicSolution[i];	
	epsilon = pow(10,-precision)*pow(2,nProb+1)/pow(nProb+2,(nProb+2)/2);
	satIns = new ClauseGen();
	satIns->finalizeCNF();
	
//begin iterations:
	while(nig>0&&cost>=epsilon){//nig is the number of columns corresponding to valuations inconsistent with Gamma
		if(VERB) printf("\nBasic Solution at the begining of iteration %d\n",it);		
		if(VERB) basis->print();
		if(VERB) printf("probabilities:\n");
		for(i=0;i<nProb+1;i++)if(VERB) printf("%.6lf ",basicSolution[i]);
		if(VERB) printf("\nColumns inconsistent with Gamma:\n");
		for(i=0;i<nProb+1;i++)if(VERB) printf("%8d ",notinGamma[i]);
		if(VERB) printf("\nCost: %3lf",cost);
		it++;
		inverse->copy(basis);
		inverse->invert();
		basis->setDet(round(1/inverse->getDet()));

	//trying simplex first
	
		if(VERB) printf("\n Trying Simplex, with reduction to SAT...\n");
		count=setSimplexInequality(inverse,basicSolution,notinGamma,inequalities);
		satIns->restart();		
		satIns->printGamma();	
		for(i=0;i<count;i++)satIns->ineqtoCNF(inequalities[i]);
		satIns->finalizeCNF();
        //		system("./zchaff clauses.cnf | tee saidazc.out");
		system("./zchaff clauses.cnf > saidazc.out");
		getColumn(columnSimplex, nProb+1);
		if(columnSimplex[0]<0){
			if(VERB) printf("Basis cost: %lf\n",cost);
			if(VERB)
				printf("\nUnsatisfiable PSAT instance. Total number of iterations:   %7d \n",it);
            else
                printf("UNSAT");

            cerr << "UNSAT" << endl;
				
			return ;
		}
		if(VERB) printf("\nNew column found.\n");
		for(i=0;i<nProb+1;i++)if(VERB) printf("%d ",columnSimplex[i]);
		outCol=columnOut(basis, inverse, columnSimplex,notinGamma, basicSolution,1);
		if(VERB) printf("\nColumn that would leave the basis:%d\n",outCol);
		for(i=0;i<nProb+1;i++)newColumn[i]=columnSimplex[i];
		sat=0;	
		if(notinGamma[outCol]==1)nig--;		
		else{	
		
			if(VERB) printf("\nThis column doesn't substitute a column inconsistent with Gamma!");
			if(VERB) printf("\nTrying to force a substitution for a column inconsistent with Gamma...");
		//try to substitute one column inconsistent with Gamma, if simplex doesn't
			for(i=0;i<nProb+1 && sat ==0;i++)if(notinGamma[i]==1){
				count=setInequalities(precision, inverse, basicSolution, i, notinGamma,inequalities);	
				satIns->restart();		
				satIns->printGamma();	
				for(j=0;j<count;j++)satIns->ineqtoCNF(inequalities[j]);
				satIns->finalizeCNF();
                //				system("./zchaff clauses.cnf | tee saidazc.out");
				system("./zchaff clauses.cnf > saidazc.out");
				getColumn(newColumn, nProb+1);
				if(newColumn[0]>0){
					sat=1;
					nig--;
					if(VERB) printf("\nNew Column found: ");
					for(j=0;j<nProb+1;j++)if(VERB) printf("%d ",newColumn[j]);
					if(VERB) printf("\n");			
					outCol=columnOut(basis, inverse, newColumn, notinGamma, basicSolution,0);
					if(VERB) printf("\nColumn that would leave the basis:%d",outCol);
				}
			}			
			if(sat==0){
				if(VERB) printf("\nNo column found; inserting the one found by simplex");		
				for(i=0;i<nProb+1;i++)newColumn[i]=columnSimplex[i];	
			}
		}
		subsColumn(basis, inverse, newColumn, basicSolution, notinGamma,outCol,epsilon);
		cost=0;
		for(i=0;i<nProb+1;i++)if(notinGamma[i]==1)cost+=basicSolution[i];			
	}
	if(VERB) {
      printf("\nInstance Satiafiable. Solution found:\n");
      basis->print();
      printf("Probabilities:\n");
      for(i=0;i<nProb+1;i++) printf("%.6lf ",basicSolution[i]);
      printf("\nColumns inconsistent with Gamma:\n");
      for(i=0;i<nProb+1;i++) printf("%8d ",notinGamma[i]);
      printf("******\n");
      printf("Total number of iterations:   %7d\n", it);
    }
    else
      printf("SAT");

  	cerr << "SAT" << endl;
}
