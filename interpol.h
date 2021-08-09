#ifndef __INTERPOL_H__
#define __INTERPOL_H__

// Bibliotecas
#include "lin_system.h"

/*!
    \brief  Gera a matriz de coeficientes para a resolução de uma interpolação

    \param  x   Valores tabelados
    \param  ls  Sistema linear
*/
void gen_poly_interpol_coef(real_t *x, lin_system_t *ls);

/*!
    \brief  Gera a matriz de coeficientes para a resolução de um problema de ajuste de curva

    \param  x   Valores tabelados
    \param  ls  Sistema linear
*/
void gen_curve_coef(real_t *x, lin_system_t *ls);

/*!
    \brief  Gera o vetor de termos independentes 
            para a resolução de um problema de ajuste de curva

    \param  x   Valores tabelados
    \param  y   Valores da função: y = f(x)
    \param  ls  Sistema linear
*/
void gen_curve_b(real_t *x, real_t *y, lin_system_t *ls);

#endif