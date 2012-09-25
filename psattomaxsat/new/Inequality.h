#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*
An inequality a_0 + a_1 x_1+...+a_k x_k >= 0, with integer coeficients,
 is represented by a
long array with coeficients a_0...a_k, where a_0 is the independent term. 

*/
class Inequality{
	private:
	int size;
	long * coeficients; 
		
	public:
	Inequality(int size);
	~Inequality();	
	int getSize(){return size;}
	void set(int index, long value){coeficients[index]=value;}
	long get(int index);
	//void initialize();
	void print();
	void fprint(FILE *f1);
	//void invert();	
	//void copy(Matrix *orig);
};
