#ifndef __LIN_SYSTEM_H__
#define __LIN_SYSTEM_H__

// Bibliotecas
#include "matrix.h"

// Estrutura de dados de um sistema linear
typedef struct lin_system_t
{
    int n;      // Ordem
    matrix_t A; // Matriz de coeficientes
    matrix_t L; // Matriz de multiplicadores
    matrix_t U; // Matriz escalonada dos coeficientes
    int *P;     // Vetor que armazena informação de troca de linhas
    real_t *b;  // Vetor de termos independentes
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

int LU_decomp_optimized(lin_system_t *ls);

/*!
    \brief  Resolve um sistema linear de ordem n (Método de fatoração LU)

    \param  ls  Sistema linear

    \return Vetor real da solução do sistema. Senão NULL, caso não foi feita a decomposição LU.
*/
real_t *solve_lin_system(lin_system_t *ls);

#endif