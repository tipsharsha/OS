#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid, waitPid;
	int status=0;
	printf("%s %s %d  %dParent Process !!!\n", 
			__FILE__, __FUNCTION__, __LINE__, getpid());
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