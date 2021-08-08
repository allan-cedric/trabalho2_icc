#ifndef __LIN_SYSTEM_H__
#define __LIN_SYSTEM_H__

// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "utils.h"

// Estrutura de dados de um sistema linear
typedef struct lin_system_t
{
    int n;     // Ordem
    real_t *A; // Matriz de coeficientes
    real_t *b; // Vetor de termos independentes
} lin_system_t;

/*!
    \brief  Aloca um sistema linear de ordem n

    \param  n   Ordem do sistema

    \return Ponteiro para o sistema linear 
*/
lin_system_t *alloc_lin_system(int n);

/*!
    \brief  Libera um sistema linear de ordem n

    \param  ls  Sistema linear
*/
void free_lin_system(lin_system_t *ls);

#endif