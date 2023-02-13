#include "../CSF372.h"
typedef void (*sighandler_t)(int);

int i =0;

sighandler_t origSigInt = NULL;
void mysighandler(int signum){
	if (signum == SIGINT){
		if (++i < 10){
			PRINT_INFO("Not terminating the process [%d]!!", i);
		}else{
			PRINT_INFO("Restoring the original sigint handler i = %d", i); 
			signal(SIGINT, origSigInt);
			
		}
	}else{
		PRINT_INFO("Not handled");
	}
}


int main(int argc, char **argv){
	origSigInt = signal(SIGINT, mysighandler);
	while(1){
		int retSleep = sleep(10);
		PRINT_INFO("Sleep completed ;; unslept seconds = %d", retSleep);
	}
	return 0;
}