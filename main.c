#define _CRT_SECURE_NO_WARNINGS 1
#include "utils.h"
#include "algoritmo.h"
#include "funcao.h"
#define DEFAULT_RUNS 10


/// Algoritmo Trepa-Colinas 

void trepa_col(char nome_fich[100], int runs, int iter) {

	int elem, subconj, k, qualidade, best_qualidade = 0;
	int *grafo, *sol, *best;
	float mbf = 0.0;

	init_rand();

	// Preenche matriz de adjacencias
	grafo = init_dados(nome_fich, &elem, &subconj);
	sol = malloc(sizeof(int)*elem);
	best = malloc(sizeof(int)*elem);

	if (sol == NULL || best == NULL) {
		printf("Erro na alocacao de memoria\n");
		exit(1);
	}

	for (k = 0; k < runs; k++) {

		printf("\nRepeticao %d: \n", k + 1);
		// Gerar solucao inicial
		gera_sol_inicial(sol, elem, subconj);

		// Mostrar solucao original
		printf("\n Solucao original: \n");
		escreve_sol(sol, grafo, elem, subconj);
		qualidade = calcula_fit(sol, grafo, elem, subconj);
		printf("\n   Qualidade: %2d\n", qualidade);
		
		// Trepa colinas
		qualidade = trepa_colinas(sol, grafo, elem, subconj, iter);
		printf("\n Solucao do viz: \n");
		escreve_sol(sol, grafo, elem, subconj);

		// Mostrar solucao do viz
	 	printf("\n   Qualidade do viz: %2d\n\n", qualidade);
		printf("---------------------------------------------------\n");

		mbf += qualidade;
		if (k == 0 || best_qualidade < qualidade) {
			best_qualidade = qualidade;
			substitui(best, sol, elem);
		}
	}

	//Escrever resultados finais
	printf("\nResultados finais:");
	printf("\n\n   MBF: %f\n", mbf / k);
	printf("\nMelhor solucao encontrada: \n\n");
	escreve_sol(best, grafo, elem, subconj);
	printf("\n   Qualidade: %2d\n", best_qualidade);
	printf("\n---------------------------------------------------\n");

	//Libertar a memória 
	free(grafo);
	free(sol);
	free(best);
}


/// Algoritmo Evolutivo 
void evolutivo(char nome_fich[100], int runs) {

	int elem, subconj, k, i, inv, qualidade, best_qualidade, gen_Atual;
	int* grafo, * sol;
	float mbf = 0.0;
	info EA_param;
	pchrom pop = NULL, parents = NULL;
	chrom best_run, best_ever;

	// Preenche matriz de adjacencias
	grafo = init_dados(nome_fich, &elem, &subconj);

	// Preenche a matriz com dados dos objectos (peso e valor) e a estrutura EA_param que foram definidos 
	EA_param = init_data(elem, subconj, grafo);
	printf("\nTamanho da populacao: %d\n", EA_param.popsize);

	for (k = 0; k < runs; k++) {

		printf("\nRepeticao %d: \n", k + 1);
		// Geração da população inicial
		pop = init_pop(EA_param);
		evaluate(pop, EA_param, grafo);
		gen_Atual = 1;
		// Como ainda não existe, escolhe-se como melhor solução a primeira da população (poderia ser outra qualquer)
		best_run = pop[0];
		// Encontra-se a melhor solução dentro de toda a população
		best_run = get_best(pop, EA_param, best_run);
		printf("\nbest_run : %d\n", best_run.fitness);
		// Reserva espaço para os pais da população seguinte
		parents = malloc(sizeof(chrom) * EA_param.popsize);
		if (parents == NULL) {
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}
		// Ciclo de optimização
		while (gen_Atual <= EA_param.numGenerations) {

			// Torneio binário para encontrar os progenitores (ficam armazenados no vector parents)
			tournament(pop, EA_param, parents);
			// Aplica os operadores genéticos aos pais (os descendentes ficam armazenados na estrutura pop)
			genetic_operators(parents, EA_param, pop);
			// Avalia a nova população (a dos filhos)
			evaluate(pop, EA_param, grafo);
			// Actualiza a melhor solução encontrada
			best_run = get_best(pop, EA_param, best_run);
			gen_Atual++;
		}
		//printf("\nEA_param.popsize = %d\n", EA_param.popsize);

		// Contagem das soluções inválidas
		for (inv = 0, i = 0; i < EA_param.popsize; i++)
			if (pop[i].valido == 0)
				inv++;

		write_best(best_run, EA_param);
		printf("\nPercentagem Invalidos: %f\n", 100 * (float)inv / EA_param.popsize);
		mbf += best_run.fitness;
		if (k == 0 || best_run.fitness > best_ever.fitness) {
			best_ever = best_run;
		}
		//Libertar a memória
		free(pop);
		free(parents);
	}

	//Escrever resultados finais
	printf("\nResultados finais:");
	printf("\n\n   MBF: %f\n", mbf / k);
	printf("\nMelhor solucao encontrada: \n");
	write_best(best_ever, EA_param);
	
	//Libertar a memória 
	free(grafo);
}



int main(int argc, char *argv[])    {

	int opcao, opc;
	char nome_fich[100];
	// int *grafo, *sol, *best;
	int runs;
	int iteracoes = 0;

	printf("Problema da Diversidade Maxima de Grupos\n\n");

    // Lê os argumentos de entrada
	if (argc == 3)	{
		runs = atoi(argv[2]);
		strcpy(nome_fich, argv[1]);
	}
	// Se o número de execuções do processo não for colocado nos argumentos de entrada, define-o com um valor default
	else if (argc == 2) {
            runs = DEFAULT_RUNS;
            strcpy(nome_fich, argv[1]);
        }
        // Se o nome do ficheiro de informações não for colocado nos argumentos de entrada, pede-o novamente
        else {
            runs = DEFAULT_RUNS;
            printf("Nome do Ficheiro: ");
            gets(nome_fich);
        }
		 // Se o número de execuções do processo for menor ou igual a 0, termina o programa
			if (runs <= 0)
				return 0;



	// Escolher o tipo de algoritmo
	do {
		printf("\n 1 - Algoritmo Trepa-Colinas\n 2 - Algoritmo Evolutivo\n 3 - Algoritmo Hibrido\n 4 - Sair\n\n");
		printf("Introduza uma opcao: ");
		scanf("%d", &opcao);
		fgetc(stdin);  

		switch (opcao) {
		case(1):

			do {
				printf("\nEscolha um numero de iteracoes:\n");
				printf("\n 1 - 100  iteracoes\n 2 - 1000 iteracoes\n 3 - 2500 iteracoes\n 4 - 5000 iteracoes\n 5 - 10000 iteracoes\n");
				printf("\nIntroduza uma opcao: ");
				scanf("%d", &opc);
				fgetc(stdin);

				switch (opc) {
				case(1):
					iteracoes = 100;
					break;
				case(2):
					iteracoes = 1000;
					break;
				case(3):
					iteracoes = 2500;
					break;
				case(4):
					iteracoes = 5000;
					break;
				case(5):
					iteracoes = 10000;
					break;
				}
			} while (iteracoes == 0);
			printf("\nNumero de Iteracoes: %d\n", iteracoes);
			trepa_col(nome_fich, runs, iteracoes);
			break;
		case(2):
			evolutivo(nome_fich, runs);
			break;
		case(3):
			printf("\nAlgoritmo por implementar!\n");
			break;
		case(4):
			printf("A terminar...\n");
			return(EXIT_SUCCESS);

		default:
			printf("Opcao Invalida\n");
			break;
		}
	} while (opcao != 4);


	return (EXIT_SUCCESS);
}
