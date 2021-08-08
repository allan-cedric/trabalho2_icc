#include "interpol.h"
#include <likwid.h>

int main()
{
    LIKWID_MARKER_INIT;

    int n; // Número de valores tabelados
    int m; // Número de funções tabeladas
    scanf("%i %i", &n, &m);

    real_t *x = (real_t *)malloc(n * sizeof(real_t)); // Aloca o vetor de valores tabelados
    alloc_test(x);
    real_t *y = (real_t *)malloc(n * sizeof(real_t)); // Aloca o vetor dos valores de uma função
    alloc_test(y);

    // Leitura dos valores tabelados
    for (int i = 0; i < n; i++)
        scanf("%lf", &x[i]);

    lin_system_t *ls_interpol = alloc_lin_system(n); // Aloca o sistema linear para interpolação
    gen_poly_interpol_coef(x, ls_interpol);          // Gera a matriz de coeficientes para interpolação

    for (int j = 0; j < m; j++)
    {
        // Leitura de uma função nos valores tabelados
        for (int i = 0; i < n; i++)
            scanf("%lf", &y[i]);

        // --- Interpolação polinomial ---

        // --- Ajuste de curvas ---
    }

    LIKWID_MARKER_CLOSE;
    return 0;
}