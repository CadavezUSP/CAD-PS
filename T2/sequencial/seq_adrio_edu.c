#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define V 4


// Function to find the minimum weight Hamiltonian Cycle
void tsp(int graph[][V], char *v, int currPos, int n, int count, int cost, int *ans, int *path, int *bestPath) {

	// If last node is reached and it has a link
	// to the starting node i.e the source then
	// keep the minimum value out of the total cost
	// of traversal and "ans"
	// Finally return to check for more possible values
	if (count == n && graph[currPos][0]) {
		if (cost + graph[currPos][0] < *ans) {
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
	// n is the number of nodes i.e. V
	int n = 4;

	int graph[][V] = {
		{ 0, 24, 15, 27 },
		{ 24, 0, 23, 14 },
		{ 15, 23, 0, 18 },
		{ 27, 14, 18, 0 }
	};

	// Boolean array to check if a node
	// has been visited or not
	char v[n];
	for (int i = 0; i < n; i++)
		v[i] = 0;

	// Mark 0th node as visited
	v[0] = 1;
	int ans = INT_MAX;

	int index = 0;
	int path[n], bestPath[n];

	path[0] = 0;

	// Find the minimum weight Hamiltonian Cycle
	tsp(graph, v, 0, n, 1, 0, &ans, path, bestPath);

	// ans is the minimum weight Hamiltonian Cycle
	printf("ans: %d\n", ans);

	for (int i=0; i<n; i++) {
		printf("%d ", bestPath[i]);
	}

	printf("\n");

	return 0;
}
