#include <stdio.h>
#include <stdlib.h>
int n; 

int prchk(int n)
{
    for(int i=2; i<n; i++)
    {
        if(n%i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    int p;
    printf("Enter value of p\n");
    scanf("%d",&p);
    int* px =(int*) malloc(p*sizeof(int*)); 
    int k;
    int n=0;
    int t = 0;
    printf("Enter a Number");
    scanf("%d", &k);
    for(int i=k+1; ; i++)
    {
        if(prchk(i) == 1)
        {
            n++;
            px[n-1] = i;
            if(n==p)
            break;
        }
    }
    // for(int z=0; z<p; z++)
    // {
    //     printf("%d ", px[z]);
    // }
    int m=0;
    int*qx = (int*)malloc(p*sizeof(int*));
    // for(int i =0;i<p;i++)
    // {
    //     qx[i]=1;
    // }
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
        printf("%d ", fx[w]);
    }
    return 0;
}