#include <pthread.h>
#include <stdio.h>


void* callme(void* p)
{
   printf("In functie!\n");
   sleep(2);
   pthread_exit(NULL);
}


void main()
{
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    pthread_t thread4;
    pthread_t thread5;


    pthread_create(&thread1, NULL, callme, NULL);
    pthread_create(&thread2, NULL, callme, NULL);
    pthread_create(&thread3, NULL, callme, NULL);
    pthread_create(&thread4, NULL, callme, NULL);
    pthread_create(&thread5, NULL, callme, NULL);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_join(thread4, NULL);
        pthread_join(thread5, NULL);   

} 
