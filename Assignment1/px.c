#include <stdio.h>
#include <stdlib.h>
 

int prchk(int num)
{
    for(int i=2; i<num; i++)
    {
        if(num%i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int arr_prime(int p,int k)
{
    int* px =(int*) malloc(p*sizeof(int*)); 
    int num=0;
    int t = 0;
    int sum;
    // printf("Enter a Number");
    // scanf("%d", &k);
    for(int i=k+1; ; i++)
    {
        if(prchk(i) == 1)
        {
            num++;
            px[num-1] = i;
            if(num==p)
            break;
        }
    }
    // for(int z=0; z<p; z++)
    // {
    //     printf("%d ", px[z]);
    // }
    int m=0;
    int*qx = (int*)malloc(p*sizeof(int*));
    for(int i =0;i<p;i++)
    {
        qx[i]=1;
    }
    for(int j=k-1; j>1; j--)
    {
        if(prchk(j) == 1)
        {
            m++;
            qx[p-m] = j;
            t = p-m;
            if(m==p || j==2)
            break;
        }
    }
    // for(int w=0; w<p; w++)
    // {
    //     printf("%d ", qx[w]);
    // }
    printf("%d \n", t);
    
    int y = prchk(k);
    
    int fx[2*p+y-t];
    
    for(int s=0; s<=p-t; s++)
    {
        fx[s] = qx[s+t];
    }
    if(y==1)
    fx[p-t]=k;
    for(int i=p-t+y; i<2*p+y-t; i++)
    {
        fx[i]=px[i-p+t-y];
    }
    for(int w=0; w<2*p+y-t; w++)
    {
        sum += px[w];
    }
    return sum/k;
}
