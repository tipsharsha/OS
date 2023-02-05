#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    pid = fork();
    if(pid<0)
    {
        fprintf(stderr,"Error has occured");
        return 1;
    }
    else if (pid==0)
    {
        printf("I am a child\n");
        execlp("ls","ls","-lh",NULL);

        
    }
    else
    {
        printf("I am a parent\n");
        wait(NULL);
        printf("Child Complete\n");
    }
    return 0;
}