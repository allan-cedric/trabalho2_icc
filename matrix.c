// Source file: 'matrix.c'
// Autores:
// Allan Cedric G. B. Alves da Silva - GRR20190351
// Gabriel N. Hishida do Nascimento - GRR20190361

// Biblioteca que implementa estruturas e operações matrizes

#include "matrix.h"

int read_matrix(matrix_t *m)
{
    int n, ret;
    real_t a;

    ret = scanf("%i", &n); // Le a ordem da matriz
    if (ret == EOF)        // Fim de arquivo
        return EOF;
    if (ret != 1) // Erro de leitura
    {
        fprintf(stderr, "Reading error!\n");
        return READ_ERROR;
    }

    m->n = n;
    m->A = (real_t *)malloc(m->n * m->n * sizeof(real_t)); // Aloca matriz
    alloc_test(m->A);

    // Leitura dos coeficientes da matriz
    for (int i = 0; i < m->n; i++)
    {
        for (int j = 0; j < m->n; j++)
        {
            ret = scanf("%lf", &a); // Le um elemento da matriz
            if (ret != 1)           // Erro de leitura
            {
                fprintf(stderr, "Reading error!\n");
                return READ_ERROR;
            }
            m->A[i * m->n + j] = a; // Atribuição do elemento na matriz alocada
        }
    }
    return 0;
}

void free_matrix(matrix_t *m)
{
    if (m->A)
        free(m->A);
    m->A = NULL;
}

void fprint_matrix(FILE *arq, matrix_t *m, bool ord_flag)
{
    if (ord_flag) // Impressão da ordem
        fprintf(arq, "%i\n", m->n);

    for (int i = 0; i < m->n; i++)
    {
        for (int j = 0; j < m->n; j++)
            fprintf(arq, "%.17g ", m->A[i * m->n + j]); // Impressão dos elementos da matriz
        fprintf(arq, "\n");
    }
}

void copy_matrix(matrix_t *src, matrix_t *dest)
{
    dest->n = src->n;
    dest->A = (real_t *)malloc(dest->n * dest->n * sizeof(real_t)); // Aloca matriz destino
    alloc_test(dest->A);

    memcpy(dest->A, src->A, dest->n * dest->n * sizeof(real_t)); // Cópia de 'src' em 'dest'
}

void swap_matrix_lines(matrix_t *m, int i, int j)
{
    for (int k = 0; k < m->n; k++)
        // Troca o elemento A(i, k) com o elemento A(j, k)
        swap_items(&m->A[i * m->n + k], &m->A[j * m->n + k], sizeof(real_t));
}

void swap_matrix_columns(matrix_t *m, int i, int j)
{
    for (int k = 0; k < m->n; k++)
        // Troca o elemento A(k, i) com o elemento A(k, j)
        swap_items(&m->A[k * m->n + i], &m->A[k * m->n + j], sizeof(real_t));
}

int pivot_index(matrix_t *m, int i)
{
    int p_index = i; // Linha do pivo
    for (int k = i + 1; k < m->n; k++)
    {
        // Se |A(k, i)| > |A(p_index, i)|
        int current = k * m->n + i;
        if (isgreater(fabs(m->A[current]), fabs(m->A[p_index * m->n + i])))
            p_index = k;
    }
    return p_index;
}

int LU_decomp(matrix_t *m, matrix_t *L, matrix_t *U, int *P, bool p_flag, real_t *rtime)
{
    L->n = m->n;
    L->A = (real_t *)calloc(L->n * L->n, sizeof(real_t)); // Aloca matriz L
    alloc_test(L->A);

    copy_matrix(m, U); // Cópia da matriz original 'm' na matriz 'U'

    // Decomposição LU
    *rtime = timestamp();
    for (int i = 0; i < U->n; i++)
    {
        // Tratamento do pivoteamento parcial
        if (p_flag)
        {
            int p_index = pivot_index(U, i);
            if (p_index != i)
            {
                swap_matrix_lines(U, i, p_index);
                swap_matrix_lines(L, i, p_index);
            }
            P[i] = p_index; // Par de linhas: (i, p_index)
        }

        // Caso tenha um zero na diagonal principal, não tem inversa
        if (fabs(U->A[i * U->n + i]) <= DBL_EPSILON)
            return INV_MAT_ERROR;

        // Geração da matriz L e U
        L->A[i * L->n + i] = 1.0; // L(i, i) = 1.0
        for (int k = i + 1; k < U->n; k++)
        {
            real_t mp = U->A[k * U->n + i] / U->A[i * U->n + i]; // mp = U(k, i)/U(i, i)
            L->A[k * L->n + i] = mp;                             // L(k, i) = mp
            U->A[k * U->n + i] = 0.0;                            // U(k, i) = 0.0

            for (int j = i + 1; j < U->n; j++)
                U->A[k * U->n + j] -= (U->A[i * U->n + j] * mp); // U(k, j) -= U(i, j) * mp
        }
    }
    *rtime = timestamp() - *rtime;
    return 0;
}

int inverse_matrix(matrix_t *m, matrix_t *inv, bool p_flag, real_t *rtime)
{
    matrix_t L, U;
    real_t *I = NULL, *scaling_I = NULL;
    int *P = NULL, ret;
    real_t init_time;

    // Inicializa matriz inversa
    inv->n = m->n;
    inv->A = NULL;

    if (p_flag)
    {
        P = (int *)malloc(m->n * sizeof(int)); // Aloca vetor P
        alloc_test(P);
    }

    // Tempo de triangularização [0],
    // Tempo médio para solução (L x scaling_I = I) [1]
    // Tempo médio para solução (U x inv = scaling_I) [2]
    rtime[0] = 0.0;
    rtime[1] = 0.0;
    rtime[2] = 0.0;

    ret = LU_decomp(m, &L, &U, P, p_flag, &rtime[0]); // Decomposição LU da matriz 'm'
    if (ret)                                          // Não tem inversa
    {
        free_matrix(&L);
        free_matrix(&U);

        if(p_flag)
        {
            free(P);
            P = NULL;
        }
        return INV_MAT_ERROR;
    }

    I = (real_t *)malloc(m->n * sizeof(real_t)); // Aloca vetor da matriz identidade
    alloc_test(I);

    scaling_I = (real_t *)malloc(m->n * sizeof(real_t)); // Aloca vetor da matriz identidade escalonado
    alloc_test(scaling_I);

    inv->A = (real_t *)malloc(inv->n * inv->n * sizeof(real_t)); // Aloca matriz inversa
    alloc_test(inv->A);

    // ------------------------------------
    // Resolução da matriz inversa
    // ------------------------------------

    for (int k = 0; k < m->n; k++) // Coluna k (k-ésimo sistema linear)
    {
        // Atribui o vetor da matriz identidade apropriado
        for (int i = 0; i < m->n; i++)
            I[i] = 0.0;
        I[k] = 1.0;

        // Troca as linhas do vetor identidade, caso tenha pivoteamento
        if (p_flag)
        {
            for (int i = 0; i < m->n - 1; i++)
            {
                if (P[i] != i) // Troca se as linhas do par (i, P[i]) forem distintas
                    swap_items(&I[i], &I[P[i]], sizeof(real_t));
            }
        }

        // ------------------------------------
        // Resolução do sistema linear
        // ------------------------------------

        // Retrossubstituicao L x scaling_I = I
        init_time = timestamp();
        for (int i = 0; i < m->n; i++)
        {
            scaling_I[i] = I[i];
            for (int j = 0; j < i; j++)
                // scaling_I[i] -= L(i, j) * scaling[j]
                scaling_I[i] -= (L.A[i * L.n + j] * scaling_I[j]);
        }
        rtime[1] += timestamp() - init_time;

        // Retrossubstituicao U x inv = scaling_I
        init_time = timestamp();
        for (int i = m->n - 1; i >= 0; i--)
        {
            inv->A[i * inv->n + k] = scaling_I[i]; // inv(i, k) = scaling_I[i]
            for (int j = i + 1; j < m->n; j++)
                // inv(i, k) -= U(i, j) * inv(j, k)
                inv->A[i * inv->n + k] -= (U.A[i * U.n + j] * inv->A[j * inv->n + k]);

            inv->A[i * inv->n + k] /= U.A[i * U.n + i]; // inv(i, k) /= U(i, i)
        }
        rtime[2] += timestamp() - init_time;
    }
    rtime[1] /= (real_t)m->n;
    rtime[2] /= (real_t)m->n;

    // Libera memória
    free_matrix(&L);
    free_matrix(&U);

    if (p_flag)
    {
        free(P);
        P = NULL;
    }
    free(I);
    I = NULL;
    free(scaling_I);
    scaling_I = NULL;

    return 0;
}

real_t L2_norm(matrix_t *m, real_t *x, real_t *y)
{
    real_t L2_value = 0.0;
    for (int i = 0; i < m->n; i++)
    {
        real_t res = 0.0; // Valor do resíduo
        for (int j = 0; j < m->n; j++)
            res += m->A[i * m->n + j] * x[j]; // res += A(i, j) * x(j)

        res = y[i] - res;
        L2_value += res * res;
    }
    L2_value = sqrt(L2_value);
    return L2_value;
}

void fprint_L2_norm_inverse_matrix(FILE *arq, matrix_t *m, matrix_t *inv)
{
    real_t *I = (real_t *)malloc(m->n * sizeof(real_t)); // Aloca vetor da matriz identidade
    alloc_test(I);

    real_t *col = (real_t *)malloc(m->n * sizeof(real_t)); // Aloca vetor coluna da matriz inversa
    alloc_test(col);

    fprintf(arq, "# Norma L2 do Residuo: --");
    for (int k = 0; k < m->n; k++)
    {
        for (int i = 0; i < m->n; i++)
        {
            I[i] = 0.0;
            col[i] = inv->A[i * m->n + k]; // col(i) = inv(i, k)
        }
        I[k] = 1.0;

        fprintf(arq, " %.17g --", L2_norm(m, col, I)); // Impressão da norma L2
    }
    fprintf(arq, "\n");

    free(I);
    I = NULL;

    free(col);
    col = NULL;
}