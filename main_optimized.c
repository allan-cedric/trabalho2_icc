// Source file: 'main_unoptimized.c'
// Autores:
// Allan Cedric G. B. Alves da Silva - GRR20190351
// Gabriel N. Hishida do Nascimento - GRR20190361

// Versão otimizada para análise de desempenho

#include "interpol.h"
#include <likwid.h>

int main()
{
    LIKWID_MARKER_INIT;

    int ret;
    int n; // Número de valores tabelados
    int m; // Número de funções tabeladas
    ret = scanf("%i %i", &n, &m);

    real_t *x = (real_t *)malloc(n * sizeof(real_t)); // Aloca o vetor de valores tabelados
    alloc_test(x);
    real_t *y = (real_t *)malloc(n * sizeof(real_t)); // Aloca o vetor dos valores de uma função
    alloc_test(y);

    // Leitura dos valores tabelados
    for (int i = 0; i < n; i++)
        ret = scanf("%lf", &x[i]);

    lin_system_t *ls_interpol = alloc_lin_system(n); // Aloca o sistema linear para interpolação
    lin_system_t *ls_curve = alloc_lin_system(n);    // Aloca o sistema linear para "ajuste de curva"

    gen_poly_interpol_matcoef(x, ls_interpol); // Gera a matriz de coeficientes para interpolação

    LIKWID_MARKER_START("Aj-MatCoef");
    gen_curve_matcoef(x, ls_curve); // Gera a matriz de coeficientes para "ajuste de curva"
    LIKWID_MARKER_STOP("Aj-MatCoef");

    // --- Decomposição LU das matrizes de coeficientes ---
    double rtime;
    LIKWID_MARKER_START("It-LU-Optimized");
    LU_decomp_optimized(ls_interpol);
    LIKWID_MARKER_STOP("It-LU-Optimized");

    LIKWID_MARKER_START("Aj-LU-Optimized");
    LU_decomp_optimized(ls_curve);
    LIKWID_MARKER_STOP("Aj-LU-Optimized");

    for (int j = 0; j < m; j++)
    {
        // Leitura de uma função nos valores tabelados
        for (int i = 0; i < n; i++)
            ret = scanf("%lf", &y[i]);

        // --- Interpolação polinomial ---
        memcpy(ls_interpol->b, y, ls_interpol->n * sizeof(real_t));

        real_t *solution = solve_lin_system(ls_interpol);
        if (solution)
            print_real_array(solution, ls_interpol->n);

        // fprintf(stderr, "L2: %.17g\n", L2_norm(&ls_interpol->A, solution, ls_interpol->b));

        free(solution);
        solution = NULL;

        // --- Ajuste de curvas ---
        gen_curve_matb(x, y, ls_curve);

        solution = solve_lin_system(ls_curve);
        if (solution)
            print_real_array(solution, ls_curve->n);

        // fprintf(stderr, "L2: %.17g\n", L2_norm(&ls_curve->A, solution, ls_curve->b));

        free(solution);
        solution = NULL;
    }

    free(x);
    x = NULL;
    free(y);
    y = NULL;

    free_lin_system(ls_interpol);
    free_lin_system(ls_curve);

    LIKWID_MARKER_CLOSE;
    return 0;
}