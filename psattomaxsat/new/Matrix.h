#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<math.h>

class Matrix{
    private:
    int size;
    double * elements, determinant; 
        
    public:
    Matrix(int size);
    ~Matrix();  
    int getSize(){return size;}
    double getDet(){return determinant;}
    void updateDet();
    void setDet(double value){determinant = value;}
    void set(int row, int column, double value){elements[row*size+column]=value;}
    double get(int row, int column);
    void initialize();
    void print();
    void fprint(FILE *f1);
    void invert();
    void invertInplace();
    void copy(Matrix *orig);
};
