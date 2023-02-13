#include "CSF372.h"

#define SZBUF 127

int parentloop=1;

sighandler_t origSigChild = NULL;
void mysighandler(int signum){
	if (signum = SIGCHLD){
		PRINT_INFO("A child process was terminated");
		parentloop=0;
	}else{
		PRINT_INFO("Not handled");
	}
}


typedef struct _data {
	int num;
	char buffer[SZBUF+1];
}DATA;

void getpipe(int *rdfd, int *wrfd){
	int pipefd[2] ={0};
	pipe(pipefd);
	*rdfd=pipefd[0];
	*wrfd=pipefd[1];
}

void do_child(int rdfd, int wrfd){
	DATA data;
	int i =5;
	while(i-- > 0){
		ssize_t ret;
		memset(&data, 0x0, sizeof(data));
		ret = read(rdfd, &data, sizeof(data));
		// validation
		PRINT_INFO("read %ld bytes values [%d %s];",
			ret, data.num, data.buffer);
		data.num = data.num *10;
		snprintf(data.buffer, SZBUF, "CHILD: The number is %d;", data.num);
		
		ret = write(wrfd, &data, sizeof(data));
		// validation
		PRINT_INFO("Wrote %ld bytes to the pipe", ret);
	}
	PRINT_INFO("The child is terminating;");
	
	close(rdfd);
	close(wrfd);
	_exit(0);
}
void do_parent(int rdfd, int wrfd){

	while(parentloop==1){
		DATA data;
		ssize_t ret;
		memset(&data, 0x0, sizeof(data));
		data.num = rand();
		snprintf(data.buffer, SZBUF, "PARENT: The number is %d;", data.num);
		ret = write(wrfd, &data, sizeof(data));
		if(ret < 0){
			PRINT_ERROR("write failed with err %d", errno);
			close(wrfd);
			close(rdfd);
			return;
		}
		PRINT_INFO("Wrote %ld bytes to the pipe", ret);
		memset(&data, 0x0, sizeof(data));
		ret = read(rdfd, &data, sizeof(data));
		// validation
		PRINT_INFO("read %ld bytes values [%d %s];",
			ret, data.num, data.buffer);
	
		sleep(1);
	}
	PRINT_INFO("PARENT : Broke out of the loop");
	close(wrfd);
	close(rdfd);
		
}


int main(int argc, char **argv){
	pid_t pid, child_pid;;
	int status=0;;
	int rdfd=0, wrfd=0;
	char buffer[SZBUF+1]={0};
	signal(SIGCHLD, mysighandler);
	getpipe(&rdfd, &wrfd);
	
	PRINT_INFO("Parent Process !!! ");
	pid = fork(); /* fork a child process */
	if (pid < 0) { /* error occurred */
		PRINT_ERR_EXIT("fork failed");
		return 1;
	}
	
	if (pid == 0) { /* child process */
		PRINT_INFO("The child Process");
		do_child(rdfd, wrfd);
		PRINT_ERR_EXIT("Unreachable code executed");
		// we never return here
		return 0;
	}
	
	/* parent process */
	PRINT_INFO("The parent Process");
	do_parent(rdfd, wrfd);
	PRINT_INFO("The parent Process returned after do_parent!!!");
	child_pid = wait(&status); /* parent waits for child to terminate */
	PRINT_INFO("Child[%d]  terminated returned %d\n", child_pid, status);
	return 0;
}

