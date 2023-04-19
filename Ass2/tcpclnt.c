#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define SHM_SIZE 1024 // size of shared memory
#define CONNECT_CHANNEL "/connect"
int main()
{
    int fd;
    int *shm;
    // open the shared memory object created by the server
    fd = shm_open(CONNECT_CHANNEL, O_RDWR, 0666);
    if (fd < 0) {
        perror("shm_open");
        exit(1);
    }

    // map the shared memory object into the client's address space
    shm = (int*) mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    int response;
    
    int* mut = shm;
    int* req =shm + 1;
    int* res =req + 1;
    while(*mut){};
    *mut++;
    
    *req = 1;
    //wait till server responds
    while(*req){};
    response = *res;
    // tell the server that the client has finished writing the response
    *mut--;
    printf("Key: %d",response);
    // unmap the shared memory object from the client's address space
    if (munmap(shm, SHM_SIZE) == -1) {
        perror("munmap");
        exit(1);
    }

    // close the shared memory object
    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }

    // return 0;
}