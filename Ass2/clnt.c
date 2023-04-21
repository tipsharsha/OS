#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <string.h>
#define SHM_SIZE 1024
#define TRUE 1
#define FALSE 0
#define MAX_CLIENT 10
#define SHM_SIZE 1024
#define UNREGISTER 1
#define REGISTER 0
#define CONNECT_SIZE 32
#define ARITH 2
#define ISPRIME 3
#define EVENODD 4
#define ISNEGETIVE 5
#define IDLE 6
struct request{
    int type;
    int N1;
    int N2;
    char operand;
};
struct response{
    int response_code;
    int clnt_res;
    int server_res;
    int out; 
};
struct shm_blk// struct made for blk requested
{
    pthread_mutex_t mutex;
    struct request req;
    struct response res;
};

struct connect
{
    int mutex;
    int reg;
    char key;
};
// struct made for blk requested
int main()
{
    int shmid,select;
    key_t key;
    struct connect* con;
    
    // generate the same key used by the server for the shared memory segment
    key = ftok(".", 'C');

    // get the shared memory segment created by the server
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    // attach the shared memory segment to the client's address space
    con = (struct connect*) shmat(shmid, NULL, 0);
    if (con == (struct connect*) -1) {
        perror("shmat");
        exit(1);
    }
    char reg_key;
    key_t key_comm;
    int comm_id;
    int N1,N2;
    char operand;
    struct shm_blk* comm;
    while(TRUE)
    {printf("Enter your choice \n 0-Register\n 1-Unregister\n 2-Arithmetic\n 3-IsPrime\n 4-EvenOdd \n 5-ISNEGETIVE");
    scanf("%d",&select);
    switch(select)
    {
        case REGISTER:
            while(con->mutex){};
            con->mutex = 1;
            con->reg = 1;
            while(con->reg == REGISTER){};
            key_comm =  con->key;
            printf("You are successfully registered Your Key:%c",key_comm);
        case UNREGISTER:
            printf("Provide your key: ");
            scanf("%c",&reg_key);
            key_comm = ftok(".", reg_key);
            comm_id = shmget(key_comm, CONNECT_SIZE, IPC_CREAT | 0666);
            comm = (struct shm_blk*)shmat(shmid, NULL, 0);
            comm->req.type = UNREGISTER;
            while(TRUE)
            {
                if(comm->res.clnt_res == 1)
                {printf("Unregistration Successful");
                }
            }
        case ARITH:
            printf("Provide your key: ");
            scanf("%c",&reg_key);
            key_comm = ftok(".", reg_key);
            comm_id = shmget(key_comm, CONNECT_SIZE, IPC_CREAT | 0666);
            comm = (struct shm_blk*)shmat(shmid, NULL, 0);
            comm->req.type = ARITH;
            printf("Enter your number for N1");
            scanf("%d",&N1);
            comm->req.N1 = N1;
            printf("Enter your Second Number N2\n");
            scanf("%d",&N2);
            comm->req.N2 = N2;
            printf("Operand");
            scanf("%c",&operand);
            comm->req.operand = operand;
            while(TRUE)
            {
                if(comm->res.clnt_res == 1)
                {
                    printf("Output %d",comm->res.out);
                break;
                }
            }
        case ISPRIME:
           printf("Provide your key: ");
            scanf("%c",&reg_key);
            key_comm = ftok(".", reg_key);
            comm_id = shmget(key_comm, CONNECT_SIZE, IPC_CREAT | 0666);
            comm = (struct shm_blk*)shmat(shmid, NULL, 0);
            comm->req.type = ISPRIME;
            printf("Enter your number for N1");
            scanf("%d",&N1);
            comm->req.N1 = N1;
            comm->req.operand = operand;
            while(TRUE)
            {
                if(comm->res.clnt_res == 1)
                {
                    printf("Output %d",comm->res.out);
                break;
                }
            }
        case EVENODD:
            printf("Provide your key: ");
            scanf("%c",&reg_key);
            key_comm = ftok(".", reg_key);
            comm_id = shmget(key_comm, CONNECT_SIZE, IPC_CREAT | 0666);
            comm = (struct shm_blk*)shmat(shmid, NULL, 0);
            comm->req.type = EVENODD;
            printf("Enter your number for N1");
            scanf("%d",&N1);
            comm->req.N1 = N1;
            comm->req.operand = operand;
            while(TRUE)
            {
                if(comm->res.clnt_res == 1)
                {
                    printf("Output %d",comm->res.out);
                break;
                }
            }
        case ISNEGETIVE:
            printf("Provide your key: ");
            scanf("%c",&reg_key);
            key_comm = ftok(".", reg_key);
            comm_id = shmget(key_comm, CONNECT_SIZE, IPC_CREAT | 0666);
            comm = (struct shm_blk*)shmat(shmid, NULL, 0);
            comm->req.type = ISNEGETIVE;
            printf("Enter your number for N1");
            scanf("%d",&N1);
            comm->req.N1 = N1;
            comm->req.operand = operand;
            while(TRUE)
            {
                if(comm->res.clnt_res == 1)
                {
                    printf("Output %d",comm->res.out);
                break;
                }
            }
    }
    }

    // read the message from the shared memory and write a response
    

    // tell the server that the client has finished writing the response
    // detach the shared memory segment from the client's address space
    if (shmdt(con) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}