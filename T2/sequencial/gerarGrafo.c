#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (int argc, char *argv[]) {

	if (argc != 2) {
		printf("Erro\nFaltou argumento\n");
		return -1;
	}

	srand (time(NULL));

	int n = atoi (argv[1]);

	int graph[n][n];

	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {

			if (j==i) {
				graph[i][j] = 0;
			}
			else {
				graph[i][j] = rand() % 100;
			}

		}
	}

	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++) {
			printf("%d ", graph[i][j]);
		}
		printf("\n");
	}

	return 0;

}
