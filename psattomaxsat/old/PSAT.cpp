#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "PSAT.h"

#include <iostream>

using namespace std;

//||

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
	//basis = new double[(nProb+1)*(nProb+1)];
	
}

PSAT::~PSAT(){
	delete probabilities;
}
//set inequalities to force one column to be replaced in the basis
int PSAT::setInequalities(int prec, Matrix *inv, double *basicSol, 
			int column, Inequality **ineq){
	int det, i, j,k,count=0;
	det=round(1/inv->getDet());
	for(i=0;i<inv->getSize();i++)
		for(j=0;j<inv->getSize();j++)
			inv->set(i,j,round(inv->get(i,j)*det));	
	for(j=0;j<inv->getSize();j++)if(column!=j){
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

int PSAT::setSimplexInequality(FILE *saida, Matrix *inv,double *basicSolution, int *notinGamma, Inequality **ineq){
	int det, i, j,k,count=1;
	double temp;
	det=round(1/inv->getDet());
	for(i=0;i<inv->getSize();i++)
		for(j=0;j<inv->getSize();j++){
			temp=inv->get(i,j)*fabs(det);
			if(fabs(round(temp)-temp)>0.4)fprintf(saida, "%4.4lf",temp);			
			inv->set(i,j,round(temp));	
		}
	ineq[0]->set(0,-1);
	for(k=1;k<inv->getSize();k++)ineq[0]->set(k,0);

	for(j=0;j<inv->getSize();j++)if(notinGamma[j]==1)
		for(k=0;k<inv->getSize();k++)
			ineq[0]->set(k,ineq[0]->get(k)+inv->get(j,k));
	
	/*for(i=0;i<inv->getSize();i++)if(basicSolution[i]==0&&notinGamma[i]==0){
		for(k=0;k<inv->getSize();k++)
			ineq[count]->set(k,-inv->get(i,k));
		count++;
	}*/
	for(i=0;i<inv->getSize();i++)
		for(j=0;j<inv->getSize();j++)
			inv->set(i,j,inv->get(i,j)/fabs(det));		
	return count;
}

void getColumn(int *column, int size, long limit){
	FILE *output;
	int sat=0, i,lit;
	char *lido;
	long optLost;
	lido = new char[50];
	output=fopen("saidamax.out","r"); //maxsat's output
	lido[0]='c';
	while(lido[0]!='s')fgets(lido, 50, output);
	if(lido[2]=='O')sat=1;
	if(sat==0){
		column[0]=-1;
		delete lido;
		return ;
	}
	fscanf(output,"%20c",lido);
	fscanf(output,"%ld",&optLost);
	if(optLost>limit){
		column[0]=-1;
		delete lido;
		return ;
	}
	fscanf(output,"%2c",lido);
	column[0]=1;
	for(i=1;i<=size-1;i++){
		fscanf(output,"%d",&lit);
		if(lit>0)column[i]=1;
		else column[i]=0;
	}
	
	delete lido;
	fclose(output);	
}
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
		//for(i=0;i<=10000;i++);
		//system("./zchaff clauses.cnf | tee saidazc.out");
		system("./zchaff clauses.cnf > saidazc.out 2> warningzc.out");
		getColumn(cons, size);
		if(cons[0]!=-1)notinGamma[i]=0;
	}
	
	//delete satInst;	
}

void pivoting(Matrix *basis, Matrix *inv,int * newColumn, double *basicSolution,int *notinGamma,double epsilon){
	int i,j,out=0,size,nz;
	double *coef, max, epsilonCoef;
	long *lex;
	size=basis->getSize();
	epsilonCoef = fabs(0.5/basis->getDet());
	coef=new double[size];
	for(i=0;i<size;i++)
      coef[i]=0;
	for(i=0;i<size;i++)
      for(j=0;j<size;j++)
		coef[i]+=inv->get(i,j)*newColumn[j];
	for(i=0;i<size;i++)
      if(fabs(coef[i])<epsilonCoef)
        coef[i]=0;
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
	nz=0;
	if(max==-1){
		for(i=0;i<size;i++)if(basicSolution[i]==0 && coef[i]>0)nz++;
		if(nz>1){		
			lex= new long[nz];
			for(i=0;i<nz;i++)lex[i]=0;
			for(i=0;i<size;i++)if(basicSolution[i]==0 && coef[i]>0)
						for(j=1;j<size;j++)lex[j]+=pow(2,basis->get(i,j));
			for(i=0;i<size;i++)if(basicSolution[i]==0 && coef[i]>0){
				if(lex[i]>max){
					max=lex[i];
					out=i;
				}
			}
		}
	}
	notinGamma[out]=0;
	for(i=1;i<size;i++)basis->set(i,out,newColumn[i]);
	if(basicSolution[out]!=0)basicSolution[out]=1/max;	
	for(i=0;i<size;i++)if(i!=out)basicSolution[i]+=-basicSolution[out]*coef[i];	
	
}

void subsColumn(Matrix *basis, Matrix *inv,int * newColumn, double *
basicSolution, int* notinGamma, int out){
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
}

void PSAT::solve(){
	FILE *fout = fopen("sat", "w");
	int i,j,count, *notinGamma, *newColumn, sat, nig=0, it=0, *signal;
	long /* temp,*/ top/*, indterm*/, value;
	double *basicSolution, cost=0, epsilon;
	Inequality ** inequalities;
	FILE *out,*status, *saida;
	saida = fopen("saida.txt","w");
	ClauseGen* satIns;		
	signal = new int[nProb+1];
	inequalities = (Inequality **) malloc(sizeof(Inequality *)*(nProb));
	for(i=0;i<nProb;i++)inequalities[i] = new Inequality(nProb+1);
	out = fopen("origMatrix.txt","w");
    if( !out ) {
      cerr << "*** origMatrix.txt could not be open for writing\n"
           << "Terminating" << endl;
      exit(2);
    }
	Matrix *basis, *inverse;
	basis= new Matrix(nProb+1);
	inverse = new Matrix(nProb+1);
	newColumn = new int[nProb+1];
	notinGamma = new int[nProb+1];
	for(i=0;i<nProb+1;i++)
      notinGamma[i]=1;
	// checkConsistency(notinGamma, nProb+1);	
	for(i=0;i<nProb+1;i++)
      nig+=notinGamma[i];
	basicSolution = new double[nProb+1];
	basis->initialize();
	basicSolution[nProb] = probabilities[nProb];
	for(j=nProb-1;j>=0;j--)
      basicSolution[j]=probabilities[j]-probabilities[j+1];
	for(i=0;i<nProb+1;i++)
      if(notinGamma[i]==1)
        cost+=basicSolution[i];	
	epsilon = pow(10,-precision)*pow(2,nProb+1)/pow(nProb+2,(nProb+2)/2);
	satIns = new ClauseGen();
	satIns->finalizeCNF();
	
//begin iterations:
	inverse->copy(basis);
	inverse->invert();
	basis->updateDet();
	epsilon=fabs(pow(10,-precision)/basis->getDet());
	while(cost>=epsilon){
		it++;		
		//inverse->copy(basis);
		//inverse->invert();
		//basis->updateDet();
		printf("\nBasic Solution at the begining of iteration %d\n",it);		
		basis->print();
		printf("Basis' Determinant: %15.0lf\n",basis->getDet());
		printf("probabilities:\n");
		for(i=0;i<nProb+1;i++)printf("%.6lf ",basicSolution[i]);
		printf("\nColumns inconsistent with Gamma:\n");
		for(i=0;i<nProb+1;i++)printf("%8d ",notinGamma[i]);
		printf("\n");
		printf("\nBasis Inverse at the begining of iteration %d\n",it);		
		inverse->print();
		
//	printf("inverse of basis:\n");
//	inverse->print();	

		sat=0;
	
	//trying simplex, if could not substitute an inconsistent column

		//printf("\n Trying Simplex, with reduction to SAT...\n");
		count=setSimplexInequality(saida,inverse,basicSolution,notinGamma,inequalities);
		printf("\nCost: %lf inequality:",cost);
		inequalities[0]->print();
		for(i=1;i<=nProb;i++){
			value = inequalities[0]->get(i);
			if(value<0){
				inequalities[0]->set(i,-value);
				inequalities[0]->set(0,inequalities[0]->get(0)+value);			
				signal[i]=-1;
			}
			else signal[i]=1;
		}
		printf("\nnew inequality:");
		inequalities[0]->print();
		top=1;
		for(i=1;i<=nProb;i++)top+=inequalities[0]->get(i);
		if(top<0){
			printf("\nOverflow!!!!!! Total number of iterations:   %7d \n",it);
			fclose(saida);
			return ;
		}	
		satIns->restart();		
		satIns->printGamma(top);	
		for(i=0;i<count;i++)satIns->ineqtoWCNF(inequalities[i], signal);
		satIns->finalizeWCNF(top);
		system("./maxsat clauses.cnf > saidamax.out 2> warningmax.out");
        //		system("./maxsat clauses.cnf | tee saidazc.out");
		getColumn(newColumn, nProb+1, top-1+inequalities[0]->get(0));
		if(newColumn[0]>0){
			sat=1;
			printf("\nNew Column found:\n");
			for(i=0;i<nProb+1;i++)printf("%d ",newColumn[i]);
			pivoting(basis, inverse, newColumn, basicSolution, notinGamma,epsilon);
			inverse->copy(basis);
			inverse->invert();
			basis->updateDet();
			epsilon=fabs(0.5*pow(10,-precision)/basis->getDet());
			for(i=0;i<nProb+1;i++)if(fabs(basicSolution[i])<epsilon)basicSolution[i]=0;		
			cost=0;
			for(i=0;i<nProb+1;i++)if(notinGamma[i]==1)cost+=basicSolution[i];
		}	
		if(sat==0){
			printf("cost: %.15lf, epsilon:%.15lf\n",cost,epsilon);
			printf("\nUnsatisfiable PSAT instance. Total number of iterations:   %7d \n",it);
			fclose(saida);
            // cerr << "UNSAT" << endl;
            fprintf(sat, "UNSAT\n");
			return ;
		}
		status = fopen("status.txt","w");
		fprintf(status,"iteration: %d cost: %lf",it,cost);
		fclose(status);
		//if(it>=100){
		//	printf("\nProbable Infinite Loop after 100 iterations!\n");
		//	fclose(saida);
		//	return ;
		//}
	}
	printf("\nPSAT Solution found in %d iterations:\n",it);
	basis->print();
	printf("Probabilities:\n");
	for(i=0;i<nProb+1;i++)printf("%.6lf ",basicSolution[i]);
	printf("\nColumns inconsistent with Gamma:\n");
	for(i=0;i<nProb+1;i++)printf("%8d ",notinGamma[i]);
	printf("******\n");
	printf("Total number of iterations:   %7d\n", it);
	fclose(saida);	

    // cerr << "SAT" << endl;
    fprintf(sat, "SAT\n");
/*	printf("Assigned probabilities: ");
	for(i=0;i<=nProb;i++)printf("%lf ",probabilities[i]);
	printf("\nbasic solution: ");
	for(i=0;i<=nProb;i++)printf("%lf ",basicSolution[i]);
	printf("\ninequalities:");
	for(i=0;i<nProb;i++)inequalities[i]->print();
	printf("\nDeterminante da base: %lf\n",basis->getDet());
*/

//TO DO: if basicSolution[columnToOut]==0 then coef[cTO]!=0 eh unica ineq!!!
//mas como implementar? try > first, then <...
//faz diferenca?
}
