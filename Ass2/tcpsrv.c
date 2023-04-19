#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define SHM_SIZE 1024 // size of shared memory
#define CONNECT_CHANNEL "/connect"
int arithmetic_operation(int N1, int N2, char Operation) {
    int result;
    switch(Operation) {
        case '+':
            result = N1 + N2;
            break;
        case '-':
            result = N1 - N2;
            break;
        case '*':
            result = N1 * N2;
            break;
        case '/':
            result = N1 / N2;
            break;
        default:
            printf("Invalid operation");
            result = 0;
    }
    return result;
}
char* even_or_odd(int N1) {
    if (N1 % 2 == 0) {
        return "Even";
    } else {
        return "Odd";
    }
}
int main()
{
    int fd;
    int *shm, *s;

    // create the shared memory object
    fd = shm_open(CONNECT_CHANNEL, O_CREAT | O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        exit(1);
    }

    // configure the size of the shared memory object
    ftruncate(fd, SHM_SIZE);

    // map the shared memory object into the server's address space
    shm = (int*) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // write a message to the shared memory for the client to read
    int* mut = shm;
    *(mut)=0;
    int* req =shm + 1;
    int* res =req + 1;
    *res = 0;
    while(1){
    if((*req) == 1)
    {
        
        *res += 1;
        //pthread_create();//create nth pthread
        sleep(1);
        *req= 0;
    }
    }

    // unmap the shared memory object from the server's address space
    if (munmap(shm, SHM_SIZE) == -1) {
        perror("munmap");
        exit(1);
    }

    // close the shared memory object
    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }

    // delete the shared memory object
    if (shm_unlink(CONNECT_CHANNEL) == -1) {
        perror("shm_unlink");
        exit(1);
    }

    return 0;
}
