#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


void get_tile(int** tile, int tile_size, int** matrix, int start_col, int start_row){
    for (int row = 0; row < tile_size; row++)
    {
        for (int col = 0; col < tile_size; col++)
        {
            tile[row][col] = matrix[row + start_row][col + start_col];
        }   
    }
}

void return_tile(int** tile, int tile_size, int** matrix, int start_x, int start_y){
    for (int y = 0; y < tile_size; y++)
    {
        for (int x = 0; x < tile_size; x++)
        {
            matrix[y + start_y][x + start_x] = tile[y][x];
        }
    }
}

void malloc_square(int ** pointer, int size){
    for (int i = 0; i < size; i++)
    {
        pointer[i] = (int *)malloc(size * sizeof(int));
    }
}

int main(int argc, char* argv[]){
    int ssize = 3;
    int **matrix = (int **)malloc(ssize * sizeof(int*));
    malloc_square(matrix, ssize);

    for (int i = 0; i < ssize; i++)
    {
        for (int j = 0; j < ssize; j++)
        {
            matrix[i][j] = j;
        }
        
    }


    int size = 2;
    int **sub_matrix = (int **)malloc(ssize * sizeof(int*));
    malloc_square(sub_matrix, size);

    get_tile(sub_matrix, size, matrix, 0, 0);

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("(%d,%d):%d ", i,j,sub_matrix[i][j]);
        }
        
    }
    printf("\n");
    return_tile(sub_matrix,size, matrix,1,1);

    for (int i = 0; i < ssize; i++)
    {
        for (int j = 0; j < ssize; j++)
        {
            printf("(%d,%d):%d ", i,j,matrix[i][j]);
        }
        
    }
    return 0;
    
}