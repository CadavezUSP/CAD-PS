#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "mpi.h"


void myReverse(int *s, int inicio, int n) {
	int aux;

	for (int i=inicio; i<n; i++, n--) {
		aux = s[i];
		s[i] = s[n-1];
		s[n-1] = aux;
	}
}

int nextPermutation(int *s, int n) {
	// Find the largest index `i` such that `s[i-1]` is less than `s[i]`
	int i = n-1;
	while (s[i-1] >= s[i])
	{
	// if `i` is the first index of the string, we are already at the last
	// possible permutation (string is sorted in reverse order)
		if (--i == 0) {
		return 0;
		}
	}

	// If we reach here, substring `s[i…n-1]` is sorted in reverse order.
	// Find the highest index `j` to the right of index `i` such that `s[j] > s[i-1]`.
	int  j = n-1;
	while (j > i && s[j] <= s[i-1]) {
		j--;
	}

	// swap character at index `i-1` with index `j`
	// std::swap(s[i-1], s[j]);
	int aux = s[j];
	s[j] = s[i-1];
	s[i-1] = aux;


	// Reverse substring `s[i…n-1]`and return true
	// std::reverse (s.begin() + i, s.end());
	myReverse(s, i, n);


	return 1;
}


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

int *ithPermutation(const int n, int i, unsigned long long *fact) {
   int j, k = 0;
   // int *fact = (int *)calloc(n, sizeof(int));
   int *perm = (int *)calloc(n, sizeof(int));

   // compute factorial numbers
   // fact[k] = 1;
   // while (++k < n)
   //    fact[k] = fact[k - 1] * k;

   // compute factorial code
   for (k = 0; k < n; ++k) {
      perm[k] = i / fact[n - 1 - k];
      i = i % fact[n - 1 - k];
   }

   // readjust values to obtain the permutation
   // start from the end and check if preceding values are lower
   for (k = n - 1; k > 0; --k)
      for (j = k - 1; j >= 0; --j)
         if (perm[j] <= perm[k])
            perm[k]++;


   // free(fact);

   return perm;
}

int calcularCustoPermutacao (int *perm, int **graph, int n, unsigned long long test) {
	int custo = 0;
	int custoAresta;

	for (int i=0; i<n-1; i++) {
		if (perm[i]<0 || perm[i+1]<0) {
			printf("\t%lld", test);
			printf("\t%d %d\n", i, i+1);
			printf("\t%d %d\n\n", perm[i], perm[i+1]);
		}
		// printf("\t%d\n", graph[perm[i]][perm[i+1]]);
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

unsigned long long *calcuarFatorial(int n) {

   unsigned long long *fact = (unsigned long long *)calloc(n, sizeof(unsigned long long));

   int k = 0;

   fact[0] = 1;
   while (++k < n)
      fact[k] = fact[k - 1] * k;

   return fact;
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

	// printar grafo
	// for (int i=0; i<n; i++) {
	// 	for (int j=0; j<n; j++) {
	// 		printf("%d ", graph[i][j]);
	// 	}
	// 	printf("\n");
	// }


	clock_t beginClock = clock();
	clock_t endClock;
	double time_spent = 0.0;


   unsigned long long *fact = calcuarFatorial(n);

	int myrank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);


	unsigned int menorCusto = INT_MAX;
	unsigned int custoAtual;
	int *permutacaoAtual, *menorCaminho=NULL;


	unsigned long long permutacoesPorProcessador = factorial(n)/P;
	unsigned long long permutacaoInicial = permutacoesPorProcessador*myrank;
	unsigned long long permutacaoFinal = permutacoesPorProcessador*(myrank+1);

	permutacaoAtual = ithPermutation(n, permutacaoInicial, fact);

	// Loop para cada combinação do processador atual
	for (unsigned long long i=permutacaoInicial; i<permutacaoFinal; i++) {

		custoAtual = calcularCustoPermutacao (permutacaoAtual, graph, n, i);

		if (custoAtual < menorCusto) {
			menorCusto = custoAtual;
			menorCaminho = permutacaoAtual;
		}

		nextPermutation(permutacaoAtual, n);
	}


	// MPI_Reduce ()
	unsigned int *custos = NULL;
	int *caminhos = NULL;
	if (myrank == 0) {
		custos = (unsigned int *) malloc (P * sizeof(unsigned int));
		caminhos = (int  *) malloc (P * n * sizeof(int));
	}
	// Gather dos custos
	MPI_Gather (&menorCusto, 1, MPI_UNSIGNED, custos, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

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

		printf("\nMENOR CUSTO: %u\nCAMINHO: ", menorCusto);

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
	free(fact);

	return 0;
}
