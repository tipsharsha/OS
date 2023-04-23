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
    int mutex;
    struct request req;
    struct response res;
    int id;
    int space;
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
    key = ftok(".",10);

    // get the shared memory segment created by the server
    shmid = shmget(key, sizeof(struct connect), IPC_CREAT|0666);
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
    char want_continue ='y';
    while(want_continue == 'y')
    {printf("Enter your choice \n 0-Register\n 1-Unregister\n 2-Arithmetic\n 3-IsPrime\n 4-EvenOdd \n 5-ISNEGETIVE\n : ");
    scanf(" %d",&select);
    fflush(stdin);
    switch(select)
    {
        case REGISTER:
            printf("Requesting to register\n");
            while(con->mutex){};
            con->mutex = 1;
            con->reg = REGISTER;
            printf("Request sent to register\n");
            while(con->reg == REGISTER){};
            reg_key =  con->key;
            printf("You are successfully registered. Your Key:   %c\n",reg_key);
            con->mutex =0;
            break;
        case UNREGISTER:
            printf("Provide your key: ");
            scanf(" %c",&reg_key);
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            printf("Connected successfully to comm channel\n");
            comm->req.type = UNREGISTER;
            printf("Request sent to unregister\n");
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
             printf("Unregistration Successful\n");
            break;
        case ARITH:
            printf("Provide your key: ");
            fflush(stdout);
            fflush(stdin);
            scanf(" %c",&reg_key);
            printf("\n");
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            printf("connected successfully to comm channel\n");
            comm->req.type = ARITH;
            printf("Request sent for Arithmetic\n");
            printf("Enter your number for N1 :");
            scanf(" %d",&N1);
            printf("\n");
            comm->req.N1 = N1;
            printf("Enter your Second Number N2 :");
            scanf(" %d",&N2);
            printf("\n");
            comm->req.N2 = N2;
            printf("Operand :");
            scanf(" %c",&operand);
            printf("\n");
            comm->req.operand = operand;
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
            printf("Result %d\n",comm->res.out);
            break;
        case ISPRIME:
           printf("Provide your key: ");
            fflush(stdout);
            fflush(stdin);
            scanf(" %c",&reg_key);
            printf("\n");
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            printf("connected successfully to comm channel\n");
            comm->req.type = ISPRIME;
            printf("Request sent for Prime check\n");
            printf("Enter your number for N1 :");
            scanf(" %d",&N1);
            printf("\n");
            comm->req.N1 = N1;
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
            if(comm->res.out==1)
            {
                printf("It is Prime\n");
            }
            else{
                printf("Not a Prime\n");
            }
            
            break;
        case EVENODD:
            printf("Provide your key: ");
            fflush(stdout);
            fflush(stdin);
            scanf(" %c",&reg_key);
            printf("\n");
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            printf("connected successfully to comm channel\n");
            comm->req.type = EVENODD;
            printf("Request sent for Even check\n");
            printf("Enter your number for N1 :");
            scanf(" %d",&N1);
            printf("\n");
            comm->req.N1 = N1;
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
            printf("Server respoded with success\n");
            if(comm->res.out==1)
            {
                printf("It is Even\n");
            }
            else{
                printf("It is odd\n");
            }
            break;
        case ISNEGETIVE:
            printf("Provide your key: ");
            fflush(stdout);
            fflush(stdin);
            scanf(" %c",&reg_key);
            printf("\n");
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                printf("your key hasn't been registered\n");
                break;
            }
            printf("connected successfully to comm channel\n");
            comm->req.type = ISNEGETIVE;
            printf("Request sent for Negetive check\n");
            printf("Enter your number for N1 :");
            scanf(" %d",&N1);
            printf("\n");
            comm->req.N1 = N1;
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
            if(comm->res.out==1)
            {
                printf("It is Negetive\n");
            }
            else{
                printf("It is positive\n");
            }
            break;
     }
    
        printf("Do want to continue? (y/n): ");
        scanf(" %c",&want_continue);
        printf("\n");
    }

    if (shmdt(con) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}