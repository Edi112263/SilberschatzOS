#include  <stdlib.h>
#include  <stdio.h>
#include  <wait.h>
#include  <string.h>
#include  <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include  <unistd.h>

#define buf_size 1024
#define argc_size 10
#define path_size 128
#define WRITE_END 1
#define READ_END 0

void exec_cmd(char* args[argc_size])
{
    int i;
    int fd[2] = {0, 1}; // pentru eventuala folosire a pipe
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        for (i = 0; args[i] != NULL && strcmp(args[i], ">") != 0 && strcmp(args[i], "<") != 0 && strcmp(args[i], "|") != 0; i++);
        if (args[i] != NULL)
        {
            if (strcmp(args[i], "|") == 0)
            {
                /* pipe */
                pid_t pid2;
                args[i] = NULL;
                pipe(fd);
                pid2 = fork();
                if (pid2 == 0)
                {
                    close(fd[READ_END]);
                    dup2(fd[WRITE_END], STDOUT_FILENO);
                    execvp(args[0], args);
                }
                else
                {
                    wait(NULL);
                    close(fd[WRITE_END]);
                    dup2(fd[READ_END], STDIN_FILENO);
                    execvp(args[i+1], args+i+1);
                }
            }
            else
            {
                /* redirectionare output */
                fd = open(args[i+1], O_CREAT | O_RDWR, 0666); // args[i+1] is the path
                if (fd < 0)
                {
                    printf("Cale invalida!\n");
                    exit(-1);
                }
                if (strcmp(args[i], ">") == 0)
                    dup2(fd, STDOUT_FILENO);
                else
                    dup2(fd, STDIN_FILENO);
                
                args[i] = NULL;
            }
            
        }
        if (execvp(args[0], args) < 0)
        {
            printf("Comanda invalida!\n");
            fflush(stdout);
            exit(-1);
        }
        
    }
    else
    {
        wait(NULL);
        if (fd[READ_END] != 0) // daca am folosit pipe, inchidem capatul citirii
            close(fd[READ_END]); 
    }
}

int main()
{
    int run = 1, argc_esh = 0, i;
    char buf[buf_size];
    char* argv_esh[argc_size];
    while (run)
    {
        printf("esh> ");
        fflush(stdout);
        fgets(buf, buf_size, stdin);
        buf[strlen(buf) - 1] = '\0';
        if (strcmp(buf, "exit") == 0)
        {
            run = 0;
        }
        else if (strcmp(buf, "!!") == 0)
        {
            if (argc_esh)
            {
                exec_cmd(argv_esh);
            }
            else
            {
                printf("Nu exista comenzi in istorie!\n");
                fflush(stdout);
            }
        }
        else
        {
           char *p = strtok(buf, " ");
           argc_esh = 0;
           while (p != NULL)
           {
                argv_esh[argc_esh] = strdup(p);
                argc_esh++;
                p = strtok(NULL, " ");
           }
           argv_esh[argc_esh] = NULL;
           exec_cmd(argv_esh);
        }
    }
    return 0;
}
