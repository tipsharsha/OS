//Assignment 1//
#include "macros.h"
//Group 27 //
//2021AAPS0717H//

// Task 1.Making the arg vector var✅//
//Task 2.Make validation of x with the given range(in worker process) and validation of count in parent✅//
//Task 3.Make controller which loops and creates child proccesses to process each row and establish 1-1 pipe
// and let it wait for all n processes✅//
//Task 4. Each worker process i is aware of the index i of the loop when it is forked and will be responsible for validating the contents of only row number i of the 2D array. Assume 0-based indexing in this case.
// If any of the row values is invalid, the worker process will report error and terminate.✅
//Task 5.Each worker process i after being spawned will do the following:
// a. Create n threads (say worker threads) where each thread will take a value x from row i
// and
// i. Create the set px, subject to the value of p.✅
// ii. Calculate the thapx✅
// iii. Report it back to the worker process and terminate the thread gracefully.✅
// b. The main thread of each worker process i will wait for all spawned worker threads to
// return the thapx.✅
//TODO:
// c. Once all n values of thapx are available, and all worker threads have joined, the main
// thread in the worker process i will calculate the wpapx and write it back to the controller
// process in the pipe.❌
// Task 6. Next, the controller which was waiting to read/receive n values of wpapx from the worker
// processes will get unblocked after receiving all the n values of wpapx.❌
// Task 7. If a worker process is terminated before reporting the wpapx, the same should be handled by
// the controller by handling the SIGCHLD signal. The controller then will report the error and kill
// and clean up all worker processes.⭐❌
// Task 8. Finally, the controller process will calculate the average of n values of wpapx as fapx and
// report/print to the console the value of fapx.✅

int parentloop = 1;
//Global Variables
int n;
int a;
int b;
int p;
int** pi;
int* wpax;
int fapx;
int **fd;
int **thpax;
pthread_t** tid;
int status=0;


// void mysighandler(int signum){
// 	if (signum = SIGCHLD){
// 		PRINT_INFO("A child process was terminated");
// 		parentloop=0;
// 	}else{
// 		PRINT_INFO("Not handled");
// 	}
// }

//Structure for thread Params
typedef struct 
{
    int i;int j;int value;
}threadparam;

//Generation of px for x and returning thpax
int prchk(int num)
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
int arr_prime(int p,int k)
{
    int* px =(int*) malloc(p*sizeof(int*));
    int num=0;
    int t = 0;
    int sum =0;
    // printf("Enter a Number");
    // scanf("%d", &k);
    for(int i=k+1; ; i++)
    {
        if(prchk(i) == 1)
        {
            num++;
            px[num-1] = i;
            if(num==p)
            break;
        }
    }
    // for(int z=0; z<p; z++)
    // {
    //     printf("%d pxz ", px[z]);
    // }
    int m=0;
    int*qx = (int*)malloc(p*sizeof(int*));
    for(int i =0;i<p;i++)
    {
        qx[i]=1;
    }
    for(int j=k-1; j>1; j--)
    {
        if(prchk(j) == 1)
        {
            m++;
            qx[p-m] = j;
            t = p-m;
            if(m==p || j==2)
            break;
        }
    }
    // for(int w=0; w<p; w++)
    // {
    //     printf("\n qx%d ", qx[w]);
    // }
    printf("%d \n", t);
    
    int y = prchk(k);
    
    int fx[2*p+y-t];
    
    for(int s=0; s<=p-t; s++)
    {
        fx[s] = qx[s+t];
    }
    if(y==1)
    fx[p-t]=k;
    for(int i=p-t+y; i<2*p+y-t; i++)
    {
        fx[i]=px[i-p+t-y];
    }
    for(int w=0; w<2*p+y-t; w++)
    {
        sum += fx[w];    
    }
    return sum/(2*p+y-t);
    free(px);
    free(qx);
}
//Row validator
int validate_row(int* p,int min,int max,int count)//Function each worker uses to validate each row
{
    for (int i =0;i<count;i++)
    {
    printf("%d pi value",p[i]);
    if((p[i]>max) || (p[i] <min)){return 1;}
    }
    return 0;
}
//Thread function
void *thr (void *param)
{

    threadparam* x = ((threadparam*)param);
    // PRINT_INFO("\nIn THREAD %d %d\n ",((*x).i),((*x).j));
    printf("\nTHREAD %d %d\n",x->i,x->j);
    thpax[x->i][x->j]=arr_prime((x->value),p);
    fprintf(stderr,"\nTHPAX %d %d %d \n",thpax[x->i][x->j],x->i,x->j);
    pthread_exit(0);
}
//WORKER FUNCTION
int do_worker(int i)
{
    // PRINT_INFO("Entered the worker");
    if(validate_row(pi[i],a,b,n)){PRINT_ERR_EXIT("Given value of an element is not in range");return 1;}
    int wpaxag=0;
    
    tid[i] = malloc(n*sizeof(pthread_t));
    // PRINT_INFO("CREATED THREADS");
    threadparam** thrpar;
    thrpar = (threadparam**)malloc(n*(sizeof(threadparam*)));
    for(int i =0;i<n;i++)
    {
        thrpar[i] = (threadparam*)malloc(n*sizeof(threadparam));
    }
    for(int j=0;j<n;j++)
    {
    printf("%d matrix values",pi[i][j]);

    (thrpar[i]+j)->i = i;
    (thrpar[i]+j)->j = j;
    (thrpar[i]+j)->value = pi[i][j];
    printf("\n VALUE %d %d %d struct value\n",(thrpar[i][j]).i,(thrpar[i][j]).j,(thrpar[i][j]).value);
    pthread_create((tid[i]+j),NULL,thr,&(thrpar[i][j]));
    printf("\nIS life good\n");
    }

    for(int j=0;j<n;j++)
    {
        pthread_join(tid[i][j],NULL);
        printf("IT IS GOOD");
        for(int i=0;i<n;i++) {free(tid[i]);}
        free(tid);
        for(int i=0;i<n;i++){free(thrpar[i]);}
        free(thrpar);
        printf("THPAXIJ %d",thpax[i][j]);
        wpaxag += thpax[i][j];
    }
        for(int i=0;i<n;i++) {free(thpax[i]);}
        free(thpax);
    PRINT_INFO("\nSuccessfully ran all the threads %d",wpaxag);
    int wpa = wpaxag/n;

    printf("\nwpa,i %d %d",wpa,i);
    close(fd[i][0]);
    write(fd[i][1],&wpa,sizeof(int));
    close(fd[i][1]);
    printf("\nwill this print\n");
    // PRINT_INFO("Could close pipes");
}
//CONTROLLER FUNCTION
int do_control(int i)
{
    // if(parentloop)
    // {
    // PRINT_INFO("What is happening in last loop %d",i);
    close(fd[i][1]);
    printf("Could u close pls");
    read(fd[i][0],(wpax+i),sizeof(int));
    printf("%d wpax[%d] \n",wpax[i], i);
    close(fd[i][0]);
    printf("I am a parent\n");
    printf("Child Complete\n");
    for(int i=0;i<n;i++)
    {free(fd[i]);}
    free(fd);
    return 1;
}
// MAIN FUNCTION
int main(int argc,char *argv[])
{
    n = atoi(argv[1]);
    if (argc != n*n+5) {printf("Given number of arguments are not valid");}//Validating the count//
    a = atoi(argv[2]);
    b = atoi(argv[3]);
    p = atoi(argv[4]);
    printf("%d %d %d %d \n",n,a,b,p);
    wpax = (int*)malloc(n*sizeof(int));
    pi = (int**)malloc(n*sizeof(int*)) ;
    fd = (int**)malloc(n*sizeof(int*)) ;
    for (int i =0;i<n;i++)
    pi[i] = (int*)malloc(n*sizeof(int));
    for (int i =0;i<n;i++)
    fd[i] = (int*)malloc(2*sizeof(int));
    tid = (pthread_t**)malloc(n*sizeof(pthread_t*));
    PRINT_INFO("Pthreads created");
    for (int i =0;i<n;i++)
    {
        for (int j =0;j<n;j++)
        {
            pi[i][j] = atoi(argv[n*i+j+5]);
        }
    }
    //Creating the pipe for all the workers
    thpax = (int**)malloc(n*sizeof(int*));
    for(int i= 0;i<n;i++)
    {
        thpax[i] = malloc(n*sizeof(int));
    }

    for(int i=0;i<n;i++)
    { if (pipe(fd[i])==-1)
    {
        fprintf(stderr,"pipe failed");
    }}
        // PRINT_INFO("Pipes Created");
    // Creating child,worker processes
    pid_t pid [n];
    int l=0;
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
            // //Child processes
            // PRINT_INFO("CHILD PROCESS %d",i);
            do_worker(i);
    printf("\nwillthis \n");
            return -1;
        }
     }
            //Control process
        for(int i =0;i<n;i++)
        {
            while(wait(&status)!=-1){}
            // PRINT_INFO("In control now");
            // PRINT_INFO("Waited for Child Process");
            do_control(i);
            
            printf("is this working");
            l += wpax[i];
            free(wpax);
            printf("%d",l);
            free(tid);
        }
    
            fapx = l/n;
            // printf("Value of fapx is %d",fapx);
     
    

    free(pi);
    for(int i=0;i<n;i++) {free(pi[i]);}
    return 1;
    //freeing all the dynamic memory//
 }
