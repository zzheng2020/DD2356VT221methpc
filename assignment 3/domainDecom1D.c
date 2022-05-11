
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    int rank, size, i, provided;
    
    // number of cells (global)
    int nxc = 128; // make sure nxc is divisible by size
    double L = 2*3.1415; // Length of the domain
    

    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // number of nodes (local to the process): 0 and nxn_loc-1 are ghost cells 
    int nxn_loc = nxc/size + 3; // number of nodes is number cells + 1; we add also 2 ghost cells
    double L_loc = L/((double) size);
    double dx = L / ((double) nxc);
    
    // define out function
    double *f = calloc(nxn_loc, sizeof(double)); // allocate and fill with z
    double *dfdx = calloc(nxn_loc, sizeof(double)); // allocate and fill with z

    for (i=1; i<(nxn_loc-1); i++)
      f[i] = sin(L_loc*rank + (i-1) * dx);
    
    // need to communicate and fill ghost cells f[0] and f[nxn_loc-1]
    // communicate ghost cells
    // ...
    // ...
    //tag 0: send left receive right
    //tag 1: send right receive left
    //first stage - send left cell
    MPI_Send(&f[2], 1, MPI_DOUBLE, (rank - 1 + size) % size, 0, MPI_COMM_WORLD);
    //second stage - send right cell
    MPI_Send(&f[nxn_loc - 3], 1, MPI_DOUBLE, (rank + 1) % size, 1, MPI_COMM_WORLD);
    //receive from left domain
    MPI_Recv(&f[0], 1, MPI_DOUBLE, (rank - 1 + size) % size, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //receive from right domain
    MPI_Recv(&f[nxn_loc - 1], 1, MPI_DOUBLE, (rank + 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // here we finish the calculations

    // calculate first order derivative using central difference
    // here we need to correct value of the ghost cells!
    for (i=1; i<(nxn_loc-1); i++)
      dfdx[i] = (f[i+1] - f[i-1])/(2*dx);

    
    // Print f values
    if (rank==0){ // print only rank 0 for convenience
        printf("My rank %d, here are my values ghost cells\n", rank);
        printf("My rank %d, ghost cell nxn_loc-1: %f\n",rank, f[nxn_loc-1]);
        printf("My rank %d, ghost cell 0: %f\n",rank, f[0]);
        printf("My rank %d, cell 2: %f\n",rank, f[2]);
        printf("My rank %d, dfdx[1] = dsin(0) = (f[2]-f[0])/(2*dx) = %f; cos(0) = %f\n",rank, dfdx[1], cos(0));
    }
    sleep(10)
    if(rank==1){
      printf("My rank %d, my Second from the left cell: %f\n", rank, f[2]);
    }
    if(rank==3){
      printf("My rank %d, my Second from the right cell: %f\n", rank, f[nxn_loc - 3]);
    }

    MPI_Finalize();
}






