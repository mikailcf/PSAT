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


/*This class is used to represent inequalities in the following way:
If we have k {0,1} variables, y_1 to y_k, the array "coeficients" contains
the integer coeficients, coeficients[0]=w_0...coeficient[k]=w_k, of the inequality:
w_0 + w_1.y+1+ ... + w_1.y_k >=0
Note that "coeficient[0]" contains the independent term. */


#include<stdio.h>
#include"Inequality.h"

Inequality::Inequality(int sz){
	size=sz;
	coeficients=new long[size];
}

Inequality::~Inequality(){
	delete [] coeficients;
}

long Inequality::get(int index){
	return coeficients[index];
}


void Inequality::print(){
	int i;
	printf("\n");
	for(i=0;i<size;i++)printf("%ld ",get(i));
}
/*void Inequality::fprint(FILE *f1){
	int i,j;
	fprintf(f1,"%d\n",size);
	for(i=0;i<size;i++){
		for(j=0;j<size;j++)fprintf(f1,"%lf ",get(i,j));
		fprintf(f1,"\n");	
	}	
}

void Inequality::invert(){
	int i,j;
	double value;
	FILE *temp;
	temp = fopen("origInequality.txt","w");	
	fprint(temp);
	fclose(temp);	
	system("./LA");
	temp = fopen("invInequality.txt","r");
	fscanf(temp, "%lf",&determinant);
	for(i=0;i<size;i++)for(j=0;j<size;j++){
		fscanf(temp, "%lf",&value);	
		set(i,j,value);
	}
	fclose(temp);
}

void Inequality::copy(Inequality * orig){
	int i,j;
	double value;
	determinant = orig->getDet();
	for(i=0;i<size;i++)for(j=0;j<size;j++){
		value=orig->get(i,j);	
		set(i,j,value);
	}
}*/
