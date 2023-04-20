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

struct shm_blk// struct made for blk requested
{
    pthread_mutex_t mutex;
    struct request req;
    struct response res;
    int id;
};
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
}
struct connect
{
    int mutex;
    int reg;
    char key;
};
int flag[MAX_CLIENT]={0};//Takes care of how many clients are registered
int to_register[MAX_CLIENT]={0};
pthread_t* tid;
void *thr (void *param)
{
    struct shm_blk* comm =  (struct shm_blk*)param;
    L1:
        while(comm->mutex){};
        pthread_mutex_lock(comm->mutex);
        switch((comm->req).type)
        {   
            case UNREGISTER:
            shmdt(comm);
            shmctl(comm->id, IPC_RMID, NULL);
            pthread_exit();
            case ARITH: (comm->res).out = arithmetic_operation((comm->request).N1,(comm->request).N2,(comm->request).operand);
            case ISPRIME: (comm->res).out = prchk((comm->request).N1);
            case EVENODD: (comm->res).out = even_or_odd((comm->request).N1);
            case ISNEGETIVE : (comm->res).out = is_negetive((comm->request).N1);
        }
        comm->res.clnt_res = 1;
        comm->res.server_res += 1;
        printf("You have been served for %dth time", comm->res.server_res);
    goto L1;
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
int prchk(int num)//✅
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
//Register a client
int register_client(struct connect* con)
{
    
    int space = first_available();
    if (space != -1)
    {
        flag[space] = TRUE;
    }
    else{

        //BUFFER FULL
    }
    //Generate key
    key_t key_comm = ftok(".",0+space);
    con->key = 0+space;
    int id = shmget(key_comm,SHM_SIZE,IPC_CREAT | 0666);
    struct shm_blk* comm = (struct shm_blk*)shmat(id, NULL, 0);
    comm->mutex = pthread_mutex_init();
    pthread_mutex_lock(comm->mutex);
    (comm->req).type = 0;
    comm->id = id;
    pthread_create(tid[space],NULL,thr,comm);
    con->reg = IDLE;//tell it's registration is complete and get lost
    printf("Client %d has been Registered",space);
}
//Main Function
int main()
{
    int shmid;
    key_t key_connect;
    struct connect*con;
    tid[i] = malloc(MAX_CLIENT*sizeof(pthread_t));
    //CONNECT CHANNEL KEY
    key_connect = ftok(".", 'C');
    // create the shared memory segment with read/write permission
    shmid = shmget(key_connect, CONNECT_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }
    else{
        printf("Connect Channel Successfully created");
    }
    // attach the shared memory segment to the server's address space
    con = (struct connect*) shmat(shmid, NULL, 0);
    if (con == (struct connect*) -1) {
        perror("shmat");
        exit(1);
    }
    con->mutex = 0;
    con->reg = IDLE;
    while(TRUE)//Put something so that it will stop at some point
        {
            if(con->reg == REGISTER)
        {
            register_client(con);
            con->reg = IDLE;
        }
        }
    
    // detach the shared memory segment from the server's address space
    if (shmdt(con) == -1) {
        perror("shmdt");
        exit(1);
    }
    // delete the shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
    return 0;
}
