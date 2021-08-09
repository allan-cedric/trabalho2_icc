// Header file: 'utils.h'
// Autores:
// Allan Cedric G. B. Alves da Silva - GRR20190351
// Gabriel N. Hishida do Nascimento - GRR20190361

// Biblioteca que implementa algumas funções auxiliares de temporização,
// troca de elementos e teste de alocação

#ifndef __UTILS_H__
#define __UTILS_H__

// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Códigos de erro
#define MEM_ALLOC_ERROR 4

typedef double real_t;        // Definição para o tipo real
typedef unsigned char byte_t; // Definição para o tipo byte

/*!
    \brief  Calcula o tempo corrente do programa

    \return Retorna o tempo em milissegundos
*/
double timestamp(void);

/*!
    \brief  Teste de alocação de memória
    
    \param  p   Bloco de memória genérico alocado
*/
void alloc_test(void *p);

/*!
    \brief  Troca entre elementos do mesmo tipo

    \param  a       Primeiro elemento
    \param  b       Segundo elemento
    \param  size    Tamanho do tipo
*/
void swap_items(void *a, void *b, size_t size);

/*!
    \brief  Imprime um vetor real

    \param  arr     Vetor real
    \param  size    Tamanho do vetor    
*/
void print_real_array(real_t *arr, size_t size);

#endif
