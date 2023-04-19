#include <stdio.h>
int main(int argc,char *argv)
{
    int i =5;
    printf("Address of 1 is %p",&i);
    return i;
}
//To generate a bin file use strace ./thefile.filetype