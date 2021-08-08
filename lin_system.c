#include "lin_system.h"

lin_system_t *alloc_lin_system(int n)
{
    lin_system_t *ls = (lin_system_t *)malloc(sizeof(lin_system_t));
    alloc_test(ls);

    ls->n = n;
    ls->A = (real_t *)malloc(ls->n * ls->n * sizeof(real_t));
    alloc_test(ls->A);

    ls->b = (real_t *)malloc(ls->n * sizeof(real_t));
    alloc_test(ls->b);

    return ls;
}

void free_lin_system(lin_system_t *ls)
{
    if (ls)
    {
        if (ls->A)
        {
            free(ls->A);
            ls->A = NULL;
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