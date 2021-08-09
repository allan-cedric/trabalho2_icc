// Header file: 'matrix.h'
// Autores:
// Allan Cedric G. B. Alves da Silva - GRR20190351
// Gabriel N. Hishida do Nascimento - GRR20190361

// Biblioteca que implementa estruturas e operações matrizes

#ifndef __MATRIX_H__
#define __MATRIX_H__

// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "utils.h"

// Códigos de erro
#define READ_ERROR 5
#define INV_MAT_ERROR 6

// Estrutura de dados de uma matriz quadrada
typedef struct matrix_t
{
    int n;     // Ordem
    real_t *A; // Matriz
} matrix_t;

/*!
    \brief  Leitura de uma matriz quadrada da entrada padrão (stdin)

    \param  m   Matriz quadrada

    \return 0 (Sucesso), 
            READ_ERROR (Erro de leitura) 
            ou EOF (Fim de arquivo)
*/
int read_matrix(matrix_t *m);

/*!
    \brief  Libera memória de uma matriz quadrada

    \param  m   Matriz quadrada
*/
void free_matrix(matrix_t *m);

/*!
    \brief  Imprime uma matriz quadrada em um arquivo de saída

    \param  arq         Descritor de arquivo
    \param  m           Matriz quadrada
    \param  ord_flag    Imprime a ordem da matriz se for diferente de 0               
*/
void fprint_matrix(FILE *arq, matrix_t *m, bool ord_flag);

/*!
    \brief  Cria uma nova cópia de uma matriz quadrada

    \param  src     Matriz quadrada a ser copiada
    \param  dest    Matriz quadrada destino  
*/
void copy_matrix(matrix_t *src, matrix_t *dest);

/*!
    \brief  Troca duas linhas de uma matriz quadrada

    \param  m   Matriz quadrada
    \param  i   Linha i
    \param  j   Linha j       
*/
void swap_matrix_lines(matrix_t *m, int i, int j);

/*!
    \brief  Troca duas colunas de uma matriz quadrada

    \param  m   Matriz quadrada
    \param  i   Coluna i
    \param  j   Coluna j       
*/
void swap_matrix_columns(matrix_t *m, int i, int j);

/*!
    \brief  Encontra o elemento pivo de uma coluna a partir de uma certa linha

    \param  m   Matriz quadrada
    \param  i   Linha i

    \return Linha do elemento pivo
*/
int pivot_index(matrix_t *m, int i);

/*!
    \brief  Decomposição LU de uma matriz quadrada

    \param  m       Matriz quadrada para realizar a decomposicao
    \param  L       Matriz L (Multiplicadores)
    \param  U       Matriz U (Coeficientes escalonados)
    \param  P       Vetor P (Armazena informação das linhas trocadas)
    \param  p_flag  Aciona pivoteamento parcial (1) ou não (0)
    \param  rtime   Tempo de triangularização da matriz U

    \return 0 (Sucesso) ou 
            INV_MAT_ERROR (Divisão por zero ou não tem inversa)
*/
int LU_decomp(matrix_t *m, matrix_t *L, matrix_t *U, int *P, bool p_flag, double *rtime);

/*!
    \brief  Calcula a matriz inversa de uma matriz quadrada
    
    \param  m       Matriz quadrada que se deseja a inversa
    \param  inv     Matriz inversa resultante
    \param  p_flag  Aciona pivoteamento parcial (1) ou não (0)
    \param  rtime   Vetor para armazenar os tempos de execução da triangularização (LU) 
                    e das soluções (inv)

    \return 0 (Sucesso) ou INV_MAT_ERROR (Não tem inversa)
*/
int inverse_matrix(matrix_t *m, matrix_t *inv, bool p_flag, double *rtime);

/*!
    \brief  Calcula a norma L2 do resíduo de um sistema linear

    \param  m   Matriz quadrada dos coeficientes do sistema linear
    \param  x   Vetor solução
    \param  y   Vetor de termos independentes

    \return Valor da norma L2 do resíduo
*/
real_t L2_norm(matrix_t *m, real_t *x, real_t *y);

/*!
    \brief  Imprime as normas L2 do resíduo da solução de uma matriz inversa

    \param  arq Descritor de arquivo
    \param  m   Matriz original m
    \param  inv Matriz inversa de m

*/
void fprint_L2_norm_inverse_matrix(FILE *arq, matrix_t *m, matrix_t *inv);

#endif