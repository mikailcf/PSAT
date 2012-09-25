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
	void print();
};
