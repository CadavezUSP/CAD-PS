#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "mpi.h"


int **constructGraph(int n) {

	// srand (time(NULL));

	int **graph = (int **) malloc (n * sizeof(int *));

	for (int i=0; i<n; i++) {
		graph[i] = (int *) malloc (n* sizeof(int));

		for (int j=0; j<n; j++) {

			if (j==i) {
				graph[i][j] = 0;
			}
			else {
				graph[i][j] = rand() % 100;
			}

		}
	}

	return graph;
}

void printAndFreeGraph (int **graph, int n) {

	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			printf("%d ", graph[i][j]);
		}
		printf("\n");
		free (graph[i]);
	}

	free(graph);
}


unsigned long long factorial (int n) {
	unsigned long long factorial=1;

	for (int i=1; i<=n; i++) {
		factorial *= i;
	}

	return factorial;
}

int *ithPermutation(const int n, int i) {
   int j, k = 0;
   int *fact = (int *)calloc(n, sizeof(int));
   int *perm = (int *)calloc(n, sizeof(int));

   // compute factorial numbers
   fact[k] = 1;
   while (++k < n)
      fact[k] = fact[k - 1] * k;

   // compute factorial code
   for (k = 0; k < n; ++k)
   {
      perm[k] = i / fact[n - 1 - k];
      i = i % fact[n - 1 - k];
   }

   // readjust values to obtain the permutation
   // start from the end and check if preceding values are lower
   for (k = n - 1; k > 0; --k)
      for (j = k - 1; j >= 0; --j)
         if (perm[j] <= perm[k])
            perm[k]++;


   free(fact);

   return perm;
}

int calcularCustoPermutacao (int *perm, int **graph, int n) {
	int custo = 0;
	int custoAresta;

	for (int i=0; i<n-1; i++) {
		custoAresta = graph[perm[i]][perm[i+1]];

		if (custoAresta != 0 && custoAresta != INT_MAX)
			custo += custoAresta;

		else
			return INT_MAX;

	}

	custoAresta = graph[perm[n-1]][perm[0]];
	if (custoAresta != 0 && custoAresta != INT_MAX)
		return custo + custoAresta;
	else
		return INT_MAX;

}



// Driver code
int main(int argc, char *argv[]) {

	if (argc < 2) {
		printf("Error\nMissing arguments\n");
		return -1;
	}

	// n is the number of nodes i.e. V
	int n = atoi (argv[1]);

	int P; // numero de processadores

	int **graph = constructGraph(n);

	clock_t beginClock = clock();
	clock_t endClock;
	double time_spent = 0.0;

	int myrank;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


	unsigned long long menorCusto = INT_MAX;
	unsigned long long custoAtual;
	int *permutacaoAtual, *menorCaminho=NULL;


	unsigned long long permutacoesPorProcessador = factorial(n)/P;
	unsigned long long permutacaoInicial = permutacoesPorProcessador*myrank;
	unsigned long long permutacaoFinal = permutacoesPorProcessador*(myrank+1);


	// Loop para cada combinação do processador atual
	for (int i=permutacaoInicial; i<permutacaoFinal; i++) {
		permutacaoAtual = ithPermutation(n, i);

	// for (int j=0; j<n; j++) {
	// 	printf("%d ", permutacaoAtual[j]);
	// }
	// printf("\n");

		custoAtual = calcularCustoPermutacao (permutacaoAtual, graph, n);

		if (custoAtual < menorCusto) {
			menorCusto = custoAtual;
			free (menorCaminho);
			menorCaminho = permutacaoAtual;
		}

		else 
			free (permutacaoAtual);
	}


	// MPI_Reduce ()
	unsigned long long *custos = NULL;
	int *caminhos = NULL;
	if (myrank == 0) {
		custos = (unsigned long long *) malloc (P * sizeof(unsigned long long));
		caminhos = (int  *) malloc (P * n * sizeof(int));
	}
	// Gather dos custos
	MPI_Gather (&menorCusto, 1, MPI_UNSIGNED_LONG_LONG, custos, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_COMM_WORLD);

	// Gather dos caminhos
	MPI_Gather (menorCaminho, n, MPI_INT, caminhos, n, MPI_INT, 0, MPI_COMM_WORLD);

	int melhorRank = 0;

	if (myrank == 0) {
		for (int i=0; i<P; i++) {
			if (custos[i] < menorCusto) {
				menorCusto = custos[i];
				melhorRank = i;
			}
		}

		endClock = clock();

		printf("\nMENOR CUSTO: %lld\nCAMINHO: ", menorCusto);

		for (int i=0; i<n; i++) {
			menorCaminho[i] = caminhos[ (melhorRank*n) + i ];
			printf("%d ", menorCaminho[i]);
		}

		printf("%d\n", menorCaminho[0]);

		free(custos);
		free(caminhos);

		printAndFreeGraph(graph, n);

		time_spent += (double)(endClock - beginClock) / CLOCKS_PER_SEC;
		printf("\n\n\tTEMPO: %lfs\n", time_spent);
	}

	MPI_Finalize();

	free(menorCaminho);


	return 0;
}
