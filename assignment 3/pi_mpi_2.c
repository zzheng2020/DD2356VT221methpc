#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define NUM_ITER 1000000000

int main(int argc, char* argv[])
{
    int local_count = 0;
    int rank, num_ranks, i, iter, provided;
    double x, y, z, pi;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    int flip = NUM_ITER/num_ranks;

    double start_time, stop_time, elapsed_time;
    start_time = MPI_Wtime();

    srand(time(NULL) + 123456789 + rank*100);
    
    // Calculate PI following a Monte Carlo method
    for (int iter = 0; iter < flip; iter++)
    {
        // Generate random (X,Y) points
        x = (double)random() / (double)RAND_MAX;
        y = (double)random() / (double)RAND_MAX;
        z = sqrt((x*x) + (y*y));
        
        // Check if point is in unit circle
        if (z <= 1.0)
        {
            local_count++;
        }
    }
    
    //assume we only use size of log2
    int epoch = log2(num_ranks);
    for(int i = 1; i < epoch + 1; i++){
        int base = pow(2,i);
        if(rank % base){
            MPI_Send(&local_count, 1, MPI_INT, rank - (rank % base), 0, MPI_COMM_WORLD);
            break;
        }else{
            int temp_count;
            MPI_Recv(&temp_count, 1, MPI_INT, rank + pow(2,i-1), 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            local_count += temp_count;
        }
    }

    stop_time = MPI_Wtime();
    elapsed_time = stop_time - start_time;
    if (rank == 0)
    {
        // Estimate Pi and display the result
        pi = ((double)local_count / (double)NUM_ITER) * 4.0; 
        printf("pi: %f\n", pi);
        printf("Execution Time: %f\n", elapsed_time);
    }

    MPI_Finalize();
    return 0;
}