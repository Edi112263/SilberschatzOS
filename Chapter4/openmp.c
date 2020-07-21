#include <stdio.h>
#include <omp.h>

#define N 5

int main(int argc, char* argv[])
{
    int a[N] = {1,2,3,4,5};
    int b[N] = {5,6,7,8,9};
    int c[N], i;
    
    #pragma omp parallel for
    for (i = 0; i < N; i++)
        c[i] = a[i] + b[i];
    
    #pragma omp parallel for
    for (i = 0; i < N; i++)
        printf("%d ", c[i]);
    printf("\n");
    
    return 0;
}
