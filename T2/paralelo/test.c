#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>


int *ithPermutation(const int n, int i, int espacos) {
   int j, k = 0;
   // int *fact = (int *)calloc(n, sizeof(int));
   int *perm = (int *)calloc(n, sizeof(int));

   // compute factorial numbers
   int x[espacos];

   for (k=0; k<espacos; k++) {
      x[k] = 1;
   }

   for (k=1; k<espacos; k++) {
      for (j=espacos-k; j<espacos; j++) {
         x[j] *= n-k;
      }
   }

   // compute factorial code
   for (k = 0; k < espacos; ++k)
   {
      perm[k] = i / x[espacos-1 - k];
      i = i % x[espacos-1 - k];
   }

   // readjust values to obtain the permutation
   // start from the end and check if preceding values are lower
   for (k = espacos - 1; k > 0; --k)
      for (j = k - 1; j >= 0; --j)
         if (perm[j] <= perm[k])
            perm[k]++;


   return perm;
}

int main () {

   int *permutacao;

   int n=4;


   for (int i =0; i<24; i++) {

      permutacao = ithPermutation(n, i, 4);

      for (int j=0; j<4; j++) {
         printf("%d ", permutacao[j]);

      }
      printf("\n");
   }

   return 0;
}