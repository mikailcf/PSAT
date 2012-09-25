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


#include"Inequality.h"
#include "Solver.h"
class ClauseGen{
    private:
    long topAtom;
    long countClause;
    long varGamma;
    FILE* cnf;
    Solver *solver;

    public:
    long gettopAtom(){return topAtom;}
    long getcountClause(){return countClause;}
    FILE* getcnf(){return cnf;}

    ClauseGen(Solver *s);
    ~ClauseGen();
    
    void restart(); 
    void restartSolver(Solver *s);
    void addGamma();

    void xor2( long a, long b);
    void eq_xor2( long r, long a, long b);
    void orN( int n, ...);
    void orNlist( int n, long *list);
    void or2( long a,long  b);
    void eq_orN( int n, long r, ...);
    void eq_or2( long r, long a, long b);
    void andN( int n, ...);
    void and2( long a, long b);
    void eq_andN( int n, long r, ...);
    void eq_and2( long r, long a, long b);
    void eq_equiv2(long r, long a, long b);
    void eq_sum( long* sum, long * p1, long *p2, int bits);
    void geq(long* p1, long* p2, int bits);

    long initVarDim0();
    long* initVarDim1(int size);
    long** initVarDim2(int nRow, int nCol);
    long*** initVarDim3(int n1, int n2, int n3);
    long** initCoefMatrix(int nRow, int nCol);
    void finalizeCNF();
    void ineqtoCNF(Inequality *ineq);
};
