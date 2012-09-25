#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Input_Reading.h"
//||

Input_Reading::Input_Reading(FILE *in){
	input=in;
}

Input_Reading::~Input_Reading(){
	fclose(input);
}

PSAT* Input_Reading::createPSAT(){
  int i=1, j=2, /* k, */ nVar, nClause, nProb, aux/* ,lixo */, precision;
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
	if(!strcmp(" pcnf ",temp)){printf("input out of pcnf standard format"); exit(-1);}
	fscanf(input,"%d", &nVar);
	fscanf(input,"%d", &nClause);	
	fscanf(input,"%d", &nProb);
	fscanf(input,"%d", &precision);
	if(nProb>0){
		probs = (double *) malloc(nProb * sizeof(double));
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
	fprintf(gamma,"%d %d\n",nVar,nClause);
	for(j=0;j<nClause;j++){     		//guarda gamma em um txt
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
	return ProbSat;
}
