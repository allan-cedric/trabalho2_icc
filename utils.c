// Source file: 'utils.c'
// Autores:
// Allan Cedric G. B. Alves da Silva - GRR20190351
// Gabriel N. Hishida do Nascimento - GRR20190361

// Biblioteca que implementa algumas funções auxiliares de temporização,
// troca de elementos e teste de alocação

#include "utils.h"

double timestamp(void)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)(tp.tv_sec * 1000.0 + tp.tv_usec / 1000.0));
}

void alloc_test(void *p)
{
    if (!p)
    {
        fprintf(stderr, "Memory allocation error!\n");
        exit(MEM_ALLOC_ERROR);
    }
}

void swap_items(void *a, void *b, size_t size)
{
    byte_t temp, *pa = a, *pb = b;
    for (size_t i = 0; i < size; i++)
    {
        // Troca byte por byte
        temp = pa[i];
        pa[i] = pb[i];
        pb[i] = temp;
    }
}
