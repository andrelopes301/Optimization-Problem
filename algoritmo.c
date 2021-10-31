#define _CRT_SECURE_NO_WARNINGS 1
#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"
#define PROB 0.01

/// Algoritmo Trepa-Colinas 

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de elementos e numero de subconjuntos
void gera_vizinho(int a[], int b[], int elem, int subconj)
{
	int i, x;

	for (i = 0; i < elem; i++)
		b[i] = a[i];

	for (i = 0; i < subconj; i++) {
		do
			x = random_l_h(0, elem - 1);
		while (b[x] != i);

		if (subconj - 1 > i)
			b[x] = i + 1;
		else
			b[x] = 0;
	}
}


// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de elementos e numero de subconjuntos
// Devolve o custo da melhor solucao encontrada
int trepa_colinas(int sol[], int* mat, int elem, int subconj, int ITER)
{
	int* nova_sol, nova_sol2;
	int qualidade, qualidade_viz,qualidade_viz2, i;
	
	nova_sol = malloc(sizeof(int)*elem);
	nova_sol2 = malloc(sizeof(int) * elem);

	if (nova_sol == NULL) {
		printf("Erro na alocacao de memoria");
		exit(1);
	}

	if (nova_sol2 == NULL) {
		printf("Erro na alocacao de memoria");
		exit(1);
	}

	// Avalia solucao inicial
	qualidade = calcula_fit(sol, mat, elem, subconj);
	
	for (i = 0; i < ITER; i++) {

		// Gera o vizinho1
		gera_vizinho(sol, nova_sol, elem, subconj);	
		// Avalia o vizinho1
		qualidade_viz = calcula_fit(nova_sol, mat, elem, subconj);

		// Vizinhança 1
		// Aceita vizinho se a qualidade aumentar (problema de maximizacao)

		if (qualidade_viz > qualidade) {
			substitui(sol, nova_sol, elem);
			qualidade = qualidade_viz;
		}
	
		/*
		// Vizinhança 2
		// Gera o vizinho2
		gera_vizinho(sol, nova_sol2, elem, subconj);
		// Avalia o vizinho2
		qualidade_viz2 = calcula_fit(nova_sol2, mat, elem, subconj);
		
		if (qualidade_viz2 > qualidade_viz) { //viz2 é melhor
			if (qualidade_viz2 > qualidade) {
				substitui(sol, nova_sol2, elem);
				qualidade = qualidade_viz2;
			}
			else {		//viz é melhor
				if (qualidade_viz > qualidade) {
					substitui(sol, nova_sol, elem);
					qualidade = qualidade_viz;
				}
			}
		}

		*/
		/*
		// Trepa colinas Probabilistico

		if (qualidade_viz >= qualidade){ //Solucao melhor ou igual é sempre aceite
			substitui(sol, nova_sol, elem);
		qualidade = qualidade_viz;
		} else {	// Solucao pior tambem pode ser aceite
			if (rand_01() < PROB) { //Isto ajuda a fugir a maximos locais
				substitui(sol, nova_sol, elem, subconj);
				qualidade = qualidade_viz;
			}
		}
			*/
	}

	free(nova_sol);
	free(nova_sol2);
	return qualidade;
}



/// Algoritmo Evolutivo


// Preenche uma estrutura com os progenitores da próxima geração, de acordo com o resultados do torneio binario (tamanho de torneio: 2)
// Parâmetros de entrada: população actual (pop), estrutura com parâmetros (d) e população de pais a encher
void tournament(pchrom pop, info d, pchrom parents)
{
	int i, x1, x2;

	// Realiza popsize torneios
	for(i=0; i<d.popsize;i++)	{
		x1 = random_l_h(0, d.popsize-1);
		do {
			x2 = random_l_h(0, d.popsize - 1);
		} while (x1 == x2);
		if(pop[x1].fitness > pop[x2].fitness)		// Problema de maximizacao
			parents[i]=pop[x1];
		else
			parents[i]=pop[x2];
	}
}


// Operadores geneticos a usar na geração dos filhos
// Parâmetros de entrada: estrutura com os pais (parents), estrutura com parâmetros (d), estrutura que guardará os descendentes (offspring)
void genetic_operators(pchrom parents, info d, pchrom offspring)
{
    // Recombinação com um ponto de corte
	crossover(parents, d, offspring);
	// Mutação binária
	mutation(offspring, d);
}


// Preenche o vector descendentes com o resultado das operações de recombinação
// Parâmetros de entrada: estrutura com os pais (parents), estrutura com parâmetros (d), estrutura que guardará os descendentes (offspring)
void crossover(pchrom parents, info d, pchrom offspring)
{
	int i, j, point;

	for (i = 0; i < d.popsize; i += 2)
	{
		if (rand_01() < d.pr)
		{
			point = random_l_h(0, d.numGenes - 1);
			for (j = 0; j < point; j++)
			{
				offspring[i].p[j] = parents[i].p[j];
				offspring[i + 1].p[j] = parents[i + 1].p[j];
			}
			for (j = point; j < d.numGenes; j++)
			{
				offspring[i].p[j] = parents[i + 1].p[j];
				offspring[i + 1].p[j] = parents[i].p[j];
			}
		}
		else
		{
			offspring[i] = parents[i];
			offspring[i + 1] = parents[i + 1];
		}
	}
}


// Mutação binária com vários pontos de mutação
// Parâmetros de entrada: estrutura com os descendentes (offspring) e estrutura com parâmetros (d)
void mutation(pchrom offspring,  info d) {

	int i, k, x1, x2, aux;
	for (i = 0; i < d.popsize; i++) {
		if (rand_01() < d.pm) {
			for (k = 0; k < d.numGenes / 2; k++) {
				x1 = random_l_h(0, d.numGenes - 1);
				do {
					x2 = random_l_h(0, d.numGenes - 1);
				} while (x1 == x2);
				aux = offspring[i].p[x1];
				offspring[i].p[x1] = offspring[i].p[x2];
				offspring[i].p[x2] = aux;
			}
		}
	}




}



