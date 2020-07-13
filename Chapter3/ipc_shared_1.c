#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    const int SIZE = 4096;
    const char *name = "zona";
    const char *message1 = "Hello";
    const char *message2 = "World!";
    
    int fd;
    char *ptr;
    
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);
    
    ptr = (char *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    
    sprintf(ptr, "%s", message1);
    ptr += strlen(message1);
    sprintf(ptr, "%s", message2);
    ptr += strlen(message2);
    return 0;
}
