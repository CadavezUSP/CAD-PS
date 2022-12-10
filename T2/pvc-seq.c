#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define V 4


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


// Function to find the minimum weight Hamiltonian Cycle
void tsp(int **graph, char *v, int currPos, int n, int count, int cost, int *ans, int *path, int *bestPath) {

	// If last node is reached and it has a link
	// to the starting node i.e the source then
	// keep the minimum value out of the total cost
	// of traversal and "ans"
	// Finally return to check for more possible values
	if (count == n && graph[currPos][path[0]]) {
		if (cost + graph[currPos][path[0]] < *ans) {
			// atualiza bestPath
			for (int i=0; i<n; i++) {
				bestPath[i] = path[i];
			}

			*ans = cost + graph[currPos][0];
		}
		return;
	}

	// BACKTRACKING STEP
	// Loop to traverse the adjacency list
	// of currPos node and increasing the count
	// by 1 and cost by graph[currPos][i] value
	for (int i = 0; i < n; i++) {
		if (!v[i] && graph[currPos][i] && graph[currPos][i]<INT_MAX) {

			// Mark as visited
			v[i] = 1;

			path[count] = i;

			tsp(graph, v, i, n, count + 1, cost + graph[currPos][i], ans, path, bestPath);


			// Mark ith node as unvisited
			v[i] = 0;
			// path[] = -1;
		}
	}
};

// Driver code
int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Error\nMissing arguments\n");
		return -1;
	}

	// n is the number of nodes i.e. V
	int n = atoi (argv[1]);

	int **graph = constructGraph(n);

	clock_t beginClock = clock();
	clock_t endClock;
	double time_spent = 0.0;

	// Boolean array to check if a node
	// has been visited or not
	char v[n];
	for (int i = 0; i < n; i++)
		v[i] = 0;

	int path[n], bestPath[n];
	int ans = INT_MAX;

	for (int i=0; i<n; i++) {

		// Mark ith node as visited
		v[i] = 1;

		if(i!=0)
			v[i-1] = 0;

		path[0] = i;

		// Find the minimum weight Hamiltonian Cycle
		tsp(graph, v, i, n, 1, 0, &ans, path, bestPath);
	}

	endClock = clock();

	// ans is the minimum weight Hamiltonian Cycle
	printf("ans: %d\n", ans);

	for (int i=0; i<n; i++) {
		printf("%d ", bestPath[i]);
	}

	printf("\n");

	printAndFreeGraph(graph, n);

	time_spent += (double)(endClock - beginClock) / CLOCKS_PER_SEC;
	printf("\n\n\tTEMPO: %lf\n", time_spent);

	return 0;
}
