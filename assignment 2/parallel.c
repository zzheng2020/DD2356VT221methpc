#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>

#define N 1000000
#define REPS 10

double mysecond();

int main()
{
    omp_set_num_threads(32);

    int x[N];
    double t1, t2;
    double real_sum = 0.0;
    double max_sum = 0;

    // init array and find the real maxval and maxloc
    srand(time(0)); // seed
    for (int i = 0; i < N; i++)
    {
        // Generate random number between 0 and 1
        x[i] = ((double)(rand()) / RAND_MAX) * ((double)(rand()) / RAND_MAX) * ((double)(rand()) / RAND_MAX) * 1000;
        real_sum += x[i];
    }

    printf("Real sum == %f\n", real_sum);

    t1 = mysecond();

// Parallel section
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        max_sum += x[i];
        // if (i <= 20) printf("%d, %f\n", i, max_sum);
    }
    t2 = mysecond();
    printf("Time=%11.8f\nmax_sum == %f\n", (t2 - t1), max_sum);
    printf("---------------------------------------\n");

    return 0;
}

// function with timer
double mysecond()
{
    struct timeval tp;
    struct timezone tzp;
    int i;

    i = gettimeofday(&tp, &tzp);
    return ((double)tp.tv_sec + (double)tp.tv_usec * 1.e-6);
}