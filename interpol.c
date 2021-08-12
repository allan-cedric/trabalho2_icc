#include "interpol.h"

void gen_poly_interpol_coef(real_t *x, lin_system_t *ls)
{
    for (int i = 0; i < ls->n; i++)
    {
        int line = i * ls->n;
        ls->A.A[line] = 1.0;
        for (int j = 1; j < ls->n; j++)
            ls->A.A[line + j] = pow(x[i], j);
    }
}

/*!
    \todo Otimizar o que for necessário
*/
void gen_curve_coef(real_t *x, lin_system_t *ls)
{
    // Primeira linha
    ls->A.A[0] = (real_t)(ls->n);
    for (int j = 1; j < ls->n; j++)
    {
        ls->A.A[j] = 0.0;
        for (int i = 0; i < ls->n; i++)
            ls->A.A[j] += pow(x[i], j);
    }

    // Restante das linhas
    for (int k = 1; k < ls->n; k++)
    {
        memcpy(ls->A.A + k * ls->n, ls->A.A + (k - 1) * ls->n + 1, ls->n * sizeof(real_t));
        // n termos a partir de A(k-1, 1) copiados para posição inicial A(k, 0)

        int last_index = k * ls->n + (ls->n - 1);
        real_t exp = ls->n + (k - 1);

        ls->A.A[last_index] = 0.0;
        for (int i = 0; i < ls->n; i++)
            ls->A.A[last_index] += pow(x[i], exp);
    }
}

void gen_curve_b(real_t *x, real_t *y, lin_system_t *ls)
{
    // Primeira linha
    ls->b[0] = 0.0;
    for (int j = 0; j < ls->n; j++)
        ls->b[0] += y[j];

    // Restante das linhas
    for (int i = 1; i < ls->n; i++)
    {
        ls->b[i] = 0.0;
        for (int j = 0; j < ls->n; j++)
            ls->b[i] += y[j] * pow(x[j], i);
    }
}