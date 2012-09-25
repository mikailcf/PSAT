#include<stdlib.h>
#include<stdio.h>
#include <string.h>
//||

/*static long //passar a matriz A transposta!!!!
dgesv(long n, long nrhs, double *A, long lda,  long *ipiv, double *B,  long ldb) 
{
  extern void dgesv_(const long *n, const long  *nrhs,
		     double *A, const long *lda, long *ipiv, double *B, const long *ldb,
		     long *info);
  long info;
  dgesv_(&n, &nrhs, A, &lda, ipiv, B, &ldb, &info );
  return info;
}*/

/*
double A[] = {
  1, 1, 2, 
  2, 4, -3, 
  3, 6, -5
};

double w[] = {
  0, 0, 0
};

long ip[] = {
   1, 2, 3
};
*/

static long
dgetri(long N, double *A, long LDA, long *IPIV, double *WORK, long LWORK)
{
  extern void dgetri_(const long *N, double *A, const long *LDA, long *IPIV, 
		double *WORK, long *LWORK, long *infop);
  long info;
  dgetri_(&N, A, &LDA, IPIV, WORK, &LWORK, &info);
  return info;
}

static long
dgetrf(long M, long N, double *A, long LDA, long *IPIV)
{
  extern void dgetrf_(const long *M,const long *N, double *A,const long *LDA,
	 long *IPIV, long *infop);
  long info;
  dgetrf_(&M, &N, A, &LDA, IPIV, &info);
  return info;
}

int main(){
  int i, j, inf, size;
  double *A, *w, determinant=1;
  long *ip;
  FILE *input, *output;
  input = fopen("origMatrix.txt", "r");
  fscanf(input, "%d", &size);
  A = (double *) malloc(size*size*sizeof(double));
  for(i=0;i<size*size;i++)fscanf(input, "%lf", &A[i]);
  w = (double *) malloc(size*sizeof(double));
  ip = (long *) malloc(size*sizeof(long));
  inf = dgetrf(size,size,A,size,ip);
  if (inf != 0) fprintf(stderr, "failure with error %d\n", inf); //LU decomposition
  for(i=0;i<size;i++)determinant*=A[i*size+i]; //determinant of A
  inf = dgetri(size, A, size, ip, w, size);
  if (inf != 0) fprintf(stderr, "failure with error %d\n", inf);//inverse from LU
  output = fopen("invMatrix.txt","w");
  fprintf(output,"%lf\n",determinant);//determinant of A
  for (i=0; i<size; ++i){
	for(j=0; j<size; j++)fprintf(output,"%5.9lf ", A[i*size+j]);
	fprintf(output,"\n");	
  }
  fclose(output);
  fclose(input);
  
  
//  printf("optimal Lw = %lf\n",w[0]);
  return 0;
}

