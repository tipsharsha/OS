//Assignment 1//
#include "macros.h"
#include "px.c"
//Group 27 //
//2021AAPS0717H//

// Task 1.Making the arg vector var✅//
//Task 2.Make validation of x with the given range(in worker process) and validation of count in parent✅//
//Task 3.Make controller which loops and creates child proccesses to process each row and establish 1-1 pipe and let it wait for all n processes✅//
//Task 4. Each worker process i is aware of the index i of the loop when it is forked and will be responsible for validating the contents of only row number i of the 2D array. Assume 0-based indexing in this case. If any of the row values is invalid, the worker process will report error and terminate.✅
//TODO:
//Task 5.Each worker process i after being spawned will do the following:
// a. Create n threads (say worker threads) where each thread will take a value x from row i
// and
// i. Create the set px, subject to the value of p.
// ii. Calculate the thapx
// iii. Report it back to the worker process and terminate the thread gracefully.
// b. The main thread of each worker process i will wait for all spawned worker threads to
// return the thapx.
// c. Once all n values of thapx are available, and all worker threads have joined, the main
// thread in the worker process i will calculate the wpapx and write it back to the controller
// process in the pipe.
// Task 6. Next, the controller which was waiting to read/receive n values of wpapx from the worker
// processes will get unblocked after receiving all the n values of wpapx.
// Task 7. If a worker process is terminated before reporting the wpapx, the same should be handled by
// the controller by handling the SIGCHLD signal. The controller then will report the error and kill
// and clean up all worker processes.
// Task 8. Finally, the controller process will calculate the average of n values of wpapx as fapx and
// report/print to the console the value of fapx.

int n;
int a;
int b;
int p;
int** pi;//Global Variables
char wrt[25] = "This is a message";
char rd[25]="Maybe";
int*** pxs;

int validate_row(int* p,int min,int max,int count)//Function each worker uses to validate each row
{
    for (int i =0;i<count;i++)
    {
    if((p[i]>max) || (p[i] <min)){return 1;}
    }
    return 0;
}

int* px_for_x(int x)
{
    int* p;

    return p;
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
            //Child pocesses
            close(fd[i][1]);
            read(fd[i][0],rd,25);
            close(fd[i][1]);
            if(validate_row(pi[i],a,b,n)){printf("Entered arguments aren't valid put them in range");exit(0);}
            printf("%s \n",rd);
            // execlp("ls","ls","-lh",NULL);
            return -1;
        }
        else
        {
            //Parent process
            wait(100);
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

