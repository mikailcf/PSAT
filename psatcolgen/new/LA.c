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

#ifdef _cplusplus
extern "C" {
#endif

#include<stdlib.h>
#include<stdio.h>
#include <string.h>
#include "LA.h"
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

double la(int size, double *A){
  int i, inf;
  double *w, determinant = 1;
  long *ip;

  w = (double *) malloc(size*sizeof(double));
  ip = (long *) malloc(size*sizeof(long));
  inf = dgetrf(size, size, A, size, ip);
  if (inf != 0) fprintf(stderr, "failure with error %d\n", inf); //LU decomposition
  for(i = 0; i < size; i++) determinant *= A[i*size+i]; //determinant of A
  inf = dgetri(size, A, size, ip, w, size);
  if (inf != 0) fprintf(stderr, "failure with error %d\n", inf);//inverse from LU

  return determinant;
}

#ifdef _cplusplus
}
#endif