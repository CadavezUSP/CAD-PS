#include <stdio.h>
#include <time.h>
#include <omp.h>

int main ()
{
    int A [] = {84, 30, 95, 94, 36, 73, 52, 23, 2, 13};
    int S [10] = {0};
#pragma omp parallel
{
    #pragma omp for reduction(+:S[:10])
    for (int n=0 ; n<10 ; ++n ) {
        for (int m=0; m<=n; ++m){
            S[n] += A[m];
        }
    }
    // #pragma omp critical
    // {
    //     for(int n=0; n<10; ++n) {
    //         S[n] += S_private[n];
    //     }
    // }
}
 for(int n=0; n<10; ++n) {
            printf("%d\n", S[n]);
        }

}
