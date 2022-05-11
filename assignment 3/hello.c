#include <mpi.h>
#include <stdio.h>

#define TAG_A 0

int main(int argc, char* argv[]) {
    
    int rank, size, i, provided;
    float A[10];

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
    for(i = 0; i < 10; i++)
        A[i] *= rank;

    printf("Hello World from rank %d from %d processes!\n", rank, size);

    MPI_Finalize();
    return 0;
}