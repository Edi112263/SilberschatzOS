#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUF_SIZE 4096

int main(int argc, char* argv[])
{
    int fd1, fd2, in, out;
    char buf[BUF_SIZE];
    if (argc != 3) exit(-1);
    fd1 = open(argv[1], O_RDONLY);
    fd2 = creat(argv[2], 0666);
    while (1)
    {
        in = read(fd1, buf, BUF_SIZE);
        if (in <= 0)
            break;
        
        out = write(fd2, buf, in);
        if (out <= 0)
            break;
    }
    close(fd1);
    close(fd2);
    exit(0);
}
