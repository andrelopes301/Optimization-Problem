#define _CRT_SECURE_NO_WARNINGS 1
#include "utils.h"
#include "funcao.h"


/// Algoritmo Trepa-Colinas 

// Calcula a qualidade de uma solução
// Recebe:  A solução, a, a matriz de adjacências, mat, e o número de vértices, vert
// Devolve: O custo que é o número de ligações que existem na solução
int calcula_fit(int *sol, int(*p)[3], int elem, int subconj){
	int n,  i;
	int aux[1000];
	int* dist;
	int qualidade = 0;
	int k = 0;
	int x = 0;
	int j = 1;
	int f = 0;
	int c = 1;

	// numero de linhas da matriz lida
	n = elem * (elem - 1) / 2; 

	dist = malloc(sizeof(*dist) * n);

	if (!dist) {
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}
	for(int sub=0; sub < subconj; sub++){
		for (i = 0; i < elem; i++) {
			if (sol[i] == sub) {
				aux[j] = i;
				//printf("aux[%d] = %d   ", j, aux[j]);
				j++;
			}
		}
	for (int m = 0; m < (elem/subconj); m++) {
		c++;
		for (k = 0; k < n; k++) {
			if (p[k][0] == aux[f]) {
				if (p[k][1] == aux[j]) {
					qualidade = qualidade + p[k][2];
					//printf("\nQualidade: %d\n", qualidade);
			    	// 	dist[x] = p[k][2];
					//printf("\n       dist(%d,%d) = %d", aux[f], aux[j], dist[x]);
					j++;
				}
			}
		}
		j = c;
		f++;
	}
	//x = 0;
	j = 1;
	f = 0;
	c = 1;
	}
	free(dist);
	//printf("\nQualidade Final: %d\n", qualidade);

	return qualidade;
}

/// Algoritmo Evolutivo 



// Avaliacao da população
// Parâmetros de entrada: populacao (pop), estrutura com parametros (d) e matriz com dados do problema (mat)
// Parâmetros de saída: Preenche pop com os valores de fitness e de validade para cada solução
void evaluate(pchrom pop, info d, int** mat) {

	int i;

	for (i = 0; i < d.popsize; i++) {
		pop[i].fitness = calcula_fit(pop[i].p, mat, d.numGenes, d.subconj);
		//printf("\npop[%d].fitness: %d\n",i, pop[i].fitness);
	}

}
