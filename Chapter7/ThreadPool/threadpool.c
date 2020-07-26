/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 16

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
struct task
{
    void (*function)(void *p);
    void *data;
};


// coada implementata folosind o lista circulara
struct coada
{
    struct task ts;
    struct coada* next;
};

struct coada* ultim = NULL;


// the worker bee
pthread_t workers[NUMBER_OF_THREADS];

// semafor pentru munca disponibila
sem_t sem;

// mutex pentru operatiile push si pop din coada
pthread_mutex_t mutex;

int empty()
{
    if (ultim != NULL)
        return 1;
    
    return 0;
}

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(struct task t) 
{
    if (ultim != NULL)
    {
        struct coada* aux;
        aux = (struct coada*) malloc(sizeof(struct coada));
        aux->ts = t;
        aux->next = ultim->next; // ultim->next e un pointer catre primul element din coada
        ultim->next = aux;
        ultim = aux;
        
    }
    else
    {
        ultim = (struct coada*) malloc(sizeof(struct coada));
        ultim->ts = t;
        ultim->next = ultim;
    }
    
    sem_post(&sem); // semnalam faptul ca am introdus o sarcina in coada
    return 0;
}

// remove a task from the queue
struct task dequeue() 
{
    if (ultim != NULL)
    {
        struct coada front = *(ultim->next);
        free(ultim->next);
        if (ultim->next == ultim) // daca coada a avut un singur element
            ultim = NULL;
        else
            ultim->next = front.next;
        
        return front.ts;
    }
    exit(-1);
    //return worktodo;
}


// the worker thread in the thread pool
void *worker(void *param)
{
    while (TRUE)
    {
        struct task worktodo;
        
        sem_wait(&sem); // asteapta pt munca
            
        pthread_mutex_lock(&mutex); // acquire lock
        worktodo = dequeue();
        pthread_mutex_unlock(&mutex); // release lock
        
        // execute the task
        execute(worktodo.function, worktodo.data);

        //pthread_exit(0);
    }
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    struct task t;
    t.function = somefunction;
    t.data = p;
    
    pthread_mutex_lock(&mutex); // acquire lock
    enqueue(t); // adauga sarcina in coada
    pthread_mutex_unlock(&mutex); //release lock
    

    return 0;
}

// initialize the thread pool
void pool_init(void)
{
    sem_init(&sem, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_create(&workers[i], NULL, worker, NULL);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    for (int i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(workers[i], NULL);
    
    sem_destroy(&sem);
    pthread_mutex_destroy(&mutex);
    
}
