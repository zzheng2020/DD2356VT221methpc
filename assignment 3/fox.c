//reference: 
//1. https://github.com/Ionitedev/DD2356-A3/blob/master/fox.c
//2. https://stackoverflow.com/questions/5104847/mpi-bcast-a-dynamic-2d-array

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <stdbool.h>

#define matrix_size 4

void matmul(double **temp_A, double **sub_B, double **sub_C, int tile_size){
    for (int i = 0; i < tile_size; i++)
    {
        for (int j = 0; j < tile_size; j++)
        {
            for (int k = 0; k < tile_size; k++)
            {
                sub_C[i][j] += temp_A[i][k] * sub_B[k][j];
            }
        }
    }
}

void cont2matrix(double * continuous_buffer, double ** matrix, int tile_size){
    int index = 0;
    for (int i = 0; i < tile_size; i++)
    {
        for (int j = 0; j < tile_size; j++)
        {
            matrix[i][j] = continuous_buffer[index];
            index++;
        }
    }
}

void matrix2cont(double *continuous_buffer, double **matrix, int tile_size){
    int index = 0;
    for (int i = 0; i < tile_size; i++)
    {
        for (int j = 0; j < tile_size; j++)
        {
            continuous_buffer[index] = matrix[i][j];
            index++;
        }
    }
}

void init_matrix(double ** pointer, int size, double value){
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            pointer[row][col] = value;
        }
    }
}

void malloc_square(double ** pointer, int size){
    for (int i = 0; i < size; i++)
    {
        pointer[i] = (double *)malloc(size * sizeof(double));
    }
}

void get_tile(double** tile, int tile_size, double** matrix, int start_x, int start_y){
    for (int y = 0; y < tile_size; y++)
    {
        for (int x = 0; x < tile_size; x++)
        {
            tile[y][x] = matrix[y + start_y][x + start_x];
        }   
    }
}

void return_tile(double** tile, int tile_size, double** matrix, int start_x, int start_y){
    for (int y = 0; y < tile_size; y++)
    {
        for (int x = 0; x < tile_size; x++)
        {
            matrix[y + start_y][x + start_x] = tile[y][x];
        }
    }
}



int main(int argc, char* argv[])
{   
    double start_time, stop_time, elapsed_time;

    //double precision input matrix
    double **A = (double **)malloc(matrix_size * sizeof(double*));
    double **B = (double **)malloc(matrix_size * sizeof(double*));
    double **C = (double **)malloc(matrix_size * sizeof(double*));
    malloc_square(A, matrix_size);
    init_matrix(A, matrix_size, 1);
    malloc_square(B, matrix_size);
    init_matrix(B, matrix_size, 1);
    malloc_square(C, matrix_size);
    init_matrix(C, matrix_size, 0);

    //old comm
    int rank, num_ranks, provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_ranks);

    start_time = MPI_Wtime();

    //assume the number of processes must be a square
    //number of process per row/col
    int grid_size = sqrt(num_ranks);

    // // int dim_size[2] = {grid_size, grid_size};
    // // int period[2] = {1, 1};

    // //version 1
    // //sub comm/new comm
    // //MPI_Comm comm_cart, comm_col, comm_row;
    // //int rank_col;
    // //int rank_row;
    // //Invalid communicator??????????????
    // //MPI_Cart_create(MPI_COMM_WORLD, 2, dim_size, period, 1, &comm_cart);
    
    // //version 2
    MPI_Comm comm_row;
    div_t colorkey = div(rank, grid_size);
    int color = colorkey.quot;
    int key   = colorkey.rem;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &comm_row);

    // //version 1
    // //MPI_Cart_create(MPI_COMM_WORLD, 2, dim_size, period, 1, &comm_cart);
    // //{1, 0} remain X, {0, 1} remain Y.
    // // MPI dimension is reversed
    // //拓扑逻辑上每col的第几个
    // // p0 p0 p0
    // // p1 p1 p1
    // // p2 p2 p2

    // // Why can not sub twice?
    // // int remain_col[2] = {1, 0};
    // // MPI_Cart_sub(comm_cart, remain_col, &comm_col); //col comm

    // //version 2
    int above, below;
    if (rank < grid_size)
    {
        above = (rank - grid_size) % num_ranks + num_ranks;
    }else{
        above = (rank - grid_size) % num_ranks;
    }
    below = (rank + grid_size) % num_ranks;
    // if(rank == 3){
    //     printf("my rank: %d\n", rank);
    //     printf("my above: %d\n", above);
    //     printf("my below: %d\n", below);
    // }
    // // MPI_Cart_shift(comm_col, 0, 1, &above, &below);
    
    // //拓扑逻辑上每row的第几个
    // // p0 p1 p2
    // // p0 p1 p2
    // // p0 p1 p2
    // //int remain_row[2] = {0, 1};
    // //MPI_Cart_sub(comm_cart, remain_row, &comm_row); //row comm
    // //every process has a sub_rank in row and col
    // //int sub_rank_col;
    // //int sub_rank_row;
    // //MPI_Comm_rank(comm_col, &sub_rank_col);
    // //MPI_Comm_rank(comm_row, &sub_rank_row);
    // //update rank if we set reorder as 1?
    // //MPI_Comm_rank(comm_cart, &rank);

    // //sub_matrix size
    int tile_size = matrix_size / grid_size;
    double ** sub_A = (double **)malloc(tile_size * sizeof(double*));
    malloc_square(sub_A, tile_size);
    double ** sub_B = (double **)malloc(tile_size * sizeof(double*));
    malloc_square(sub_B, tile_size);
    //get tile (different MPI process has different sub_rank_col/sub_rank_row)
    // //reverse!!!
    // //
    // // +++++++++>x
    // // + p0 p1 p2
    // // + p3 p4 p5
    // // + p6 p7 p8
    // // v
    // // y
    int start_x = (rank % grid_size) * tile_size;
    int start_y = (rank / grid_size) * tile_size;
    // if (rank == 0 || rank == 1 || rank == 3)
    // {
    //     printf("my_rank: %d start_x: %d start_y: %d \n",rank,start_x, start_y);
    // }
    
    get_tile(sub_A, tile_size, A, start_x, start_y);
    // if (rank == 0)
    // {
    //     for (int i = 0; i < tile_size; i++)
    //     {
    //         for (int j = 0; j < tile_size; j++)
    //         {
    //             printf("(%d,%d):%f ", i,j,sub_A[i][j]);
    //         }
            
    //     }
    //     printf("\n");
        
    // }
    get_tile(sub_B, tile_size, B, start_x, start_y);
    // if (rank == 0)
    // {
    //     for (int i = 0; i < tile_size; i++)
    //     {
    //         for (int j = 0; j < tile_size; j++)
    //         {
    //             printf("(%d,%d):%f ", i,j,sub_B[i][j]);
    //         }
            
    //     }
    //     printf("\n");
        
    // }

    double ** sub_C = (double **)malloc(tile_size * sizeof(double*));
    malloc_square(sub_C, tile_size);
    //init to zero at first
    init_matrix(sub_C, tile_size,0);

    // //use this buffer to receive the bcast sub_matrix
    double * continuous_buffer = (double *)malloc(tile_size * tile_size * sizeof(double));

    // //store the temp sub_A matrix in every iteration received from Bcast
    double ** temp_A = (double **)malloc(tile_size * sizeof(double*));
    malloc_square(temp_A, tile_size);


    for (int iteration = 0; iteration < grid_size; iteration++){
        // if (rank == 0)
        // {
        //     printf("iteration: %d\n", iteration);
        // }
        //（第几次迭代 + 当前MPI process位于第几行） mod grid_size 可以求出当前迭代这一行需要广播的process X坐标
        //int x = (iteration + sub_rank_col) % grid_size;
        int x = (iteration + (rank / grid_size)) % grid_size;
        //如果坐标匹配上就广播
        //"Bcast by defining an MPI datatype which described how the 2d array is actually laid out in memory"
        //one process bcast and the rest of processes receive
        if(x == key /*x == sub_rank_row*/){
            // if (rank == 1)
            // {
            //     printf("my rank: %d ,iteration: %d\n", rank, iteration);
            // }
            
            matrix2cont(continuous_buffer, sub_A, tile_size);
            //x send!
            MPI_Bcast(continuous_buffer, tile_size * tile_size, MPI_DOUBLE, x, comm_row);
            //from continuous data to matrix (Whether array can be copied in C?)
            cont2matrix(continuous_buffer, temp_A, tile_size);
        }else{
            // if (rank == 1)
            // {
            //     printf("my rank: %d ,iteration: %d\n", rank, iteration);
            //     for (int i = 0; i < tile_size * tile_size; i++)
            //     {
            //         printf("%f ",continuous_buffer[i]);
            //     }
            //     printf("\n");
            // }
            //the rest receive
            MPI_Bcast(continuous_buffer, tile_size * tile_size, MPI_DOUBLE, x, comm_row);
            // if (rank == 1)
            // {
            //     printf("my rank: %d ,iteration: %d\n", rank, iteration);
            //     for (int i = 0; i < tile_size * tile_size; i++)
            //     {
            //         printf("%f ",continuous_buffer[i]);
            //     }
            //     printf("\n");
            // }
            //from continuous data to matrix
            cont2matrix(continuous_buffer, temp_A, tile_size);

        }

        //temp_A X sub_B = sub_C
        //每次迭代都会根据新的temp_A和sub_B去更新sub_C
        // if (rank == 1)
        // {
        //     printf("my rank: %d ,iteration: %d\n", rank, iteration);
        //     for (int i = 0; i < tile_size; i++)
        //     {
        //         for (int j = 0; j < tile_size; j++)
        //         {
        //             printf("%f ",sub_C[i][j]);
        //         }
                
        //     }
        //     printf("\n");
        // }
        matmul(temp_A, sub_B, sub_C, tile_size);
        // if (rank == 1)
        // {
        //     printf("my rank: %d ,iteration: %d\n", rank, iteration);
        //     for (int i = 0; i < tile_size; i++)
        //     {
        //         for (int j = 0; j < tile_size; j++)
        //         {
        //             printf("%f ",sub_C[i][j]);
        //         }
                
        //     }
        //     printf("\n");
        // }
        //no more iteration, no need to update new sub_B
        if(iteration == grid_size - 1)
            break;

        //update sub_B for next iteration
        matrix2cont(continuous_buffer, sub_B, tile_size);
        MPI_Sendrecv_replace(
            continuous_buffer, tile_size * tile_size, MPI_DOUBLE, above, 0, below, 0, 
            MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cont2matrix(continuous_buffer, sub_B, tile_size);

    }

    free(sub_A);
    free(sub_B);
    free(temp_A);
    free(continuous_buffer);
    if(rank == 0){
        // printf("my rank: %d\n", rank);
        // for (int i = 0; i < tile_size; i++)
        // {
        //     for (int j = 0; j < tile_size; j++)
        //     {
        //         printf("%f ",sub_C[i][j]);
        //     }
        // }
        //return its tile
        return_tile(sub_C, tile_size, C, start_x, start_y);
        // printf("my rank: %d\n", rank);
        // for (int i = 0; i < matrix_size; i++)
        // {
        //     for (int j = 0; j < matrix_size; j++)
        //     {
        //         printf("%f ",C[i][j]);
        //     }
        // }
        double ** tile_buffer = (double **)malloc(num_ranks * sizeof(double*));
        malloc_square(tile_buffer, tile_size * tile_size);
        //double * tile_buffer = (double *)malloc(tile_size * tile_size * sizeof(double));
        for (int i = 1; i < num_ranks; i++)
        {
            MPI_Recv(tile_buffer[i], tile_size * tile_size, MPI_DOUBLE, i, 666, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            // for (int j = 0; j < tile_size * tile_size; j++)
            // {
            //     printf("rank %d send : %f ",i, tile_buffer[i][j]);
            // }
            // printf("\n");
        }

        for (int i = 1; i < num_ranks; i++)
        {
           cont2matrix(tile_buffer[i], sub_C, tile_size);
           int x = (i % grid_size) * tile_size;
           int y = (i / grid_size) * tile_size;
           return_tile(sub_C, tile_size, C, x, y);
        }
        //free(tile_buffer);
        stop_time = MPI_Wtime();
        elapsed_time = stop_time - start_time;
        printf("Execution Time: %f\n", elapsed_time);

        //test
        for (int i = 0; i < matrix_size; i++)
        {
            for (int j = 0; j < matrix_size; j++)
            {
                printf("X:%d Y:%d Value:%f\n",i,j,C[i][j]);
            }
        }
        //free(C);
    }else{
        double * tile_buffer = (double *)malloc(tile_size * tile_size * sizeof(double));
        // if(rank == 1){
        //     for (int j = 0; j < tile_size * tile_size; j++)
        //     {
        //         printf("%f ",continuous_buffer[j]);
        //     }
        //     printf("\n");
        // }
        matrix2cont(tile_buffer, sub_C, tile_size);
        // if(rank == 1){
        //     for (int j = 0; j < tile_size * tile_size; j++)
        //     {
        //         printf("%f ",tile_buffer[j]);
        //     }
        // }
        MPI_Send(tile_buffer, tile_size * tile_size, MPI_DOUBLE, 0, 666, MPI_COMM_WORLD);
    }
    //free(sub_C);
    //MPI_Comm_free(&comm_row);
    //MPI_Comm_free(&comm_col);
    MPI_Finalize();
    return 0;
}