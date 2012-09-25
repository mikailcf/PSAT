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


#include<stdio.h>

class Matrix{
	private:
	int size;
	double * elements, determinant; 
		
	public:
	Matrix(int size);
	~Matrix();	
	int getSize(){return size;}
	double getDet(){return determinant;}
	void setDet(double value){determinant = value;}
	void set(int row, int column, double value){elements[row*size+column]=value;}
	double get(int row, int column);
	void initialize();
	void print();
	void fprint(FILE *f1);
	void invert();	
	void copy(Matrix *orig);
};
