#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid = fork();
    if (!pid)
    {
        exit(0);
    }
    else
    {
        while (1)
        {
            
        }
    }
}
