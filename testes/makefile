all:
	g++ testador.cpp -o test
	g++ torturador9.cpp -o tort
solvers:
	cd ..; cd psatcolgen; cd new; make
	cd ..; cd psatcolgen; cd old; make
	cd ..; cd psattosat; cd new; make
	cd ..; cd psattosat; cd old; make
	cd ..; cd psattomaxsat; cd new; make
	cd ..; cd psattomaxsat; cd old; make
cp:
	cp ../psatcolgen/new/PsatColGen ./PsatColGen
	cp ../psatcolgen/old/PsatColGen ./PsatColGen_old
	cp ../psatcolgen/old/LA ./LA
	cp ../psattosat/new/PSATtoSAT ./PSATtoSAT
	cp ../psattosat/old/PSATtoSAT ./PSATtoSAT_old
	cp ../psattosat/old/zchaff ./zchaff
	cp ../psattomaxsat/new/PSATtoMaxSat ./PSATtoMaxSat
	cp ../psattomaxsat/old/PSATtoMaxSat ./PSATtoMaxSat_old
	cp ../psattomaxsat/old/LA2 ./LA2
	cp ../psattomaxsat/old/maxsat ./maxsat
