#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
using namespace std;
#define SHM_SIZE 1024 // size of shared memory
    struct connect
{
    int mutex;
    int reg;
    char key;
};
int main()
{
    int shmid;
    struct connect*shm;
    shmid = shmget(13, 32 , 0666);
    shm = (struct connect*) shmat(shmid, NULL, 0);
    printf("%d\n",shm->mutex);
    printf("%d\n",shm->reg);
    printf("%c\n",shm->key);
    while (shm->mutex)
    {
        /* code */
    };
    shm->reg = 0;
    while (shm->reg == 0)
    {
        /* code */
    };
    cout<<shm->key<<endl;
    shmdt(shm);
    return 0;
}

