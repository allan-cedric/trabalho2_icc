#include "interpol.h"

void gen_poly_interpol_coef(real_t *x, lin_system_t *ls)
{
    for (int i = 0; i < ls->n; i++)
    {
        int line = i * ls->n;
        ls->A[line] = 1.0;
        for (int j = 1; j < ls->n; j++)
            ls->A[line + j] = pow(x[i], j);
    }
}