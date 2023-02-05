
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PRINT(MSG, ...) printf("%d %d %ld %s %s %d : "MSG"\n", getpid(),getppid(),pthread_self(),__FILE__, __FUNCTION__, __LINE__,##__VA_ARGS__)
int main()
{
	pid_t pid, waitPid;
	int status=0;
	// printf("%s %s %d  %dParent Process !!!\n", 
	// 		__FILE__, __FUNCTION__, __LINE__, getpid());
    PRINT("Parent Process");
	pid = fork(); /* fork a child process */
	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) { /* child process */
		printf("%s %s %d %d Child Process\n",
			       	__FILE__, __FUNCTION__, __LINE__, getpid());
		execlp("/bin/ls","ls",NULL);
		printf("%s %s %d %d Child Process after exec\n",
			       	__FILE__, __FUNCTION__, __LINE__, getpid());
		exit(43);
	}
	else { /* parent process */
		sleep(10);
		printf("%s %s %d  %d Child[%d]  Complete returned %d \n", 
				__FILE__, __FUNCTION__, __LINE__, 
				getpid(), waitPid, status);
		waitPid = wait(&status); /* parent waits for child to complete */
		printf("%s %s %d  %d Child[%d]  Complete returned %d\n", 
				__FILE__, __FUNCTION__, __LINE__, 
				getpid(), waitPid, status);
	}
	return 0;
}