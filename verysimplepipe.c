#include "../CSF372.h"

int main(int argc, char *argv[]) { 
   int pipefd[2];
   pid_t cpid;
   char buf;

   if (argc != 2) {
	   fprintf(stderr, "Usage: %s <string>\n", argv[0]);
	   exit(EXIT_FAILURE);
   }
	
	PRINT_INFO("Creating pipe !!!");
   if (pipe(pipefd) == -1) {
	   perror("pipe");
	   exit(EXIT_FAILURE);
   }

	PRINT_INFO("Before forking the child!!");
   cpid = fork();
   if (cpid == -1) {
	   perror("fork");
	   exit(EXIT_FAILURE);
   }
	
	
   if (cpid == 0) {    /* Child reads from pipe */
	int i =0;
	PRINT_INFO("Child after fork; closing write fd in the child!!")
	   close(pipefd[1]);          /* Close unused write end */

	   while (read(pipefd[0], &buf, 1) > 0){
		   // Bug
		   if (++i==5){
			   break;
		   }
		   write(STDOUT_FILENO, &buf, 1);
	   }

	   write(STDOUT_FILENO, "\n", 1);
	   PRINT_INFO("Child has written on the console!!");
	   close(pipefd[0]);
	   PRINT_INFO("Child is termionating!!")
	   _exit(EXIT_SUCCESS);

   } else {            /* Parent writes argv[1] to pipe */
		PRINT_INFO("Parent after fork; closing read fd in the child!!");
	   close(pipefd[0]);          /* Close unused read end */
	   write(pipefd[1], argv[1], strlen(argv[1]));
	   close(pipefd[1]);          /* Reader will see EOF */
	   
	   PRINT_INFO("Parent waiting for child to terminate!!");
	   wait(NULL);                /* Wait for child */
	   exit(EXIT_SUCCESS);
   }
}