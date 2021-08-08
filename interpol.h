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

#endif