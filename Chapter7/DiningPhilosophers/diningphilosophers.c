/* 
 * Program ce rezolva problema "Dining Philosophers".
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NR_FILOSOFI 5
#define TRUE 1

pthread_t filosofi[NR_FILOSOFI]; // thread-urile filosofilor

int furculite[NR_FILOSOFI]; // 1 -> disponibila, 0 -> preluata

int id_filosofi[NR_FILOSOFI]; // id-urile filosofilor pentru transmiterea parametrilor functiei work

pthread_mutex_t mutex; // mutex pentru modificarea vectorului de furculite

pthread_cond_t cond_var[NR_FILOSOFI]; // variabile conditionale pentru fiecare furculita

void mananca(int id_filosof)
{
    printf("EU sunt filosoful no. %d si acum mananc...\n", id_filosof);
    sleep(6);
}

void filosofeaza(int id_filosof)
{
   printf("EU sunt filosoful no. %d si acum filosofez...\n", id_filosof);
   sleep(10); 
}

void ia_furculite(int id_filosof)
{
    int stanga = id_filosof;
    int dreapta = (id_filosof+1) % NR_FILOSOFI;
    
    pthread_mutex_lock(&mutex); // preia lacatul
    
    while (furculite[stanga] == 0 || furculite[dreapta] == 0) // cat timp nu sunt ambele furculite disponibile
    {
        if (furculite[stanga] == 0) //verifica daca furculita din stanga este ocupata
            pthread_cond_wait(&cond_var[stanga], &mutex); // asteapta pana cand se elibereaza
        if (furculite[dreapta] == 0) // verifica daca si cea din dreapta nu este disponibila
            pthread_cond_wait(&cond_var[dreapta], &mutex);  // asteapta pana cand se elibereaza
    }
    //marcheaza furculitele ca fiind preluate
    furculite[stanga] = 0; 
    furculite[dreapta] = 0;
    
    pthread_mutex_unlock(&mutex); // elibereaza lacatul    
    
}

void lasa_furculite(int id_filosof)
{
    int stanga = id_filosof;
    int dreapta = (id_filosof+1) % NR_FILOSOFI;
    
    pthread_mutex_lock(&mutex);
    
    //marcheaza furculitele ca fiind disponibile
    furculite[stanga] = 1;
    furculite[dreapta] = 1;
    
    //trimite semnale catre alti filosofi care vor sa se foloseasca de furculite
    pthread_cond_signal(&cond_var[stanga]);
    pthread_cond_signal(&cond_var[dreapta]); 
    
    pthread_mutex_unlock(&mutex);
}

void* work(void* p)
{
    int id_filosof = *(int*)p;
    while (TRUE)
    {
        ia_furculite(id_filosof);
        mananca(id_filosof);
        lasa_furculite(id_filosof);
        filosofeaza(id_filosof);
    }
}

void init_filosofi()
{
    pthread_mutex_init(&mutex, NULL); // initializeaza mutex-ul
    
    for (int i = 0; i < NR_FILOSOFI; i++)
    {
        pthread_cond_init(&cond_var[i], NULL); // initializeaza variabilele de conditie
        furculite[i] = 1; // fiecare furculita este disponibila la inceput
        id_filosofi[i] = i;
    }
    for (int i = 0; i < NR_FILOSOFI; i++)
    {
        pthread_create(&filosofi[i], NULL, work, &id_filosofi[i]); // creeaza cate un thread pt fiecare filosof
    }
        
}

void shutdown_filosofi()
{
    for (int i = 0; i < NR_FILOSOFI; i++)
        pthread_join(filosofi[i], NULL);
}

int main()
{
    init_filosofi();
    shutdown_filosofi();
    return 0;
}
