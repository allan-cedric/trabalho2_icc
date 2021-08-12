#include <stdio.h>
#include <stdlib.h>

#define STEP 0.01
#define TICS 5

double pol(double x, double *coef, int n) {

	double sum = 0.0;
	double pot = 1;
	for (int i = 0; i < n; ++i) {
		sum += coef[i]*pot;
		pot *= x;
	}
	return sum;
}

int main(int argc, char **argv) {

	FILE *in = fopen(argv[1], "r");
	FILE *out = fopen(argv[2], "r");

	int n, m;
	fscanf(in, "%d %d",&n, &m);

	double X[n];
	double F[m][n];

	for (int i = 0; i < n; ++i)
		fscanf(in, "%lf", &(X[i]));

	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j)
			fscanf(in, "%lf", &(F[i][j]));

	fclose(in);

	double P_inter[m][n];
	double P_ajust[m][n];

	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j)
			fscanf(out, "%lf", &(P_inter[i][j]));
		for (int j = 0; j < n; ++j){
			fscanf(out, "%lf", &(P_ajust[i][j]));
			//P_ajust[i][j] += (double) rand() / RAND_MAX; // "estragando" o polinomio do ajuste para ver a diferença
		}
	}

	fclose(out);


	printf("#!/usr/bin/gnuplot -c\n");
	printf("set encoding utf\n");
	printf("set terminal qt persist\n");
	printf("set style function dots\n");
	printf("set datafile separator ','\n");
	printf("set grid xtics ytics mxtics mytics\n");
	printf("set mxtics %d\n",TICS);
	printf("set mytics %d\n",TICS);

	for (int i = 0; i < m; ++i) {
		char name[8] = {'f'};
		sprintf(name+1, "%d.csv", i);
		FILE *f = fopen(name, "w+");
		
		for (int j = 0; j < n; ++j) {
			double   x = X[j];
			double  fx = F[i][j];
			double pix = pol(x, P_inter[i], n);
			double pax = pol(x, P_ajust[i], n);

			fprintf(f, "%.20lg, %.20lg, %.20lg, %.20lg, , \n", x, fx, pix, pax);
		}
		for (double x = X[0]; x <= X[n-1]; x += STEP) {
			double pix = pol(x, P_inter[i], n);
			double pax = pol(x, P_ajust[i], n);

			fprintf(f, "%.20lg, , , , %.20lg, %.20lg\n", x, pix, pax);
		}

		printf("set terminal qt %d title \"f%d\"\n",i,i+1);
		printf("plot '%s' using 1:2 with points title \"f%d(x)\", '' using 1:5 with line title \"pi%d\", '' using 1:6 with line title \"pa%d\"\n", name,i+1,i+1,i+1);
		// printf("plot '%s' using 1:2 with points title \"f%d(x)\", '' using 1:3 with points title \"pi%d(x)\", '' using 1:4 with points title \"pa%d(x)\", '' using 1:5 with line title \"pi%d\", '' using 1:6 with line title \"pa%d\"\n", name,i+1,i+1,i+1,i+1,i+1);
		printf("show plot\n");
		printf("pause mouse\n\n");

		fclose(f);
	}

}
