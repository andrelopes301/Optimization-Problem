#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "algoritmo.h"

/// Algoritmo Trepa-Colinas 

int* init_dados(char* nome, int* elem, int* subconj);
void gera_sol_inicial(int* sol, int elem, int subconj);
void escreve_sol(int* sol, int* mat, int elem, int subconj);
void substitui(int a[], int b[], int n);
void init_rand();
int random_l_h(int min, int max);
float rand_01();


/// Algoritmo Evolutivo

info init_data(int elem, int subconj, int* mat);
pchrom init_pop(info d);
chrom get_best(pchrom pop, info d, chrom best);
void write_best(chrom x, info d);
int flip();

#endif
