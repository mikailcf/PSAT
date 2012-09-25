#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>
#include"ClauseGen.h"
//||

ClauseGen::ClauseGen(){
	int i;	
	cnf=fopen("clauses.cnf","w");
	topAtom = 1;
	countClause = 0;
	varGamma = -1;
	for(i=0;i<201;i++)fprintf(cnf," ");//espaço para posterior cabecalho
}

ClauseGen::~ClauseGen(){
	fclose(cnf);	
}

void ClauseGen::restart(){
	int i;
	//fclose(cnf);	
	cnf=fopen("clauses.cnf","w");
	topAtom = 1;
	countClause = 0;
	varGamma = -1;
	for(i=0;i<201;i++)fprintf(cnf," ");//espaço para posterior cabecalho
}

void ClauseGen::printGamma(){
	int i,nClause, nVar;
	long lit;
	FILE * gamma;
	gamma=fopen("gamma.txt","r");
	fscanf(gamma,"%d",&nVar);
	fscanf(gamma,"%d",&nClause);
	topAtom--;
	for(i=0;i<nClause;i++){     		//guarda gamma em um txt
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

void ClauseGen::printGamma(long top){
	int i,nClause, nVar;
	long lit;
	FILE * gamma;
	gamma=fopen("gamma.txt","r");
	fscanf(gamma,"%d",&nVar);
	fscanf(gamma,"%d",&nClause);
	topAtom--;
	for(i=0;i<nClause;i++){     		//guarda gamma em um txt
		fprintf(cnf,"%ld ",top);
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

void ClauseGen::xor2( long a, long b){
	countClause+=2;
	fprintf(cnf,"%ld %ld 0\n", a, b);
	fprintf(cnf,"%ld %ld 0\n", -a, -b);
}

void ClauseGen::eq_xor2( long r, long a, long b){
	countClause+=4;
	fprintf(cnf,"%ld %ld %ld 0\n", -r, a, b);
	fprintf(cnf,"%ld %ld %ld 0\n", -r, -a, -b);
	fprintf(cnf,"%ld %ld %ld 0\n", r, -a, b);
	fprintf(cnf,"%ld %ld %ld 0\n", r, a, -b);
}

void ClauseGen::orN( int n, ...){
	countClause+=1;
	va_list args;
	va_start(args, n);
	int j;
	for(j=0;j<n; j++)fprintf(cnf,"%ld ",va_arg(args, long));
	fprintf(cnf, "0\n");
	va_end(args);
}

void ClauseGen::orNlist( int n, long *list){
	int i;
	countClause+=1;
	for(i=0;i<n;i++)fprintf(cnf,"%ld ",list[i]);
	fprintf(cnf, "0\n");
}

void ClauseGen::or2( long a,long b){orN(2, a, b);}

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

void ClauseGen::eq_or2( long r, long a, long b){eq_orN(2, r, a, b);}

void ClauseGen::andN( int n, ...){
	countClause+=n;	
	va_list args;
	va_start(args, n);
	int j;
	for(j=0;j<n; j++)fprintf(cnf,"%ld 0\n",va_arg(args, long));
	va_end(args);
}

void ClauseGen::and2( long a, long b){andN( 2, a, b);}

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

void ClauseGen::eq_and2( long r, long a, long b){eq_andN(2, r, a, b);}

void ClauseGen::eq_equiv2(long r, long a, long b){
	countClause += 4;
	fprintf(cnf, "%ld %ld %ld 0\n",-r, a, -b);//r->(a v ¬b)
	fprintf(cnf, "%ld %ld %ld 0\n",-r, -a, b);//r->(¬a v b)
	fprintf(cnf, "%ld %ld %ld 0\n",-a, -b, r);//ab -> r
	fprintf(cnf, "%ld %ld %ld 0\n", a,  b, r);//¬a¬b -> r
}

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

void longToBin(long p, int k,int *out)
{
  int x = p;
  for (int i = 0; i < k ; i++)
    {
      out[i] = x % 2;
      x = x / 2;
    }
//  return bin;
}

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

void ClauseGen::ineqtoWCNF(Inequality *ineq, int *signal){
	int size,i;
	long sum=0;
	size=ineq->getSize()-1;
	//if(ineq->get(0)>=0)return ;inequality is a tautology, unfalsifiable
	for(i=1;i<=size;i++)sum+=ineq->get(i);
	if(sum<-ineq->get(0)){//unsatisfiable inequality 
		or2(sum+1,1);
		or2(sum+1,-1);
		return ;
	}
	for(i=1;i<=size;i++)or2(ineq->get(i),i*signal[i]);	
	return ;
}

/*void ClauseGen::eq_mul( long* product, long * p1, long *p2, int bits1, int bits2){//bits1 <= bits2
	int i,j,k,tbits, bitstemp;
	long** parcelas,*temp, **partialSums;
	if(bits1>bits2){
		temp=p1;
		p1=p2;
		p2=temp;
		bitstemp=bits1;
		bits1=bits2;
		bits2=bitstemp;
	}
	parcelas = new long*[bits1];
	for(i=0;i<bits1;i++)parcelas[i]=new long[tbits];
	tbits = bits1+bits2;
	for(i=0;i<bits1;i++){//gera parcelas, multiplicando p2 por cada bit de p1
		for(j=0;j<tbits;j++)parcelas[i][j]=topAtom++;
		for(j=0;j<i;j++)andN(1, -parcelas[i][j]);
		for(j=0;j<bits2;j++)eq_and2(parcelas[i][j+i], p1[i], p2[j]);
		for(j=i+bits2;j<tbits;j++)andN(1, -parcelas[i][j]);
	}
	if(bits1==1){
		for(j=0;j<tbits;j++)eq_andN(2,product[j],parcelas[0][j]);
		return ;
	}
	if(bits1==2){
		eq_sum( product, parcelas[1],parcelas[0],tbits);
		return ;
	}
	partialSums = new long*[bits1-2];
	for(i=0;i<bits1-2;i++)partialSums[i]=new long[tbits];
	for(i=0;i<bits1-2;i++)for(j=0;j<tbits;j++)partialSums[i][j]=topAtom++;
	eq_sum(partialSums[0],parcelas[0],parcelas[1],tbits);
	for(i=1;i<bits1-2;i++)eq_sum(partialSums[i],partialSums[i-1],parcelas[i+1],tbits);
	eq_sum(product, partialSums[bits1-3],parcelas[bits1-1],tbits);
}*/

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
	int i, j, temp=-1;
	long **matrix; 
	matrix = new long*[dim];
	for(i=0;i<dim;i++) 
      matrix[i] = new long[dim];
	if(varGamma != -1){// Gamma já impresso, coeficientes são os mesmos átomos de gamma
		temp = topAtom;
		topAtom = varGamma;
	}
	else varGamma = topAtom; //Gamma ainda não impresso, guarda-se o primeiro átomo para futura impressão
	for(i=0;i<dim;i++){ //inicializa matriz A dos coeficientes; dim = nProb+1
		for(j=0;j<dim-1;j++)
          matrix[j][i]=topAtom++; 
		topAtom=(i+1)*(nVar+1);
		matrix[dim-1][i]=topAtom++; //variavel referente a ultimanha linha, =1 para somar 1
	}
	if(varGamma != -1)
      topAtom=temp;
	return matrix;
}

void ClauseGen::finalizeCNF(){
	rewind(cnf);
	fprintf(cnf,"c This file was automatically generated and it's a\n");//50 char
	fprintf(cnf,"c reduction to SAT from a Probabilistic SAT instance\n");//53 char
	fprintf(cnf, "p cnf %15ld %15ld\n",topAtom-1,countClause);//37 char
	fprintf(cnf,"c %15ld variáveis e %15ld cláusulas\n",topAtom-1,countClause);//56 char
	fclose(cnf);
}

void ClauseGen::finalizeWCNF(long top){
	rewind(cnf);
	fprintf(cnf,"c This file was automatically generated and it's a\n");//50 char
	fprintf(cnf,"c reduction to SAT from a Probabilistic SAT instance\n");//53 char
	fprintf(cnf, "p wcnf %8ld %8ld %10ld\n",topAtom-1,countClause, top);//37 char
	fclose(cnf);
}
