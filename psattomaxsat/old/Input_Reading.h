#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>
#include"PSAT.h"
//||

class Input_Reading{
	public:	
	Input_Reading(FILE * in);
	~Input_Reading();
	PSAT* createPSAT();

	private:
	FILE *input;
};
