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
#define TEST cout<<"TEST";
int flag[MAX_CLIENT]={0};//Takes care of how many clients are registered
int to_register[MAX_CLIENT]={0};
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
};

struct connect
{
    int mutex;
    int reg;
    char key;
};

void *thr (void *param)
{
    cout<<"thread has been created"<<endl;
    struct shm_blk* comm =  (struct shm_blk*)param;
    cout<<comm->mutex<<endl;
    while(TRUE)
        {
        comm->res.clnt_res = 0;
        cout<<"it is locked"<<endl;
        cout<<comm->id<<endl;
        while(comm->mutex){};
        cout<<"it got unlocked";
        comm->mutex=1;
        switch((comm->req).type)
        {   
            case UNREGISTER:
                shmdt(comm);
                shmctl(comm->id, IPC_RMID, NULL);
                break;

            case ARITH: 
                cout<<"tried aarith"<<endl;
                (comm->res).out = arithmetic_operation((comm->req).N1,(comm->req).N2,(comm->req).operand);
                break;
            case ISPRIME:
                (comm->res).out = prchk((comm->req).N1);
                break;
            case EVENODD:
                (comm->res).out = even_or_odd((comm->req).N1);
                break;
            case ISNEGETIVE : (comm->res).out = is_negetive((comm->req).N1);break;
        }
        comm->res.clnt_res = 1;
        comm->res.server_res += 1;
        printf("You have been served for %dth time", comm->res.server_res);
        }pthread_exit(0);
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
    cout<<"created thread with key"<<key_comm<<endl;
    int id = shmget(key_comm,SHM_SIZE,IPC_CREAT | 0666);
    cout<<id<<endl;
    struct shm_blk* comm = (struct shm_blk*)shmat(id, NULL, 0);
    comm->mutex = 1;
    comm->id = id;
    pthread_create(tid+space,NULL,thr,comm);
    con->reg = IDLE;//tell it's registration is complete and get lost
    cout<<"Client"<<space<<"has been Registered"<<endl;
    return 1;
}
//Main Function
int main()
{
    int shmid;
    key_t key_connect;
    struct connect* con;
    tid = (pthread_t*)malloc(MAX_CLIENT*sizeof(pthread_t));
    //CONNECT CHANNEL KEY
    key_connect = 13;
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
    con->key = 'A';
    while(1)//Put something so that it will stop at some point
        {
            if(con->reg == REGISTER)
        {
            cout<<"Register"<<endl;
            register_client(con);
            con->reg = IDLE;
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
    return 0;
    free(tid);
}