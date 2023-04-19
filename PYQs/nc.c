#include "macros.h"

int num1 = 6; //global variable
void meth(int y){
 static int x = 5;
 x = x + y;
 num1 = x;
}
int main(){
 int num2 = 10;
 int x =fork();
 if(x){ //Line p
 printf("\n%d %d Parent 1%d \n ", num1 + num2, getpid(),x);
 meth(num2);
 }
 else if(fork() + fork() + fork()){
 meth(num2);
 meth(num2);
 printf("\n%d %d Parent 2 %d\n", num1, getpid(),x);
}
 else printf("\n%d %d Child\n", num2, getpid());
}