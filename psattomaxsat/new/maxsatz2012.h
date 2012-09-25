void setHARD_WEIGHT(long long int weight);
void setNB_VAR(int nvar);
int lire_clause(long int wght, int *lits, int length, int clause_number, int instance_type);
// int solve(int clauses, int var);
int maxsatz_main(int argc, char *argv[], int solution[], long long int *opt_solution, int *sat, int size);
int maxsatz_main_new(int clauses, int var, int solution[], long long int *opt_solution, int *sat, int size);