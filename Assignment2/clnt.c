#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
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
#define PRINT_INFO(MSG, ...) { \
	printf ( "%s INFO %d:%d %ld %s %s %d : " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), __FILE__, __FUNCTION__, \
	__LINE__,  ##__VA_ARGS__); \
}

#define PRINT_ERROR(MSG, ...) { \
	printf ( "%s ERROR %d:%d %ld %s %s %d : [%d] " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), __FILE__, __FUNCTION__, \
	__LINE__,  errno, ##__VA_ARGS__);	\
	}
	
#define PRINT_ERR_EXIT(MSG, ...) { \
	printf ( "%s ERROR %d:%d %ld %s %s %d : [%d] " MSG ";;\n", \
	"TODO_PRINT_TIME", getpid(), getppid(), pthread_self(), __FILE__, __FUNCTION__, \
	__LINE__,  errno, ##__VA_ARGS__);	\
	_exit(-1); \
	}
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
            PRINT_INFO("Requesting to register\n");
            while(con->mutex){};
            con->mutex = 1;
            con->reg = REGISTER;
            PRINT_INFO("Request sent to register\n");
            while(con->reg == REGISTER){};
            reg_key =  con->key;
            PRINT_INFO("You are successfully registered. Your Key: key%c\n",reg_key);
            con->mutex =0;
            break;
        case UNREGISTER:
            PRINT_INFO("Provide your key: ");
            scanf(" key%c",&reg_key);
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                PRINT_INFO("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                PRINT_ERROR("your key hasn't been registered\n");
                break;
            }
            PRINT_INFO("Connected successfully to comm channel\n");
            comm->req.type = UNREGISTER;
            PRINT_INFO("Request sent to unregister\n");
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
             PRINT_INFO("Unregistration Successful\n");
            break;
        case ARITH:
            PRINT_INFO("Provide your key: ");
            fflush(stdout);
            fflush(stdin);
            scanf(" key%c",&reg_key);
            PRINT_INFO("\n");
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                PRINT_ERROR("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                PRINT_ERROR("your key hasn't been registered\n");
                break;
            }
            PRINT_INFO("connected successfully to comm channel\n");
            comm->req.type = ARITH;
            PRINT_INFO("Request sent for Arithmetic\n");
            PRINT_INFO("Enter your number for N1 :");
            scanf(" %d",&N1);
            printf("\n");
            comm->req.N1 = N1;
            PRINT_INFO("Enter your Second Number N2 :");
            scanf(" %d",&N2);
            printf("\n");
            comm->req.N2 = N2;
            PRINT_INFO("Operand :");
            scanf(" %c",&operand);
            printf("\n");
            comm->req.operand = operand;
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
            PRINT_INFO("Result %d\n",comm->res.out);
            break;
        case ISPRIME:
           PRINT_INFO("Provide your key: ");
            fflush(stdout);
            fflush(stdin);
            scanf(" key%c",&reg_key);
            printf("\n");
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                PRINT_INFO("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                PRINT_ERROR("your key hasn't been registered\n");
                break;
            }
            PRINT_INFO("connected successfully to comm channel\n");
            comm->req.type = ISPRIME;
            PRINT_INFO("Request sent for Prime check\n");
            PRINT_INFO("Enter your number for N1 :");
            scanf(" %d",&N1);
            printf("\n");
            comm->req.N1 = N1;
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
            if(comm->res.out==1)
            {
                PRINT_INFO("It is Prime\n");
            }
            else{
                PRINT_INFO("Not a Prime\n");
            }
            
            break;
        case EVENODD:
            PRINT_INFO("Provide your key: ");
            fflush(stdout);
            fflush(stdin);
            scanf(" key%c",&reg_key);
            printf("\n");
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                PRINT_ERROR("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                PRINT_ERROR("your key hasn't been registered\n");
                break;
            }
            PRINT_INFO("connected successfully to comm channel\n");
            comm->req.type = EVENODD;
            PRINT_INFO("Request sent for Even check\n");
            PRINT_INFO("Enter your number for N1 :");
            scanf(" %d",&N1);
            printf("\n");
            comm->req.N1 = N1;
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
            PRINT_INFO("Server responded with success\n");
            if(comm->res.out==1)
            {
                PRINT_INFO("It is Even\n");
            }
            else{
                printf("It is odd\n");
            }
            break;
        case ISNEGETIVE:
            printf("Provide your key: ");
            fflush(stdout);
            fflush(stdin);
            scanf(" key%c",&reg_key);
            printf("\n");
            key_comm = ftok(".",reg_key);
            comm_id = shmget(key_comm,SHM_SIZE, IPC_CREAT | 0666);
            if (comm_id<0)
            {
                PRINT_ERROR("your key hasn't been registered\n");
                break;
            }
            comm = (struct shm_blk*)shmat(comm_id, NULL, 0);
            if (comm == (struct shm_blk*) -1)
            {
                PRINT_ERROR("your key hasn't been registered\n");
                break;
            }
            PRINT_INFO("connected successfully to comm channel\n");
            comm->req.type = ISNEGETIVE;
            PRINT_INFO("Request sent for Negetive check\n");
            PRINT_INFO("Enter your number for N1 :");
            scanf(" %d",&N1);
            printf("\n");
            comm->req.N1 = N1;
            comm->mutex= 0;
            while(comm->res.clnt_res != 1){};
            if(comm->res.out==1)
            {
                PRINT_INFO("It is Negetive\n");
            }
            else{
                PRINT_INFO("It is positive\n");
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