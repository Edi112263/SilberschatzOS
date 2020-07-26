/**
 * Example client program that uses thread pool.
 */

#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;
    temp = (struct data*)param;

    printf("I add two values %d and %d result = %d\n",temp->a, temp->b, temp->a + temp->b);
}

void test_func(void *param)
{
    printf("Dorm 2 secunde\n");
    sleep(2);
}

int main(void)
{
    // create some work to do
    struct data work[8];
    work[0].a = 5;
    work[0].b = 10;

    // initialize the thread pool
    pool_init();
    

    // submit the work to the queue
    for (int i = 0; i < 16; i++)
    {
        //work[i].a = 5 + i;
        //work[i].b = 10 + i;
        pool_submit(&test_func, NULL);
    }
    printf("Am iesit!\n");
    
    
    //sleep(2);
    
    pool_submit(&add,&work);
    pool_submit(&add,&work);
    pool_submit(&add,&work);
    pool_submit(&add,&work);
    pool_submit(&add,&work);
    pool_submit(&add,&work);

    // may be helpful 
    //sleep(3);

    pool_shutdown();

    return 0;
}
