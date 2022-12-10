#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>


unsigned long long factorial (int n) {
	unsigned long long factorial=1;

	for (int i=1; i<=n; i++) {
		factorial *= i;
	}

	return factorial;
}


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


int main () {

	int n = 13;

	int *perm = ithPermutation(n, 0);
	int *ith;

	unsigned long long fac = factorial(n);

	for (int i=1; i<fac; i++) {
		ith = ithPermutation(n, i);
		nextPermutation(perm, n);

		for (int j=0; j<n; j++) {
			printf("%d", ith[j]);
		}
		printf("\n");

		for (int j=0; j<n; j++) {
			printf("%d", perm[j]);
		}
		printf("\n");
		printf("\n");

	}


	return 0;
}