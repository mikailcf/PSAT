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


#include"Input_Reading.h"


int main(int argc, char ** argv){
  //int i;
	PSAT *ProbSat;
	FILE * input;
	input = fopen(argv[1],"r");
	Input_Reading Reading(input);
	ProbSat = Reading.createPSAT();
	ProbSat->solve();
	printf("\n");
    //fclose( input );
    delete ProbSat;
	return 0;
}
