#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>

int main(void)
{
    pid_t pid;
    int   status;
    
    pid = fork();
    
    if (pid < 0)
        printf("Nu am reusit sa creez un proces. :(\n");
    else if (pid)
    {
        printf("Am creat procesul %d care va executa programul ls\n", pid);
        wait(&status);
        printf("Am executat cu succes, cu statusul %d\n", status);
    }
    else
    {
        execlp("ls", "ls", "-l", (char *) NULL);
        exit(420);
    }
        
    return 0;
    
}
