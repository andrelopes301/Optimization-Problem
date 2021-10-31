#ifndef FUNCAO_H
#define FUNCAO_H
#include "algoritmo.h"

/// Algoritmo Trepa-Colinas 

int calcula_fit(int* sol, int(*p)[3], int elem, int subconj);

/// Algoritmo Evolutivo 
void evaluate(pchrom pop, info d, int** mat);

#endif 