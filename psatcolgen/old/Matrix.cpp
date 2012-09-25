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


/*This class is used to represent matrices with "double" elements. 
The inverse operation is done by another program, "LA", written in 
regular C to expoit the CLAPACK library. The source-code "LA.c" 
might be in this same folder. */


#include<stdlib.h>
#include"Matrix.h"

Matrix::Matrix(int sz){
	size=sz;
	elements=new double[size*size];
}

Matrix::~Matrix(){
	delete [] elements;
}

double Matrix::get(int row, int column){
	return elements[row*size+column];
}

void Matrix::initialize(){
	int i,j;
	determinant=1;
	for(i=0;i<size;i++)for(j=0;j<size;j++)
		if(i>j)set(i,j,0);
		else set(i,j,1);
}

void Matrix::print(){
	int i,j;
	for(i=0;i<size;i++){
		for(j=0;j<size;j++)printf("%lf ",get(i,j));
		printf("\n");	
	}	
}
void Matrix::fprint(FILE *f1){
	int i,j;
	fprintf(f1,"%d\n",size);
	for(i=0;i<size;i++){
		for(j=0;j<size;j++)fprintf(f1,"%lf ",get(i,j));
		fprintf(f1,"\n");	
	}	
}

void Matrix::invert(){
	int i,j;
	double value;
	FILE *temp;
	temp = fopen("origMatrix.txt","w");	
	fprint(temp);
	fclose(temp);	
	system("./LA");
	temp = fopen("invMatrix.txt","r");
	fscanf(temp, "%lf",&determinant);
	for(i=0;i<size;i++)for(j=0;j<size;j++){
		fscanf(temp, "%lf",&value);	
		set(i,j,value);
	}
	fclose(temp);
}

void Matrix::copy(Matrix * orig){
	int i,j;
	double value;
	determinant = orig->getDet();
	for(i=0;i<size;i++)for(j=0;j<size;j++){
		value=orig->get(i,j);	
		set(i,j,value);
	}
}
