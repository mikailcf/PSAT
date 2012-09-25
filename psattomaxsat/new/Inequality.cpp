#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>
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
void Inequality::fprint(FILE *f1){
	int i;
	fprintf(f1,"\n");
	for(i=0;i<size;i++)fprintf(f1,"%ld ",get(i));	
}
/*
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
