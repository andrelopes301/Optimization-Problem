#ifndef ALGORITMO_H
#define ALGORITMO_H

/// Algoritmo Trepa-Colinas 

void gera_vizinho(int a[], int b[], int elem, int subconj);
int trepa_colinas(int sol[], int* mat, int elem, int subconj, int iteracoes);


/// Algoritmo Evolutivo 


// Estrutura para armazenar parametros
typedef struct {
    // Tamanho da população
    int   popsize;
    // Número de objetos que se podem colocar na mochila
    int     numGenes; // = numero de elementos
    int     subconj;	
    // Probabilidade de mutação
    float   pm;
    // Probabilidade de recombinação
    float   pr;
    // Tamanho do torneio para seleção do pai da próxima geração
	int     tsize;
    // Constante para avaliação com penalização
    float   ro;
	// Número de gerações
    int     numGenerations;
}info;

// Individuo (solução)
typedef struct individual chrom, * pchrom;

struct individual {
    // Solução (objetos que estão dentro da mochila)
    int     *p;
    // Valor da qualidade da solução
	int   fitness;
    // 1 se for uma solução válida e 0 se não for
	int     valido;

};



void tournament(pchrom pop, info d, pchrom parents);
void genetic_operators(pchrom parents, info d, pchrom offspring);
void crossover(pchrom parents, info d, pchrom offspring);
void mutation(pchrom offspring, info d);



#endif