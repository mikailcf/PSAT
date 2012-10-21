#include <iostream>
#include <fstream>
#include <string>

using namespace std;

float getTime(fstream &file){
    char c;
    string trash;
    float time;

    c = file.peek();

    while(c != 'r'){
        while(c != '\n') c = file.get();
        c = file.peek();
    }

    file >> trash;
    file >> time;

    return time;
}

int sat(){
    char c;
    FILE *sat = fopen("sat", "r");

    c = getc(sat);
    fclose(sat);
    if(c == 'S') return 1;
    else return 0;
}

int main(){
    int n_min, n_max, n_sat, n_unsat, n_solver, sat_only, k = 0;
    int **sat_res;
    char *in = new char[50];
    char *cmd = new char[100];
    char *out = new char[100];
    char *solver_name = new char[100];
    float time_sat, time_unsat;
    fstream time_file;
    FILE *result_file;

    cout << "Somente SAT: ";
    cin >> sat_only;

    cout << "Numero minimo de variaveis: ";
    cin >> n_min;

    cout << "Numero maximo de variaveis: ";
    cin >> n_max;

    cout << "Numero de instancias SAT: ";
    cin >> n_sat;

    cout << "Numero de instancias UNSAT: ";
    cin >> n_unsat;

    cout << "Numero de solvers: ";
    cin >> n_solver;

    if(sat_only){
        sat_res = (int**) malloc((n_max - n_min)*(sizeof(int*)));

        for(int i = 0; i < n_max - n_min; i++){
            sat_res[i] = (int*) malloc(n_sat*(sizeof(int)));

            for(int j = 0; j < n_max - n_min; j++)
                sat_res[i][j] = 0;
        }
    }

    for(int n = 0; n < n_solver; n++, k++, k %= 2){
        printf("Nome %d: ", n);
        scanf("%s", solver_name);
        sprintf(out, "%s.result", solver_name);

        result_file = fopen(out, "w");
        fprintf(result_file, "  n   k n_sat time_sat\n");

        for(int i = n_min; i <= n_max; i++){
            time_sat = time_unsat = 0;

            for(int j = 0; j < n_sat; j++){
                time_file.open("time.out", fstream::in);

                sprintf(in, "instances/SATISn%d_%d.pcnf", i, j+1);
                sprintf(cmd, "(time -p ./%s %s > /dev/null 2> sat) 2> time.out", solver_name, in);
                // (/usr/bin/time -p ./PSATtoMaxSat instances/SATISn25_1.pcnf > /dev/null 2> sat) > time.out
                
                system(cmd);

                if(sat_only){
                    if(sat()){
                        printf("%d %d\n", i, j);
                        if(k == 0){
                            sat_res[i - n_min][j] = 1;

                            time_sat += getTime(time_file);
                            time_file.close();
                        }
                        else if(sat_res[i - n_min][j] == 1){
                            time_sat += getTime(time_file);
                            time_file.close();
                        }
                        else n_sat--;
                    }
                    else n_sat--;       
                }
                else{
                    time_sat += getTime(time_file);
                    time_file.close();
                }

            }
            if(n_sat != 0) time_sat /= n_sat;

            fprintf(result_file, "%3d %3d %5d %8.3f\n", (2*i)+1, i, n_sat, time_sat);

            // for(int j = 0; j < n_unsat; j++){
            //     time_file.open("time.out", fstream::in);

            //     sprintf(in, "instances/UNSATISn%d_%d.pcnf", i, j+1);
            //     sprintf(cmd, "/usr/bin/time -p ./%s %s 2> /dev/null > time.out", solver_name, in);
            //     system(cmd);

            //     time_file.close();
            //     time_unsat += getTime(time_file);
            // }
        }
        fclose(result_file);
    }
}