#include "../CSF372.h"
typedef void (*sighandler_t)(int);

int i =0;

sighandler_t origSigInt = NULL;
sighandler_t origSigChild = NULL;//Sends if Child terminates
void mysighandler(int signum){
	if (signum == SIGINT){
		if (++i < 10){
			PRINT_INFO("Not terminating the process [%d]!!", i);
		}else{
			PRINT_INFO("Restoring the original sigint handler i = %d", i); 
			signal(SIGINT, origSigInt);
			
		}
	}else if (signum = SIGCHLD){
	}else{
		PRINT_INFO("Not handleed");
	}
}


int main(int argc, char **argv){
	origSigInt = signal(SIGINT, mysighandler);
	origSigChild = signal(SIGCHLD, mysighandler);
	while(1){}
	return 0;
}




// int i; char c; long l;
// int *pi = &i; // valid statement
// char *pc= &c; // valid statement
// long *pl=&l; // valid statement

// void foo(void){
	// // do something
// }

// typedef int* PI;
// PI pi = &i;

// typedef void (*foo_t) (void);
// foo_t pfoo = foo;
 
// void (*pfoo)(void) = foo; // functions when called without parama decompose to their address



// typedef void (*sighandler_t)(int);

       // sighandler_t signal(int signum, sighandler_t handler);


// int mymain(int argc, char **argv){

// }
// typedef int (*main_t)(int, cha**);

// main_t mm = mymain;
