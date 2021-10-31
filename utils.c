#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "algoritmo.h"
#include "funcao.h"

#define POPSIZE 100
#define PM 0.001
#define PR 0.3
#define NUMGENERATIONS 1000
#define TSIZE 2


// Recebe: nome do ficheiro, numero de elementos e numero de subconjuntos 
// Devolve a matriz de adjacencias
int* init_dados(char* nome, int *elem, int *subconj)
{
	FILE* f;
	int  (*p)[3];
	int i, n, j;


	f = fopen(nome, "r");
	if (!f)	{
		printf("Erro ao abrir o ficheiro\n");
		exit(1);
	}

	//Leitura dos parametros e ignora resto da linha
	fscanf(f,"%d %d %*[^\n]", elem, subconj);
	printf("\nNumero de Elementos: %d", *elem);
	printf("\nNumero de Subconjuntos: %d\n", *subconj);

	printf("\n---------------------------------------------------\n");

	n = *elem * (*elem - 1) / 2;
	//printf("Numero de Linhas: %d\n\n", n);

	// Alocacao dinamica da matriz
	p = malloc(sizeof(*p) * n);

	if (!p) {
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}

	// Preenchimento da matriz
	for (i = 0; i < n; i++){
		for (j = 0; j < 3; j++)
			fscanf(f, "%d", &p[i][j]);
	}

		// printf("        X:    Y:   Dist(X,Y):\n");
	   //  for (i = 0; i < n; i++) 
      //printf("       [%d]   [%d]   [%d] \n", p[i][0], p[i][1], p[i][2]);


	fclose(f);
	return p;
}

// Gera a solucao inicial
// Parametros: solucao, numero de elementos e numero de subconjuntos
void gera_sol_inicial(int* sol, int elem, int subconj)
{
	int i, x;

	for (i = 0; i < elem; i++)
		sol[i] = 0;

	int j = 1;
	for (int sub = 0; sub < subconj-1; sub++) {
		for (i = 0; i < elem / subconj; i++)  {
		do{
			x = random_l_h(0, elem - 1);
		} while (sol[x] != 0);
		sol[x] = j;
		}
		j++;
	}
}
 
// Escreve solucao
// Parametros: solucao, grafo, numero de elementos e numero de subconjuntos
void escreve_sol(int* sol, int* mat, int elem, int subconj)
{
	int i, k;
	//int qualidade;
	for(k=0; k < subconj; k++){
		printf("   Div(G%d):  ", k+1);
		for (i = 0; i < elem; i++){
			if (sol[i] == k)
				printf("%d  ", i); 
		}
		printf("\n");
	}
	//qualidade = calcula_fit(sol, mat, elem, subconj);
	//	printf("\n Qualidade: %d\n", qualidade);
}


// Copia vector b para a (tamanho n)
void substitui(int a[], int b[], int elem){
	int i;
	for (i = 0; i < elem; i++)
		a[i] = b[i];
}

// Inicializa o gerador de numeros aleatorios
void init_rand() {
	srand((unsigned)time(NULL));
}

// Devolve valor inteiro aleatorio entre min e max
int random_l_h(int min, int max) {
	return min + rand() % (max - min + 1);
}

// Devolve um valor real aleatorio do intervalo [0, 1]
float rand_01() {
	return ((float)rand()) / RAND_MAX;
}


/// Algoritmo Evolutivo

info init_data(int elem, int subconj, int* mat) {
	info x;
	x.numGenes = elem;
	x.subconj = subconj;
	x.popsize = POPSIZE;
	x.pm = PM;
	x.pr = PR;
	x.tsize = TSIZE;
	x.ro = 0.0;
	x.numGenerations = NUMGENERATIONS;
	return x;
}

// Criacao da populacao inicial. O vector e alocado dinamicamente
// Parâmetro de entrada: Estrutura com parâmetros do problema
// Parâmetro de saída: Preenche da estrutura da população apenas o vector binário com os elementos que estão dentro ou fora da mochila
pchrom init_pop(info d) {

	int i, j;
	pchrom indiv;
//	printf("     d.elem: %d        \n", d.numGenes);
//	printf("     d.subconj: %d      \n", d.subconj);

	indiv = malloc(sizeof(chrom) * d.popsize);
	if (!indiv) {
		printf("Erro a alocar memoria\n");
		exit(1);
	}

	for (i = 0; i < d.popsize; i++) {

		indiv[i].p = malloc(sizeof(int) * d.numGenes);
		if (!indiv[i].p) {
			printf("Erro a alocar memoria\n");
			exit(2);
		}
		gera_sol_inicial(indiv[i].p, d.numGenes, d.subconj);


	}

			//for (int j = 0; j < d.popsize; j++)
	      //for(int k =0; k< d.numGenes; k++)
		//printf("indiv[%d].p = %d\n", j, indiv[j].p[k]);


	return indiv;
}

chrom get_best(pchrom pop, info d, chrom best) {

	int i;
	//printf("\nBest fitness = %d \n", best.fitness);
	for (i = 0; i < d.popsize; i++) {
		if (best.fitness < pop[i].fitness) {
			best = pop[i];
			//printf("\npop[%d] = %d \n", i, best.fitness);
		}
	}
//	printf("\nBest = %d \n", best);
	return best;
}

void write_best(chrom x, info d) {

	int i;
	//int k;

	//printf("\nBest individual: %d\n\n", x.fitness);

	/*
	for (i = 0; i < d.numGenes; i++)
		printf("[%d]", x.p[i]);
	putchar('\n');

	for (k = 0; k < d.subconj; k++) {
		printf("   Div(G%d):  ", k + 1);
		for (i = 0; i < d.numGenes; i++) {
			if (x.p[i] == k)
				printf("%d  ", i);
		}
		printf("\n");
	}


	

	printf("\nBest individual: %4.1f\n", x.fitness);
	for (i = 0; i < d.numGenes; i++)
		printf("%d", x.p[i]);
	printf("\n");
	*/
}
