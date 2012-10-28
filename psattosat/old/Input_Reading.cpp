/*
    Copyright (C) 2011 Glauber De Bona

    This file is part of PSATtoSAT.

    PSATtoSAT is free software: you can redistribute it and/or modify
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


/*This class handles the input file, generating an PSAT object with the instance that wa read.*/

#include<stdlib.h>
#include <string.h>
#include"Input_Reading.h"

Input_Reading::Input_Reading(FILE *in){
	input=in;
}

Input_Reading::~Input_Reading(){
	fclose(input);
}

//this function returns a pointer to a PSAT object corresponding to the instace described in the input file.
PSAT* Input_Reading::createPSAT(){
  int i=1, j=2, /*k,*/ nVar, nClause, nProb, aux,/*lixo,*/ precision;
	long lit;
	double *probs;
	char lido, temp[10]/*, temp2[10]*/;
	PSAT* ProbSat;
	FILE *gamma;
	lido =fgetc(input);
	while(lido=='c'){
		while(fgetc(input)!='\n');
		lido = fgetc(input);
	}
	if(lido!='p'){printf("input out of pcnf standard format"); exit(-1);}
	fgets(temp,6,input);
	if(!strcmp(" pcnf ",temp)){
      printf("input out of pcnf standard format"); 
      exit(-1);
    }
	fscanf(input,"%d", &nVar);
	fscanf(input,"%d", &nClause);	
	fscanf(input,"%d", &nProb);
	fscanf(input,"%d", &precision);
    precision = 6;

	if(nProb>0){
      //		probs = (double *) malloc(nProb * sizeof(double));
      probs = new double[nProb];
		lido=fgetc(input);
		while(lido=='\n')lido=fgetc(input);	
		while(lido=='c'){
			while(fgetc(input)!='\n');
			lido = fgetc(input);
		}
		while(lido=='\n')lido=fgetc(input);
		for(i=0;i<nProb;i++){
			if(lido!='a'){printf("input out of pcnf standard format"); exit(-1);}
			fscanf(input,"%d", &aux);
			fscanf(input,"%lf", &probs[aux-1]);
			while(lido!='\n')lido=fgetc(input);
			if(i!=nProb-1)lido = fgetc(input);
		}
	}
	else {
		probs = NULL;
		lido=fgetc(input);
		while(lido=='\n')lido=fgetc(input);
	}
	while(lido!='\n')lido=fgetc(input);
	gamma=fopen("gamma.txt", "w");
	// clauses of Gamma are kept in a ".txt" file, for future use.
	for(j=0;j<nClause;j++){
		fscanf(input,"%ld",&lit);
		while(lit!=0){
			fprintf(gamma,"%ld ",lit);
			fscanf(input,"%ld",&lit);
		}
        	fprintf(gamma,"0\n");
        }		
	rewind(input);
	ProbSat = new PSAT(nVar, nProb, nClause, probs, precision, gamma);
	fclose(gamma);	
    delete[] probs;
	return ProbSat;
}
