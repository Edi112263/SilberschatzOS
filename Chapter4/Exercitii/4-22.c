#include <pthread.h>
#include <stdio.h>


int v[50], n, min, max;
float med;

void* medie(void* p)
{
    int s = 0;
    for (int i = 0; i < n; i++)
        s += v[i];
    med = (float) s / n;
    
    pthread_exit(0);
}

void* maxim(void* p)
{
    max = v[0];
    for (int i = 1; i < n; i++)
        if (v[i] > max)
            max = v[i];
        
   pthread_exit(0);     
}

void* minim(void* p)
{
    min = v[0];
    for (int i = 1; i < n; i++)
        if (v[i] < min)
            min = v[i];
        
    pthread_exit(0);
}

int main(int argc, char* argv[])
{
    pthread_t workers[3];
    pthread_attr_t attr;
    int i;
    
    printf("n = ");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
        scanf("%d", &v[i]);
    
    pthread_attr_init(&attr);
    pthread_create(&workers[0], &attr, medie, NULL);
    pthread_create(&workers[1], &attr, maxim, NULL);
    pthread_create(&workers[2], &attr, minim, NULL);
    
    for (i = 0; i < 3; i++)
        pthread_join(workers[i], NULL);
    
    printf("Media: %f\n", med);
    printf("Minim: %d\n", min);
    printf("Maxim: %d\n", max);
    
    return 0;
            
}
