//Assignment 2 
//Sriharsha Tippavajhala 2021AAPS0717H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>
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
int flag[MAX_CLIENT]={0};//Takes care of how many clients are registered
int to_register[MAX_CLIENT]={0};
int client_requests =0;
pthread_t* tid;
int prchk(int num)//
{
    for(int i=2; i<num; i++)
    {
        if(num%i == 0)
        {
            return 0;
        }
    }
    return 1;
}
int total_clients()
{
    int num=0;
    for(int i =0;i<MAX_CLIENT;i++ )
    {
        if (flag[i])
        {
            num += 1;
        }
    }
    return num;
}
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
            printf("Invalid operation\n");
            result = 0;
    }
    return result;
}
int even_or_odd(int N1) {
    if (N1 % 2 == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}
int is_negetive(int n)
    {if (n<0){return TRUE;}
    return FALSE;
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

void *sum (void *param)
{
    int input;
    while(1)
    {
        if(scanf("%d",&input)){printf("total requests processed %d\n total clients registers %d\n",client_requests,total_clients());}
    }
}

void *thr (void *param)
{   int x;
    int y;
    struct shm_blk* comm =  (struct shm_blk*)param;
    while(TRUE)
        {
            
        comm->res.clnt_res = 0;
        while(comm->mutex){};
        comm->mutex=1;
        if((comm->req).type == UNREGISTER)
        {   comm->res.clnt_res = 1;
            x = comm->id;
            y = comm->space;
            shmdt(comm);
            shmctl(x, IPC_RMID, NULL);
            flag[y] == FALSE;
            printf("Client%d's comm channel has been successfully deleted\n",y);
            break;
        }
        switch((comm->req).type)
        {
            case ARITH: 
                printf("Request for Arith recieved\n");
                (comm->res).out = arithmetic_operation((comm->req).N1,(comm->req).N2,(comm->req).operand);
                break;
            case ISPRIME:
                printf("Request for prime check recieved\n");
                (comm->res).out = prchk((comm->req).N1);
                break;
            case EVENODD:
                printf("Request for evenodd check recieved\n");
                (comm->res).out = even_or_odd((comm->req).N1);
                break;
            case ISNEGETIVE :
                printf("Request for negetive check recieved\n");
                (comm->res).out = is_negetive((comm->req).N1);break;
        }
        printf("Success!The result has been sent on comm channel\n");
        comm->res.clnt_res = 1;
        comm->res.server_res += 1;
        client_requests += 1;
        printf("Requests serviced to this client = %d time\n", comm->res.server_res);
        fflush(stdout);
        printf("Total requests handled by the client %d\n",client_requests);
        }
        printf("thread %d ended after registration\n",y);
        pthread_exit(0);
}

//Function to search first space available in buffer
int first_available()
{ 
    for(int i=0;i<MAX_CLIENT;i++)
    {   
        if(flag[i] == FALSE)
        {
            return i;
        }
    }
    return -1;
}
//Arrays Global
//Keys for the registered client
//For the functionality

//Register a client
int register_client(struct connect* con)
{
    
    int space = first_available();
    if (space != -1 && space< MAX_CLIENT)
    {
        flag[space] = TRUE;
    }
    else{
        return 0;
        //BUFFER FULL
    }
    //Generate key
    
    con->key = 33+space;
    key_t key_comm = ftok(".",con->key);
    printf("created thread with key %c\n",con->key);
    int id = shmget(key_comm,SHM_SIZE,IPC_CREAT | 0666);
    if (id < 0) 
    {
        perror("Error in making comm id");
        exit(1);
    }
    struct shm_blk* comm = (struct shm_blk*)shmat(id, NULL, 0);
    if (comm == (struct shm_blk*) -1)
    {
        perror("Communication channel pointer error");
        exit(1);
    }
    printf("Comm channel %d successfully created\n", space);
    comm->mutex = 1;
    comm->id = id;
    comm->space = space;
    comm->res.server_res =0;
    pthread_create(tid+space,NULL,thr,comm);
    con->reg = IDLE;//tell it's registration is complete and get lost
    printf("Client %d has been Registered\n",space);
    return 1;
}
//Main Function
int main()
{
    int shmid;
    key_t key_connect;
    struct connect* con;
    pthread_t p;
    tid = (pthread_t*)malloc(MAX_CLIENT*sizeof(pthread_t));
    //CONNECT CHANNEL KEY
    key_connect = ftok(".",10);
    // create the shared memory segment with read/write permission
    shmid = shmget(key_connect, sizeof(struct connect), IPC_CREAT | 0666);
    if (shmid < 0) 
    {
        perror("shmget");
        exit(1);
    }
    printf("Connect Channel Successfully created\n");
    con = (struct connect*) shmat(shmid, NULL, 0);
    if (con == (struct connect*) -1)
    {
        perror("shmat");
        exit(1);
    }
    con->mutex = 0;
    con->reg = IDLE;
    //thread to give info
    pthread_create(&p,NULL,sum,NULL);
    printf("\nFor summary press anything\n");
    while(TRUE)//Put something so that it will stop at some point
        {
            if(con->reg == REGISTER && first_available() != -1)
        {
            printf("Register initiated\n");
            register_client(con);
            con->reg = IDLE;
        }
        else if(first_available() == -1)
        {
            printf("Max clients reached");
        }
        }
    // detach the shared memory segment from the server's address space
    if (shmdt(con) == -1)
     {
        perror("shmdt");
        exit(1);
    }
    // delete the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) 
    {
        perror("shmctl");
        exit(1);
    }
    for(int i = 0;i<MAX_CLIENT;i++)
    {
    pthread_join(tid[i],NULL);
    return 0;
    }
    free(tid);
}