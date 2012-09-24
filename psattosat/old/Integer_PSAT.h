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


/* Esta classe passa o problema PSAT P para o problema P' com inteiros, multiplicando o vetor pi por 10^precisao, onde a precisao é o numero de algarismos decimais após a virgula em pi, especificados no arquivo de entrada. P' tem solução sse P tem solução, e para recuperá-la, basta dividir a solução encontrada por 10^precisao*/

class Integer_PSAT{
	private:
	long *piNum; //pi numérico
	long **piBin; //pi em binário, com kbits bits
	long **pi; //literais referentes aos bits 
	FILE *gamma;
	//int dbits;
	int kbits;
	//int tbits;
	int nVar;
	int nProb;
	int nClause;
	
	public:
	int getnVar(){return nVar;}
	int getnProb(){return nProb;}
	int getnClause(){return nClause;}
	int getkbits(){return kbits;}
	FILE* getgamma(){return gamma;}
	
	Integer_PSAT(int nV, int nPr, int nCl, long* piN, int kbits, FILE* gam);
   ~Integer_PSAT(); 
	void solve();
	
};
