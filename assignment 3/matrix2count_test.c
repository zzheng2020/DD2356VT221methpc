#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define matrix_size 4

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


int main(int argc, char* argv[])
{
    //double precision input matrix
    double **A = (double **)malloc(matrix_size * sizeof(double*));
    double **B = (double **)malloc(matrix_size * sizeof(double*));
    double **C = (double **)malloc(matrix_size * sizeof(double*));
    malloc_square(A, matrix_size);
    malloc_square(B, matrix_size);
    malloc_square(C, matrix_size);

    init_matrix(A, matrix_size, 1);
    init_matrix(B, matrix_size, 2);
    init_matrix(C, matrix_size, 3);

    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            printf("%f,%f,%f\n",A[i][j],B[i][j],C[i][j]);
        }
        
    }


    
    double * continuous_buffer = (double *)malloc(matrix_size * matrix_size * sizeof(double));
    for (int i = 0; i < matrix_size * matrix_size; i++)
    {
        printf("%f ",continuous_buffer[i]);
    }
    printf("\n");
    matrix2cont(continuous_buffer, A, matrix_size);
    for (int i = 0; i < matrix_size * matrix_size; i++)
    {
        printf("%f ",continuous_buffer[i]);
    }
    printf("\n");
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            printf("%f ",B[i][j]);
        }
        
    }
    printf("\n");
    cont2matrix(continuous_buffer, B, matrix_size);

    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            printf("%f ",B[i][j]);
        }
        
    }
    printf("%f ",B[4][4]);
    return 0;
}