//#include<stdio.h>
#include<stdlib.h>
//#include<string.h>
#include<time.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>   // file I/O
//#include <cstdio>

#include "paramsMaxSat.h"

float * geradorPi(int n);

using namespace std;

void gerador(int m, int k, int n, ofstream &f1){
	int h, i, j, *literal,repetido, **count;	

    //cout << "comecei " << flush;
	count = new int*[n];
    literal = new int[k];
	for(i=0;i<n;i++)
		count[i]=new int[2];
	for(i=0;i<n;i++)
		for(j=0;j<2;j++)
			count[i][j]=0;
	for(i=0;i<m;i++){
		for(j=0;j<k;j++){
			do{
				repetido=0;				
				literal[j]= rand()%(2*n)-n;
				if(literal[j]==0)literal[j]=n;			
				for(h=0;h<j;h++)
					if(literal[j]==literal[h]||literal[j]==-literal[h])
						repetido=1;
			} while(repetido==1);						
			f1 << literal[j] << " ";
            //cout << literal[j] << " ";
			if(literal[j]>0)
				count[literal[j]-1][0]++;
			else 
				count[-literal[j]-1][1]++;
		}
		f1 << "0\n" << flush;
        //cout << "0\n" << flush;
	}
    //cout << "vou terminar " << endl;

    // Desaloca memória
    for(i=0;i<n;i++) {
      //cout << i << flush;
      delete[] count[i];
    }
    //cout << "quase terminando " << endl;
    delete[] count;
    //cout << "terminei " << endl;
}

void imprimeCabecalho(ofstream &f1, int n, int m, int k, int prec){
  f1 << "c pseudo-aleatorio\n"
     << "c\n"
     << "p pcnf " << n << " " << m << " " << k << " " << prec << "\n"
     << "c Atomic Probabilities\n" << flush;
}

void imprimePi(ofstream &f1, int n){

  float * pi = geradorPi(n);
  for(int i=0;i<n;i++)
    f1 << "a " << i+1 << " "
       << setiosflags(ios::fixed) << setprecision(2) << pi[i]
       << endl;
  delete[] pi; // desaloca a memória !!!!
  
}

float * geradorPi(int n){
	float * pi;
	float temp;
	pi = new float[n]; 
	for(int k=0; k<n;k++)
      pi[k]=(float)(rand()%(PREC+1))/PREC;//sorteia n probabilidades
	for(int i=n-1;i>0;i--) //ordena de forma decrescente
		for(int j=0; j<i; j++)
			if(pi[j]<pi[j+1]){
				temp=pi[j];
				pi[j]=pi[j+1];
				pi[j+1]=temp;
			}
	return pi;	
}

int main(){
  int /* **count,*/ j, /* **result, */sat, /*i,  temp, */ totalsat=0, totalunsat=0,k, kCl=K_CL, n/*=4*/, m,prec=2, qtde=N_TESTES;
	float totaltimeSAT=0,totaltimeUNSAT=0,tempomedSAT,tempomedUNSAT, satporcento, unsatporcento, tempomedio;
    //	char  lido[6], aux[6];
	//FILE *saida /*, *ftempo, *resultado */;
    stringstream resultFile;
    resultFile << "resultMaxSatN" << N << "K" << K << ".txt";
	//resultado = fopen(resultFile.str().c_str(),"w");
    ofstream ofsResult( resultFile.str().c_str(), ios::trunc );
    ofsResult << setiosflags(ios::right);
 
    stringstream ssComand;
    ssComand << "(time ./" 
             << PROGRAM 
             << " in.pcnf) 2> saidatm.out > /dev/null";

    //    cout << ssComand.str() << endl;
	srand ( time(NULL) );

    ofsResult << setw(4) << "n"
              << setw(4) << "m"
              << setw(4) << "k"
              << setw(7) << "NTests"
              << setw(4) << "SAT"
              << setw(6) << "UNSAT"
              << setw(8) << "timeSAT"
              << setw(10) << "timeUNSAT"
              << setw(10) << "avgTime\n" << flush;
 	n=N;	
	k = K; //for(k=0;k<=n;k++)
    for(m=0;m<=5*n;m+=n/10){
      //cout << "M = " << m << endl;
		totalsat=0;
		totalunsat=0;
		totaltimeSAT=0;		
		totaltimeUNSAT=0;
		for(j=0;j<qtde;j++){
          //cout << "J = " << j << endl;
          ofstream saida("in.pcnf", ios::out);
          imprimeCabecalho(saida, n, m, k, prec);
          imprimePi(saida,  k);
          // cout << j << "n" << n << "m" << m <<  endl;
          gerador(m,kCl,n,saida);//Gamma
          // cout << j << "n" << n << "m" << m <<  endl;
          saida << endl;
          // cout << "vai fechar" << flush;
          saida.close();
          // cout << " fechou" << endl;
          // cout << j << "n" << n << "m" << m <<  endl;
          //cout << ssComand.str() << endl;
          system( ssComand.str().c_str() );
          //cout << j << "n" << n << "m" << m <<  endl;
			
          string decision;
          ifstream ifsDecision( "saidatm.out", ios::in ); 
          sat=0;
          ifsDecision >> decision;
          // cout << decision << endl;
          if( decision == "SAT" )
            sat = 1;
          else if( decision != "UNSAT" ) {
            cerr << "Decision is neither SAT nor UNSAT, but " << decision << endl;
            //exit(1);
            j--;
            continue;
          }

          double time;
          ifsDecision >> time;
          //cout << "time: " << time << endl;

          if(sat == 0){
            totalunsat++;
            totaltimeUNSAT += time; //atof(aux);
          }
          else if (sat==1){
            totalsat++;
            totaltimeSAT+= time; //atof(aux);
          }
          // cout << "saida" << endl;
		}
        // cout << "preflush" << endl;
		satporcento =  (float) 100*totalsat/qtde;
        // cout << "satporcento =  " << satporcento << endl;
		unsatporcento = (float) 100*totalunsat/qtde;
        // cout << "unsatporcento =  " << unsatporcento << endl;
		tempomedio = (totaltimeSAT+totaltimeUNSAT)/(totalsat+totalunsat);
        //  cout << "tempomedio =  " << tempomedio << endl;
		tempomedSAT = totaltimeSAT/totalsat;
        // cout << "tempomedSAT =  " << tempomedSAT << endl;
		tempomedUNSAT = totaltimeUNSAT/totalunsat;
        // cout << "tempomedUNSAT =  " << tempomedUNSAT << endl;
        // cout << "preflush0" << endl;
        // cout << (!ofsResult.bad() ? "Tudo bem" : "nada bem") << endl;
        // cout << ( ofsResult.fail() ? "fail" : "succeed" ) << endl;
        
        //ofsResult << setiosflags(ios::right);
        ofsResult << setw(4) << n 
                  << setw(4) << m 
                  << setw(4) << k 
                  << setw(7) << qtde 
                  << setiosflags(ios::fixed) << setprecision(0) 
                  << setw(4) << satporcento 
                  << setw(6) << unsatporcento 
                  << setiosflags(ios::fixed) << setprecision(3) 
                  << setw(8) << tempomedSAT                   
                  << setw(10) << tempomedUNSAT 
                  << setw(10) << tempomedio << endl;
        //cout << (!ofsResult.bad() ? "Tudo bem" : "nada bem") << endl;

        //cout << "flush" << endl;

       }	
       // cout << "vai fechar" << endl;
       ofsResult.close();
       // cout << "fechou" << endl;
       return 0;
}

