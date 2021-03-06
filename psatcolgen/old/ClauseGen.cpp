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


/*This class is responsible for the construction of the .cnf file containing the SAT instance
resulting from the PSAT instance reduction.*/

#include<stdarg.h>
#include<stdio.h>
#include<math.h>
#include"ClauseGen.h"

ClauseGen::ClauseGen(){
	int i;	
	cnf=fopen("clauses.cnf","w");
	topAtom = 1;			//number for a new atom to enter the SAT instance 
	countClause = 0;		//total number of clauses
	varGamma = -1;			//auxialiary variable to point where Gamma was printed
	for(i=0;i<201;i++)fprintf(cnf," ");//empty space to print the header later 
}

ClauseGen::~ClauseGen(){
	fclose(cnf);	
}


//This function reopen the file clauses.cnf o start printing clauses from scratch
void ClauseGen::restart(){
	int i;
	//fclose(cnf);	
	cnf=fopen("clauses.cnf","w");
	topAtom = 1;
	countClause = 0;
	varGamma = -1;
	for(i=0;i<201;i++)fprintf(cnf," ");//space to print header later
}

//This function print Gamma clauses in the CNF file
void ClauseGen::printGamma(){
	int i,nClause, nVar;
	long lit;
	FILE * gamma;
	gamma=fopen("gamma.txt","r");
	fscanf(gamma,"%d",&nVar);
	fscanf(gamma,"%d",&nClause);
	topAtom--;
	for(i=0;i<nClause;i++){    
		fscanf(gamma,"%ld",&lit);
		while(lit!=0){
			if(lit>0)lit+=topAtom;
			else lit-=topAtom;
			fprintf(cnf,"%ld ",lit);
			fscanf(gamma,"%ld",&lit);
		}
        	fprintf(cnf,"0\n");
        }
	topAtom=nVar+1;
	countClause+=nClause;
	fclose(gamma);
}
//prints the clauses corresponding to the "exclusive OR" operation between atoms 'a' and 'b'
void ClauseGen::xor2( long a, long b){
	countClause+=2;
	fprintf(cnf,"%ld %ld 0\n", a, b);
	fprintf(cnf,"%ld %ld 0\n", -a, -b);
}

//prints the clauses corresponding to the equivalence between 'r' and the result of the "exclusive OR" operation between literals 'a' and 'b'
void ClauseGen::eq_xor2( long r, long a, long b){
	countClause+=4;
	fprintf(cnf,"%ld %ld %ld 0\n", -r, a, b);
	fprintf(cnf,"%ld %ld %ld 0\n", -r, -a, -b);
	fprintf(cnf,"%ld %ld %ld 0\n", r, -a, b);
	fprintf(cnf,"%ld %ld %ld 0\n", r, a, -b);
}
//prints the clauses corresponding to the "exclusive OR" operation among 'n' literals
void ClauseGen::orN( int n, ...){
	countClause+=1;
	va_list args;
	va_start(args, n);
	int j;
	for(j=0;j<n; j++)fprintf(cnf,"%ld ",va_arg(args, long));
	fprintf(cnf, "0\n");
	va_end(args);
}

//prints the clauses corresponding to the "exclusive OR" operation among 'n' literals, given as an array
void ClauseGen::orNlist( int n, long *list){
	int i;
	countClause+=1;
	for(i=0;i<n;i++)fprintf(cnf,"%ld ",list[i]);
	fprintf(cnf, "0\n");
}

//prints the clauses corresponding to the "OR" operation between 2 literals
void ClauseGen::or2( long a,long b){orN(2, a, b);}

//prints the clauses corresponding to the equivalence between 'r' and the result of the "OR" operation among 'n' literals
void ClauseGen::eq_orN( int n, long r, ...){
	countClause+=n+1;
	va_list args;
	va_start (args, r);
	int j;
	fprintf(cnf, "%ld ",-r);
	for(j=0;j<n; j++)fprintf(cnf,"%ld ",va_arg(args, long));
	fprintf(cnf, "0\n");
	va_start (args, r);
	for(j=0;j<n; j++)fprintf(cnf,"%ld %ld 0\n",-va_arg(args, long), r);
	va_end(args);
}

//prints the clauses corresponding to the equivalence between 'r' and the result of the "OR" operation between literals 'a' and 'b'
void ClauseGen::eq_or2( long r, long a, long b){eq_orN(2, r, a, b);}

//prints the clauses corresponding to the "AND" operation between 'n' literals
void ClauseGen::andN( int n, ...){
	countClause+=n;	
	va_list args;
	va_start(args, n);
	int j;
	for(j=0;j<n; j++)fprintf(cnf,"%ld 0\n",va_arg(args, long));
	va_end(args);
}
//prints the clauses corresponding to the "AND" operation between literals 'a' and 'b
void ClauseGen::and2( long a, long b){andN( 2, a, b);}

//prints the clauses corresponding to the equivalence between 'r' and the result of the "AND" operation among 'n' literals
void ClauseGen::eq_andN( int n, long r, ...){
	countClause+=n+1;
	va_list args;
	va_start (args, r);
	int j;
	fprintf(cnf, "%ld ",r);
	for(j=0;j<n; j++)fprintf(cnf,"%ld ",-va_arg(args, long));
	fprintf(cnf, "0\n");
	va_start (args, r);
	for(j=0;j<n; j++)fprintf(cnf,"%ld %ld 0\n",va_arg(args, long), -r);
	va_end(args);
}

//prints the clauses corresponding to the equivalence between 'r' and the result of the "AND" operation between literals 'a' and 'b'
void ClauseGen::eq_and2( long r, long a, long b){eq_andN(2, r, a, b);}

//prints the clauses corresponding to the equivalence between 'r' and the result of the "equivalence" operation between literals 'a' and 'b'
void ClauseGen::eq_equiv2(long r, long a, long b){
	countClause += 4;
	fprintf(cnf, "%ld %ld %ld 0\n",-r, a, -b);//r->(a v ¬b)
	fprintf(cnf, "%ld %ld %ld 0\n",-r, -a, b);//r->(¬a v b)
	fprintf(cnf, "%ld %ld %ld 0\n",-a, -b, r);//ab -> r
	fprintf(cnf, "%ld %ld %ld 0\n", a,  b, r);//¬a¬b -> r
}

//prints the clauses corresponding to the equivalence between the array of literals 'sum' and the result of the bitwise sum operation between two array of literals, p1 an p2 - all arrays with 'bits' literals representing integer numbers in the binary system.
void ClauseGen::eq_sum( long* sum, long * p1, long *p2, int bits){ 
	int k,carry, carrytemp1,carrytemp2, lastcarry, sumtemp;
	carry=topAtom++;
	eq_and2(carry, p1[0], p2[0]);
	eq_xor2(sum[0], p1[0],p2[0]);
	for(k=1;k<bits;k++){	
		lastcarry=carry;
		carrytemp1=topAtom++;
		eq_and2(carrytemp1, p1[k], p2[k]);
		sumtemp=topAtom++;
		eq_xor2( sumtemp, p1[k],p2[k]);
		eq_xor2(sum[k], sumtemp, lastcarry);
		carrytemp2=topAtom++;
		eq_and2(carrytemp2, sumtemp,lastcarry);
		carry=topAtom++;
		eq_or2( carry, carrytemp1,carrytemp2);
	}
	andN(1, -carry);
}

//prints the clauses corresponding to the  "greater than or equal to" operation between two array of literals, p1 an p2 - all arrays with 'bits' literals representing integer numbers in the binary system.
void ClauseGen::geq(long* p1, long* p2, int bits){
	long conj, disj, greater, equal;
	int i;
	conj = topAtom++;
	eq_equiv2(conj, p1[0],p2[0]);
	for(i=0;i<bits-1;i++){
		greater = topAtom++;
		eq_and2(greater, p1[i],-p2[i]);
		disj = topAtom++;
		eq_or2(disj, greater, conj);
		equal = topAtom++;
		eq_equiv2(equal, p1[i+1],p2[i+1]);
		conj = topAtom++;
		eq_and2(conj, equal, disj);
	}
	greater = topAtom++;
	eq_and2(greater, p1[bits-1],-p2[bits-1]);
	or2(greater, conj);
}

//convets p to the binary
void longToBin(long p, int k,int *out)
{
  int x = p;
  for (int i = 0; i < k ; i++)
    {
      out[i] = x % 2;
      x = x / 2;
    }
}

//this function converts an Inequality into a clauses.
void ClauseGen::ineqtoCNF(Inequality *ineq){
	int bits,i,j,*signal,size,*out;
	long /*max,*/ sum=0, value, **coef,**partialSum,**coefY;

	size=ineq->getSize();
	signal= new int[size];
	for(i=1;i<size;i++){
		value = ineq->get(i);
		if(value<0){
			ineq->set(i,-value);
			ineq->set(0,ineq->get(0)+value);			
			signal[i]=-1;
		}
		else signal[i]=1;
	}
	if(ineq->get(0)>=0)return ;//inequality is a tautology, unfalsifiable
	for(i=1;i<size;i++)sum+=ineq->get(i);
	if(sum<-ineq->get(0)){//unsatisfiable inequality 
		and2(1,-1);
		return ;
	}
	bits=floor(log(sum)/log(2))+2;
	//printf("\nbits:%d\n",bits);
	out=new int[bits];
	coef=initVarDim2(size,bits);//coef[0] is the coeficient of y_1 or ¬y_1 in the inequality
	coefY=initVarDim2(size-1,bits);
	partialSum=initVarDim2(size-2,bits); 
	for(i=1;i<size;i++){
		value=ineq->get(i);
		longToBin(value,bits,out);
		for(j=0;j<bits;j++){
			andN(1,coef[i-1][j]*(out[j]*2-1)); 
			eq_and2(coefY[i-1][j],i*signal[i],coef[i-1][j]);	
		}
	}
	eq_sum(partialSum[0],coefY[0],coefY[1],bits);
	for(i=1;i<=size-3;i++)eq_sum(partialSum[i],partialSum[i-1],coefY[i+1],bits);
	longToBin(-ineq->get(0),bits,out);//set the indepent coeficient, passig it to the rhs
	for(j=0;j<bits;j++)andN(1,coef[size-1][j]*(out[j]*2-1));
	geq(partialSum[size-3],coef[size-1],bits);
}


//the following functions gives new atoms in the SAT instance for variables of dimensions 0,1,2 and 3.
long ClauseGen::initVarDim0(){
	return topAtom++;
}

long* ClauseGen::initVarDim1(int size){
	int i;
	long * vector;
	vector = new long[size];
	for(i=0;i<size;i++) vector[i]= topAtom++;
	return vector;
}

long** ClauseGen::initVarDim2(int nRow, int nCol){
	int i, j;
	long ** matrix;
	matrix = new long*[nRow];
	for(i=0;i<nRow;i++) matrix[i] = new long[nCol];
	for(i=0;i<nRow;i++)for(j=0;j<nCol;j++) matrix[i][j]=topAtom++;
	return matrix;
}

long*** ClauseGen::initVarDim3(int n1, int n2, int n3){
	int i, j, k;
	long ***matrix3;
	matrix3 = new long**[n1];
	for(i=0;i<n1;i++) matrix3[i] = new long*[n2];
	for(i=0;i<n1;i++)for(j=0;j<n2;j++) matrix3[i][j] = new long[n3];
	for(i=0;i<n1;i++)for(j=0;j<n2;j++)for(k=0;k<n3;k++)matrix3[i][j][k] = topAtom++;
	return matrix3;
}

long** ClauseGen::initCoefMatrix(int dim, int nVar){
	int i, j, temp=topAtom;
	long **matrix; 
	matrix = new long*[dim];
	for(i=0;i<dim;i++) matrix[i] = new long[dim];
	if(varGamma != -1){// Gamma has already been printed, elements of A are first atoms in Gamma
		temp = topAtom;
		topAtom = varGamma;
	}
	else varGamma = topAtom; //Gamma has not benn printed yet, we keep record of the first atom to print Gamma later
	for(i=0;i<dim;i++){ //initialize the matrix A;
		for(j=0;j<dim-1;j++)matrix[j][i]=topAtom++; 
		topAtom=(i+1)*(nVar+1);
		matrix[dim-1][i]=topAtom++; //variables from the bottom line =1, for the probabilities to sum up to one
	}
	if(varGamma != -1)topAtom=temp;
	return matrix;
}

//prints the header at the file's begining
void ClauseGen::finalizeCNF(){
	rewind(cnf);
	fprintf(cnf,"c This file was automatically generated and it's a\n");//50 char
	fprintf(cnf,"c reduction to SAT from a PSAT instance\n");//53 char
	fprintf(cnf, "p cnf %15ld %15ld\n",topAtom-1,countClause);//37 char
	fprintf(cnf,"c %15ld variables and %15ld clauses\n",topAtom-1,countClause);//56 char
	fclose(cnf);
}
