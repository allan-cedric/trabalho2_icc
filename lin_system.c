#include "lin_system.h"

lin_system_t *alloc_lin_system(int n)
{
    lin_system_t *ls = (lin_system_t *)malloc(sizeof(lin_system_t));
    alloc_test(ls);

    ls->n = n;

    ls->A.n = n;
    ls->A.A = (real_t *)malloc(ls->n * ls->n * sizeof(real_t));
    alloc_test(ls->A.A);

    ls->L.n = n;
    ls->L.A = NULL;

    ls->U.n = n;
    ls->U.A = NULL;

    ls->P = (int *)malloc(ls->n * sizeof(int));
    alloc_test(ls->P);

    ls->b = (real_t *)malloc(ls->n * sizeof(real_t));
    alloc_test(ls->b);

    return ls;
}

void free_lin_system(lin_system_t *ls)
{
    if (ls)
    {
        free_matrix(&ls->A);
        free_matrix(&ls->L);
        free_matrix(&ls->U);
        if (ls->P)
        {
            free(ls->P);
            ls->P = NULL;
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

int LU_decomp_optimized(lin_system_t *ls)
{
    ls->L.A = (real_t *)calloc(ls->L.n * ls->L.n, sizeof(real_t)); // Aloca matriz L
    alloc_test(ls->L.A);

    copy_matrix(&ls->A, &ls->U); // Cópia da matriz original 'A' na matriz 'U'

    // Decomposição LU
    for (int i = 0; i < ls->U.n; i++)
    {
        // Tratamento do pivoteamento parcial
        int p_index = pivot_index(&ls->U, i);
        if (p_index != i)
        {
            swap_matrix_lines(&ls->U, i, p_index);
            swap_matrix_lines(&ls->L, i, p_index);
        }
        ls->P[i] = p_index; // Par de linhas: (i, p_index)

        int diag_index = i * ls->U.n + i;
        // Caso tenha um zero na diagonal principal, não tem inversa
        if (fabs(ls->U.A[diag_index]) <= DBL_EPSILON)
            return INV_MAT_ERROR;

        // Geração da matriz L e U
        ls->L.A[diag_index] = 1.0; // L(i, i) = 1.0
        for (int k = i + 1; k < ls->U.n; k++)
        {
            int index = k * ls->U.n + i;

            real_t mp = ls->U.A[index] / ls->U.A[diag_index]; // mp = U(k, i)/U(i, i)
            ls->L.A[index] = mp;                              // L(k, i) = mp
            ls->U.A[index] = 0.0;                             // U(k, i) = 0.0

            int line_k = k * ls->U.n; 
            int line_i = i * ls->U.n;

            // --- Sem Unroll ---
            // for (int j = i + 1; j < ls->U.n; j++)
            //     ls->U.A[line_k + j] -= (ls->U.A[line_i + j] * mp); // U(k, j) -= U(i, j) * mp

            // --- Unroll ---
            int end_step = ls->U.n - ((ls->U.n - (i + 1)) % UNROLL_STEP);
            for (int j = i + 1; j < end_step; j += UNROLL_STEP)
            {
                int kj = line_k + j;
                int ij = line_i + j;

                ls->U.A[kj] -= (ls->U.A[ij] * mp);
                ls->U.A[kj + 1] -= (ls->U.A[ij + 1] * mp);
                ls->U.A[kj + 2] -= (ls->U.A[ij + 2] * mp);
                ls->U.A[kj + 3] -= (ls->U.A[ij + 3] * mp);
            }
            // Resíduo
            for (int j = end_step; j < ls->U.n; j++)
                ls->U.A[line_k + j] -= (ls->U.A[line_i + j] * mp);
        }
    }
    return 0;
}

real_t *solve_lin_system(lin_system_t *ls)
{
    if (!ls->L.A || !ls->U.A)
    {
        fprintf(stderr, "Error: There is no LU decomposition!\n");
        return NULL;
    }

    real_t *x = (real_t *)malloc(ls->n * sizeof(real_t));
    alloc_test(x);

    real_t *y = (real_t *)malloc(ls->n * sizeof(real_t));
    alloc_test(y);

    real_t *b = (real_t *)malloc(ls->n * sizeof(real_t));
    alloc_test(b);
    memcpy(b, ls->b, ls->n * sizeof(real_t));

    // Troca as linhas do vetor b, caso tenha pivoteamento
    for (int i = 0; i < ls->n - 1; i++)
    {
        if (ls->P[i] != i) // Troca se as linhas do par forem distintas
            swap_items(&b[i], &b[ls->P[i]], sizeof(real_t));
    }

    // ------------------------------------
    // Resolução do sistema linear
    // ------------------------------------

    // Retrossubstituicao L x y = b
    for (int i = 0; i < ls->n; i++)
    {
        y[i] = b[i];
        int line = i * ls->n;
        for (int j = 0; j < i; j++)
            // y[i] -= L(i, j) * y[j]
            y[i] -= (ls->L.A[line + j] * y[j]);
    }

    // Retrossubstituicao U x (x) = y
    for (int i = ls->n - 1; i >= 0; i--)
    {
        x[i] = y[i]; // x[i] = y[i]
        int line = i * ls->n;
        for (int j = i + 1; j < ls->n; j++)
            // x[i] -= U(i, j) * x[j]
            x[i] -= (ls->U.A[line + j] * x[j]);

        x[i] /= ls->U.A[line + i]; // x[i] /= U(i, i)
    }

    free(y);
    y = NULL;

    free(b);
    b = NULL;

    return x;
}