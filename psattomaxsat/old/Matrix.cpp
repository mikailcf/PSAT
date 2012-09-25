#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>
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
void Matrix::updateDet(){
	//double value;
	FILE *temp;
	temp = fopen("invMatrix.txt","r");
	fscanf(temp, "%lf",&determinant);
	determinant=round(determinant);
	fclose(temp);
}

void Matrix::invert(){
	int i,j;
	double value;
	FILE *temp;
	temp = fopen("origMatrix.txt","w");	
	fprint(temp);
	fclose(temp);	
	system("./LA2");
	temp = fopen("invMatrix.txt","r");
	fscanf(temp, "%lf",&determinant);
	determinant=1/determinant;
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
