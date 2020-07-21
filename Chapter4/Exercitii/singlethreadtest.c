#include <stdio.h>

void callme()
{
    printf("In functie!\n");
    sleep(2);
}

void main()
{
    callme();
    callme();
    callme();
    callme();
    callme();
} 
