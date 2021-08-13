#include "interpol.h"

void gen_poly_interpol_matcoef(real_t *x, lin_system_t *ls)
{
    for (int i = 0; i < ls->n; i++)
    {
        int line = i * ls->n;
        real_t result = x[i];

        ls->A.A[line] = 1.0; // A(i, 0) = 1.0
        for (int j = 1; j < ls->n; j++, result *= x[i])
            ls->A.A[line + j] = result; // A(i, j) = x(i)^j
    }
}

void gen_curve_matcoef(real_t *x, lin_system_t *ls)
{
    // --- Primeira linha ---
    ls->A.A[0] = (real_t)(ls->n); // A(0, 0) = n
    for (int j = 1; j < ls->n; j++)
    {
        ls->A.A[j] = 0.0; // A(0, j) = 0.0

        // --- Sem Unroll ---
        // for (int i = 0; i < ls->n; i++)
        //     ls->A.A[j] += pow(x[i], j); // A(0, j) += x(i)^j

        // --- Unroll ---
        int end_step = ls->n - (ls->n % UNROLL_STEP);
        for (int i = 0; i < end_step; i += UNROLL_STEP)
            // A(0, j) += x(i)^j + x(i + 1)^j + x(i + 2)^j + x(i + 3)^j
            ls->A.A[j] += pow(x[i], j) + pow(x[i + 1], j) + pow(x[i + 2], j) + pow(x[i + 3], j);

        // Resíduo
        for (int i = end_step; i < ls->n; i++)
            ls->A.A[j] += pow(x[i], j); // A(0, j) += x(i)^j
    }

    // --- Restante das linhas ---
    for (int k = 1; k < ls->n; k++)
    {
        // n termos a partir de A(k-1, 1) copiados a partir da posição inicial A(k, 0)
        memcpy(ls->A.A + k * ls->n, ls->A.A + (k - 1) * ls->n + 1, ls->n * sizeof(real_t));

        // --- Calcula o último termo A(k, n - 1) da linha ---
        int last_index = k * ls->n + (ls->n - 1); // Índice (k, n - 1)
        real_t exp = ls->n + (k - 1);             // Expoente (n - k - 1)

        ls->A.A[last_index] = 0.0; // A(k, n - 1) = 0.0

        // --- Sem Unroll ---
        // for (int i = 0; i < ls->n; i++)
        //     ls->A.A[last_index] += pow(x[i], exp); // A(k, n - 1) += x(i)^exp

        // --- Unroll ---
        int end_step = ls->n - (ls->n % UNROLL_STEP);
        for (int i = 0; i < end_step; i += UNROLL_STEP)
            // A(k, n - 1) += x(i)^exp + x(i + 1)^exp + x(i + 2)^exp + x(i + 3)^exp
            ls->A.A[last_index] += pow(x[i], exp) + pow(x[i + 1], exp) + pow(x[i + 2], exp) + pow(x[i + 3], exp);

        // Resíduo
        for (int i = end_step; i < ls->n; i++)
            ls->A.A[last_index] += pow(x[i], exp); // A(k, n - 1) += x(i)^exp
    }
}

void gen_curve_matb(real_t *x, real_t *y, lin_system_t *ls)
{
    // --- Primeira linha ---
    ls->b[0] = 0.0; // b(0) = 0.0
    for (int j = 0; j < ls->n; j++)
        ls->b[0] += y[j]; // b(0) += y(j)

    // --- Restante das linhas ---
    for (int i = 1; i < ls->n; i++)
    {
        ls->b[i] = 0.0; // b(i) = 0.0
        for (int j = 0; j < ls->n; j++)
            ls->b[i] += y[j] * pow(x[j], i); // b(i) += y(j) * (x(j)^i)
    }
}