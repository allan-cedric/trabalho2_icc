#include "lin_system.h"

lin_system_t *alloc_lin_system(int n)
{
    lin_system_t *ls = (lin_system_t *)malloc(sizeof(lin_system_t));
    alloc_test(ls);

    ls->n = n;

    ls->A.n = n;
    ls->A.A = (real_t *)malloc(ls->n * ls->n * sizeof(real_t));
    alloc_test(ls->A.A);

    ls->L.n = n;
    ls->L.A = NULL;

    ls->U.n = n;
    ls->U.A = NULL;

    ls->P = (int *)malloc(ls->n * sizeof(int));
    alloc_test(ls->P);

    ls->b = (real_t *)malloc(ls->n * sizeof(real_t));
    alloc_test(ls->b);

    return ls;
}

void free_lin_system(lin_system_t *ls)
{
    if (ls)
    {
        free_matrix(&ls->A);
        free_matrix(&ls->L);
        free_matrix(&ls->U);
        if (ls->P)
        {
            free(ls->P);
            ls->P = NULL;
        }
        if (ls->b)
        {
            free(ls->b);
            ls->b = NULL;
        }
        free(ls);
        ls = NULL;
    }
}

real_t *solve_lin_system(lin_system_t *ls)
{
    if (!ls->L.A || !ls->U.A)
    {
        fprintf(stderr, "Error: There is no LU decomposition!\n");
        return NULL;
    }

    real_t *x = (real_t *)malloc(ls->n * sizeof(real_t));
    alloc_test(x);

    real_t *y = (real_t *)malloc(ls->n * sizeof(real_t));
    alloc_test(y);

    // Troca as linhas do vetor identidade, caso tenha pivoteamento
    for (int i = 0; i < ls->n - 1; i++)
    {
        if (ls->P[i] != i) // Troca se as linhas do par forem distintas
            swap_items(&ls->b[i], &ls->b[ls->P[i]], sizeof(real_t));
    }

    // ------------------------------------
    // Resolução do sistema linear
    // ------------------------------------

    // Retrossubstituicao L x y = ls->b
    for (int i = 0; i < ls->n; i++)
    {
        y[i] = ls->b[i];
        int line = i * ls->n;
        for (int j = 0; j < i; j++)
            // y[i] -= L(i, j) * y[j]
            y[i] -= (ls->L.A[line + j] * y[j]);
    }

    // Retrossubstituicao U x (x) = y
    for (int i = ls->n - 1; i >= 0; i--)
    {
        x[i] = y[i]; // x[i] = y[i]
        int line = i * ls->n;
        for (int j = i + 1; j < ls->n; j++)
            // x[i] -= U(i, j) * x[j]
            x[i] -= (ls->U.A[line + j] * x[j]);

        x[i] /= ls->U.A[line + i]; // x[i] /= U(i, i)
    }

    free(y);
    y = NULL;

    return x;
}