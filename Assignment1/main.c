
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
//Group 27 //
//2021AAPS0717H//
#define PRINT(MSG, ...) printf("%d %d %ld %s %s %d : "MSG"\n", getpid(),getppid(),pthread_self(),__FILE__, __FUNCTION__, __LINE__,##__VA_ARGS__)

// Task 1.Making the arg vector var//
//Task 2.Make validation of x with the given range(in worker process) and validation of count in parent//
//Task 3.Make controller which loops and creates child proccesses to process each row and establish 1-1 pipe and let it wait for all n processes//
//Task 4.

int n;
int a;
int b;
int p;
int** pi;//Global Variables
char wrt[25] = "This is a message";
char rd[25]="Maybe";

int validate_row(int* p,int min,int max,int count)//Function each worker uses to validate each row
{
    for (int i =0;i<=max;i++)
    {
    if((p[i]>max) || p[i] <min){return 0;}
    }
    return 1;
}

int main(int argc,char *argv[])
{
    n = atoi(argv[1]);
    if (argc != n*n+5) {printf("Given number of arguments are not valid");}//Validating the count//
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    p = atoi(argv[4]);
    pi = (int**)malloc(n*sizeof(int*)) ;
    for (int i =0;i<n;i++)
    pi[i] = (int*)malloc(n*sizeof(int));
    for (int i =0;i<n;i++)
    {
        for (int j =0;j<n;j++)
        {
            pi[i][j] = atoi(argv[n*i+j+5]);
        }
    }
    //Creating the pipe for all the workers
    int fd[n][2];
    for(int i=0;i<n;i++)
    { if (pipe(fd[i])==-1)
    {
        fprintf(stderr,"pipe failed");
    }}
    // Creating child,worker processes
    pid_t pid [n];
    for(int i =0;i<n;i++)
     {   
        pid[i] = fork();
        if(pid[i]<0)
        {
            fprintf(stderr,"Fork Failed");
            return 1;
        }
        else if (pid[i]==0)
        {
            close(fd[i][1]);
            read(fd[i][0],rd,25);
            close(fd[i][1]);

            printf("%s \n",rd);
            execlp("ls","ls","-lh",NULL);
            return -1;
        }
        else
        {
            close(fd[i][0]);
            write(fd[i][1],wrt,strlen(wrt)+1);
            close(fd[i][1]);
            printf("I am a parent\n");
            printf("Child Complete\n");
        }
     }


    // for (int i =0;i<n;i++)//for checking the matrix
    // {
    //     for (int j =0;j<n;j++)
    //     {
    //         printf("%d ",pi[i][j]);
    //     }
    //     printf("\n");
    // }
    for(int i=0;i<n;i++) {free(pi[i]);}//freeing all the dynamic memory//
 }

