#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define nume "zona"
#define marime sizeof(struct timeval)
#define READEND 0
#define WRITEEND 1

/*
Varianta 1 - cu memorie comuna 

int main(int argc, char* argv[])
{
    struct timeval current, *ptr;
    pid_t pid;
    int fd;
    
    
    pid = fork();
    if (!pid)
    {
        // child
        gettimeofday(&current, NULL);
        fd = shm_open(nume, O_CREAT | O_RDWR, 0666);
        ftruncate(fd, marime);
        ptr = (struct timeval*) mmap(NULL, marime, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        *ptr = current;
        
        execvp(argv[1], argv+1);
    }
    else
    {
        // parent
        struct timeval rez;
        wait(NULL);
        gettimeofday(&current, NULL);
        fd = shm_open(nume, O_RDONLY, 0666);
        ptr = (struct timeval*) mmap(NULL, marime, PROT_READ, MAP_SHARED, fd, 0);
        timersub(&current, ptr, &rez);
        printf("Timp de executie: %lf\n", rez.tv_usec * 0.000001 + rez.tv_sec);
        shm_unlink(nume);
    }
}

*/

/* Varianta 2 - cu pipe */
int main(int argc, char* argv[])
{
    struct timeval current;
    pid_t pid;
    int fd[2];
    
    pipe(fd);
    pid = fork();
    if (!pid)
    {
        // child
        gettimeofday(&current, NULL);
        close(fd[READEND]);
        write(fd[WRITEEND], &current, marime);
        close(fd[WRITEEND]);
        execvp(argv[1], argv+1);
    }
    else
    {
        // parent
        struct timeval val, rez;
        wait(NULL);
        gettimeofday(&current, NULL);
        close(fd[WRITEEND]);
        read(fd[READEND], &val, marime);
        close(fd[READEND]);
        timersub(&current, &val, &rez);
        printf("Timp de executie: %lf\n", rez.tv_usec * 0.000001 + rez.tv_sec);
    }
}
