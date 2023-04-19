#include "macros.h"
int status =0;
int main(){
 int num2 = 10;
 int x =fork();
    if (x)
    {
        wait(&status);
        printf("\n%d %d Child\n", num2, getpid());
    }
    else num2 =10;
}