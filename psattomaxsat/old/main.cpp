#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>
#include"Input_Reading.h"
//||

int main(int argc, char** argv){
  //int i;
	PSAT *pPSAT;
	Input_Reading *Reading;
	FILE * input;
	if(argc<1){
		printf("Specify the input file name!\n");
		printf("psatSolver <FILENAME>");
        exit(1);
	}
	printf("%s",argv[1]);
	input = fopen(argv[1],"r");
	Reading = new Input_Reading(input);
	pPSAT = Reading->createPSAT();
	pPSAT->solve();
	printf("\n");
	return 0;
}
