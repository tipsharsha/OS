#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int sum;
void *runner(void *param);

int main(int argc,char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;

    if (argc != 2)
    {
        fprintf(stderr,"Input your arguments");
        return -1;
    }
    if (atoi(argv[1])<0)
    {
        fprintf(stderr,"%d must be >=0",atoi(argv[1]));
        return 0;
    }


    pthread_attr_init(&attr);
    pthread_create(&tid,&attr,runner,argv[1]);
    pthread_join(tid,NULL);
    printf("sum = %d\n",sum);
}

void *runner(void *param)
{
    int i,upper = atoi(param);
    sum =0;
    // sleep(10);
    for (i =1; i<=upper;i++)
    sum +=i;
    pthread_exit(0);
}
