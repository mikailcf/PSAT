#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>

#define PREC 100 
int lixo;
int fatorial(int n){
	int fat=1;	
	if(n==1) return 1;
	if(n==0) return 1;
	for(int i=2;i<=n;i++)fat*=i;
	return fat;
}

int combinacao(int k,int j){
	if(k<j){
		printf("erro!");return 0;
	}	
	return fatorial(k)/(fatorial(j)*fatorial(k-j));
}

float * geradorPi(int n){
	float * pi;
	float temp;
	pi = new float[n]; 
	for(int k=0; k<n;k++)pi[k]=(float)(rand()%(PREC+1))/PREC;
	for(int i=n-1;i>0;i--)
		for(int j=0; j<i; j++)
			if(pi[j]<pi[j+1]){
				temp=pi[j];
				pi[j]=pi[j+1];
				pi[j+1]=temp;
			}
	return pi;	
}

float * geradorXi(int n){
	float * pi, *xi;
	xi = new float[n+1];
	pi = geradorPi(n+1);
	xi[n]=1-pi[0];
	for(int k=n-1;k>0;k--)xi[k]=pi[n-k-1]-pi[n-k];
	xi[0]=pi[n-1];	
//	for(int l=0; l<n+1; l++)printf("%.2f\n",pi[l]);
//	printf("\n");
	return xi;
}

int * geradorMatriz(int n){
	int * A;
	A = new int[(n+1)*(n+1)];
	for(int i=0;i<=n;i++)A[i]=1;
	for(int j=n+1;j<(n+1)*(n+1);j++)A[j]=rand()%2;
	return A;
}
void trocaLinhas(int *A, int a, int b, int n){
	int temp,j;
	for(j=0;j<n+1;j++){
		temp=A[a*(n+1)+j];
		A[a*(n+1)+j]=A[b*(n+1)+j];
		A[b*(n+1)+j]=temp;
	}
}
float * calculaPi(int * A, float * xi, int n){
	float* pi, temp;
	pi = new float[n];
	for(int k=1;k<n+1;k++)
		for(int l=0; l<n+1;l++)pi[k-1]+=A[l+k*(n+1)]*xi[l];
	for(int i=n-1;i>0;i--)
		for(int j=0; j<i; j++)
			if(pi[j]<pi[j+1]){
				temp=pi[j];
				pi[j]=pi[j+1];
				pi[j+1]=temp;
				trocaLinhas(A,j+1,j+2,n);
			}	
	return pi; 
}

void imprimeMatriz(int * A, int n){
	for(int i=0;i<n+1;i++){
		for(int j=0;j<n+1;j++)printf("%d ",A[j+i*(n+1)]);
		printf("\n");
	}

}

void imprimePi(float *pi, int n){
	for(int i=0; i<n;i++)printf("%.2f ",pi[i]);
	printf("\n");
}

void imprimeXi(float *xi, int n){
	for(int i=0; i<n+1;i++)printf("%.2f ",xi[i]);
	printf("\n");
}

int * converteMatriz(int *A, int n){
	int * B;
	B= new int[(n+1)*(n+1)];
	for(int i=1;i<(n+1);i++)
		for(int j=0;j<n+1;j++)
			if(A[i*(n+1)+j]==1)B[i*(n+1)+j]=i;
			else B[i*(n+1)+j]= -i;
	return B;
}


int* copiaLista(int * lista,int n){
	int *nova;
	nova= new int[n];
	for(int i=0;i<n;i++)nova[i]=lista[i];
	return nova;
}

void imprimeGammaSAT(int *A, int n, FILE *f1, int *mask){ // numero linear de variaveis!!!!!!!
	int nova, a, *B,b, v, cont=0;
	B=converteMatriz(A,n);
	for(v=0;v<n+1;v++){
		nova=n+1+v;
		if(mask[cont++]){
			fprintf(f1,"%d ", nova);
			for(int i=1;i<=n;i++)fprintf(f1,"%d ",-B[v+i*(n+1)]);
			fprintf(f1,"0\n");
		}
		for(int k=1;k<=n;k++)if(mask[cont++])fprintf(f1,"%d %d 0\n",-nova,B[v+k*(n+1)]);		
	}
	if(mask[cont++]){
		for(int j=0;j<n+1;j++)fprintf(f1,"%d ",n+j+1);
		fprintf(f1,"0");
	}
}

void imprimeGammaUNSAT(int *clause,int n, int nLit, int jmax, int * Cf,FILE * f1 ){	
	if(jmax==0){
		for(int j=0;clause[j]!=0;j++)fprintf(f1,"%d ",clause[j]);
		fprintf(f1,"0\n");
		return ;
	}	
	for(int i=nLit-1;i>=jmax-1;i--){//i=2 e i=1
		clause[jmax-1]=-Cf[i];
		//printf("i:%d, jmax:%d\n",i,jmax);
		//for(int h=0;clause[h]!=0;h++)printf("%d ",clause[h]);
		//printf("\n");
		imprimeGammaUNSAT(copiaLista(clause,n+1),n, i, jmax-1, Cf, f1);
	}
}

int y(int a, int h, int k, int j, int nvar){//nvar: numero de variaveis antes do inicio da formacao da ¬Cj; k=nLit
	int y=(a-1)*(k-j+1)+h;
	return 2*y-1+nvar;
}

int z(int a, int h, int k, int j, int nvar, int *Cf){//nvar: numero de variaveis antes do inicio da formacao da ¬Cj; k=nLit
	if(a==0)return -Cf[h-1];
	int z=(a-1)*(k-j+1)+h;
	return 2*z+nvar;
}

void imprimeGammaUNSATpoly(int nvar, int k, int j, int * Cf,FILE * f1){//k=nLit e j=jmax
	//printf("\njmax: %d k:%d\n",j,k);
	//printf("literais da C-formula: ");
	//for(int temp=0;temp<k;temp++)printf("%d ",Cf[temp]);
	//printf("\n");
	for(int a=1;a<=j-1;a++){
		for(int h=1;h<=k-j+1;h++){
			fprintf(f1,"%d ",y(a,h,k,j,nvar));
			for(int g=h-1+a;g>=a;g--)fprintf(f1,"%d ",-z(a-1,g+1-a,k,j,nvar,Cf));
			fprintf(f1,"0\n"); //clausula grande do sse terminada
			for(int g2=h-1+a;g2>=a;g2--)
				fprintf(f1,"%d %d 0\n",-y(a,h,k,j,nvar),z(a-1,g2+1-a,k,j,nvar,Cf));//2-clausulas do sse
		}
		for(int h2=1;h2<=k-j+1;h2++){
			fprintf(f1,"%d %d %d 0\n",-z(a,h2,k,j,nvar,Cf),-Cf[h2+a-1],y(a,h2,k,j,nvar));
			fprintf(f1,"%d %d 0\n",z(a,h2,k,j,nvar,Cf),+Cf[h2+a-1]);
			fprintf(f1,"%d %d 0\n",z(a,h2,k,j,nvar,Cf),-y(a,h2,k,j,nvar));
		}
	}
	//impressao do ultimo y comecando
	fprintf(f1,"%d ",y(j,1,k,j,nvar));
	for(int g3=k;g3>=j;g3--)fprintf(f1,"%d ",-z(j-1,g3+1-j,k,j,nvar,Cf));
	fprintf(f1,"0\n"); //clausula grande do sse terminada
	for(int g4=k;g4>=j;g4--)
		fprintf(f1,"%d %d 0\n",-y(j,1,k,j,nvar),z(j-1,g4+1-j,k,j,nvar,Cf));//2-clausulas do sse	
	fprintf(f1,"%d 0\n",y(j,1,k,j,nvar));//clausula da formula toda
	
}

void imprimeCabecalhoSAT(FILE * f1, int n, int nclause){
	int precisao=(int)log10(PREC);
	fprintf(f1,"c pseudo-aleatorio SATISFAZIVEL\n");
	fprintf(f1,"c\n");
	fprintf(f1,"p pcnf %d %d %d %d\n",2*n+1,nclause,n,precisao);
	fprintf(f1,"c Atomic Probabilities\n");
}
void imprimeCabecalhoUNSAT(FILE * f1, int n, int nLit, int jmax){
	int precisao=(int)log10(PREC);
	fprintf(f1,"c pseudo-aleatorio INSATISFAZIVEL\n");
	fprintf(f1,"c\n");
	fprintf(f1,"p pcnf %d %d %d %d\n",n,combinacao(nLit,jmax),n,precisao);
	fprintf(f1,"c Atomic Probabilities\n");
}

void imprimeCabecalhoUNSATpoly(FILE * f1, int n, int nLit, int jmax){
	int precisao=(int)log10(PREC);
	int nclause, nvar;
	nvar=n+1+2*(jmax-1)*(nLit-jmax+1);
	nclause=1+3*(jmax-1)*(nLit-jmax+1)+(jmax-1)*(nLit-jmax+1+(nLit-jmax+2)*(nLit-jmax+1)/2)+nLit-jmax+2;
	fprintf(f1,"c pseudo-aleatorio INSATISFAZIVEL\n");
	fprintf(f1,"c\n");
	fprintf(f1,"p pcnf %d %d %d %d\n",nvar,nclause,n,precisao);
	fprintf(f1,"c Atomic Probabilities\n");
}

void imprimePi(FILE *f1, float * pi, int n){
	for(int i=0;i<n;i++)fprintf(f1,"a %d %.2f \n",i+1,pi[i]);
}

void geraSAT(char *arq, int n){
	float *xi, *pi;
	int *mask, nclause=0, Perc=100;//porcetagem de chance de uma clausula entrar
	mask=new int[(n+1)*(n+1)+1];
	for(int i=0;i<(n+1)*(n+1)+1;i++)
		if(rand()%100<Perc){
			mask[i]=1;
			nclause++;
		}
		else mask[i]=0;
	FILE *f1;
	f1=fopen(arq,"w");
	int *A, *B;
	A=geradorMatriz(n);
	xi = geradorXi(n);
	pi = calculaPi(A, xi, n);
	imprimeCabecalhoSAT(f1,n,nclause);
	imprimePi(f1,pi,n);
	imprimeGammaSAT(A,n,f1, mask);
	//B=converteMatriz(A,n);	
	//imprimeMatriz(B,n);	

	fclose(f1);
}



void geraUNSAT(char *arq, int n){
	float * pi, jmax=0;
	int * Cformula,nLit=n, k=0;
	int *clause;
	pi = geradorPi(n);
	Cformula = new int[n];
	do{
		nLit=n;
		for(int i=0; i<n; i++){
			Cformula[i]=(i+1)*((rand()%3)-1);		
			if(Cformula[i]>0)jmax+= pi[i];
			else if(Cformula[i]<0) jmax+=1-pi[i];
			else nLit--;
		}	
	}while(nLit==0);
	clause= new int[n+1];
	for(int h=0; h<nLit+1;h++)clause[h]=0;	
	jmax= ceil(jmax);
	jmax=(int)jmax;
	for(int j=0; j<nLit;j++){
		while(Cformula[k]==0)k++;
		Cformula[j]=Cformula[k];
		k++;	
	}
	FILE *f1;
	f1=fopen(arq,"w");
//	imprimeCabecalhoUNSAT(f1,n, nLit, jmax);
	imprimeCabecalhoUNSATpoly(f1,n, nLit, jmax);
	imprimePi(f1,pi,n);
//	imprimeGammaUNSAT(clause,n,nLit,jmax,Cformula,f1);
	imprimeGammaUNSATpoly(n,nLit,jmax,Cformula,f1);
	fclose(f1);
}

void testa(int minProb, int maxProb, int qtdeSAT, int qtdeUNSAT){
	int i,j, k, sat;
	char aux[100];	
	FILE *saida, *tabelado, *instancia;
	
	tabelado = fopen("resultadosSAT.txt", "w");
	fprintf(tabelado, "nProb, instance, SAT \n");	
	for(i=minProb;i<=maxProb;i++)
		for(j=1;j<=qtdeSAT;j++){
		instancia = fopen("inst.txt","w");
		fprintf(instancia,"último arquivo de entrada: SATISn%d_%d.pcnf",i,j);
		fclose(instancia);		
		sprintf(aux,"./PsatColGen instances/SATISn%d_%d.pcnf | tee saidaPSAT.out",i,j);
		system(aux);
		saida=fopen("saidaPSAT.out","r");		
		while(!feof(saida)&&(aux[0]!= '*' || aux[1]!='*' || aux[2]!='*'))
			fscanf(saida,"%3c",aux);
		if(feof(saida))sat=0;
		else sat=1;		
		fclose(saida);
		fprintf(tabelado,"\n%7d, %7d, %7d",i,j,sat);
	}
	fclose(tabelado);

	tabelado = fopen("resultadosUNSAT.txt", "w");
	fprintf(tabelado, "nProb, instance, SAT \n");	
	for(i=minProb;i<=maxProb;i++)
		for(j=1;j<=qtdeUNSAT;j++){
		instancia = fopen("inst.txt","w");
		fprintf(instancia,"último arquivo de entrada: UNSATn%d_%d.pcnf",i,j);
		fclose(instancia);		
		sprintf(aux,"./PsatColGen instances/UNSATn%d_%d.pcnf | tee saidaPSAT.out",i,j);
		system(aux);
		saida=fopen("saidaPSAT.out","r");		
		while(!feof(saida)&&(aux[0]!= '*' || aux[1]!='*' || aux[2]!='*'))
			fscanf(saida,"%3c",aux);
		if(feof(saida))sat=0;
		else sat=1;		
		fclose(saida);
		fprintf(tabelado,"\n%7d, %7d, %7d",i,j,sat);
	}
	fclose(tabelado);
}

int main(){
	int n1,n2,qtdeSAT,qtdeUNSAT;
	char *arq=new char[50];
	// arq = new char[35]; 	
	printf("numero minimo de variveis em PSI:");
	scanf("%d",&n1);
	printf("numero maximo de variveis em PSI:");
	scanf("%d",&n2);
	printf("Qtde de instancias satisfaziveis, para cada numero de variaveis:");	
	scanf("%d",&qtdeSAT);
	printf("Qtde de instancias insatisfaziveis, para cada numero de variaveis:");
	scanf("%d",&qtdeUNSAT);
	srand ( time(NULL) );	
	for(int i=n1;i<=n2;i++)for(int j=1;j<=qtdeSAT;j++){
		sprintf(arq,"instances/SATISn%d_%d.pcnf",i,j);
		geraSAT(arq,i);
	}
	for(int k=n1;k<=n2;k++)for(int l=1;l<=qtdeUNSAT;l++){
		sprintf(arq,"instances/UNSATn%d_%d.pcnf",k,l);
		geraUNSAT(arq,k);
	}	
	// testa(n1,n2,qtdeSAT,qtdeUNSAT);
	return 0;
		
}
