#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

int main(int argc,char * argv[])
{
    int np, rank, provided;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    printf("Hello world!   rank:%d  of np:%d\n",rank,np);
    int a[6][6];
    int b[6][6];

    for(int i=0;i<6;i++)
        for(int j=0;j<6;j++)
        {
            a[i][j]=rank;
            b[i][j]=0;
            if(1==rank){
                printf("i:%d, j:%d, a value:%d, b value:%d\n",i,j,a[i][j],b[i][j]);
            }
        }

    MPI_Reduce(&a,&b,6,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

    if(0==rank)
    {
        for(int i=0;i<6;i++)
        {
            for(int j=0;j<6;j++)
            {
                printf("i:%d, j:%d, b:%d \n",i,j,b[i][j]);
            }
        }
    }



    MPI_Finalize();

    return 0;
}