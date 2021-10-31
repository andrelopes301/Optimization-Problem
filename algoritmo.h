#ifndef ALGORITMO_H
#define ALGORITMO_H

/// Algoritmo Trepa-Colinas 

void gera_vizinho(int a[], int b[], int elem, int subconj);
int trepa_colinas(int sol[], int* mat, int elem, int subconj, int iteracoes);


/// Algoritmo Evolutivo 


// Estrutura para armazenar parametros
typedef struct {
    // Tamanho da popula��o
    int   popsize;
    // N�mero de objetos que se podem colocar na mochila
    int     numGenes; // = numero de elementos
    int     subconj;	
    // Probabilidade de muta��o
    float   pm;
    // Probabilidade de recombina��o
    float   pr;
    // Tamanho do torneio para sele��o do pai da pr�xima gera��o
	int     tsize;
    // Constante para avalia��o com penaliza��o
    float   ro;
	// N�mero de gera��es
    int     numGenerations;
}info;

// Individuo (solu��o)
typedef struct individual chrom, * pchrom;

struct individual {
    // Solu��o (objetos que est�o dentro da mochila)
    int     *p;
    // Valor da qualidade da solu��o
	int   fitness;
    // 1 se for uma solu��o v�lida e 0 se n�o for
	int     valido;

};



void tournament(pchrom pop, info d, pchrom parents);
void genetic_operators(pchrom parents, info d, pchrom offspring);
void crossover(pchrom parents, info d, pchrom offspring);
void mutation(pchrom offspring, info d);



#endif