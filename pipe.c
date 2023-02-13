#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    char wrt[25] = "This is a message";
    char rd[25];
    int fd[2];
    pid_t pid;
    //Checking for pipe//
    if (pipe(fd)==-1)
    {
        fprintf(stderr,"pipe failed");
    }
    pid = fork();
    if(pid<0)
    {
        fprintf(stderr,"Error has occured");
        return 1;
    }
    else if (pid==0)
    {
        close(fd[1]);
        read(fd[0],rd,25);
        close(fd[0]);

        printf("%s \n",rd);
        execlp("ls","ls","-lh",NULL);

        
    }
    else
    {
        close(fd[0]);
        write(fd[1],wrt,strlen(wrt)+1);
        close(fd[1]);
        printf("I am a parent\n");
        printf("Child Complete\n");
    }
    return 0; 
}