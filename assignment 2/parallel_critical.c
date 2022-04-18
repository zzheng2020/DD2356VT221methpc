#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>

#define N 1000000
#define REPS 10
#define MAX_THREADS 32

double mysecond();

int main()
{

    omp_set_num_threads(32);

    int x[N];
    double t1, t2;
    double real_sum = 0.0;
    double sum = 0;

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

    double sumvals[MAX_THREADS], sum_val;

// Parallel section
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        sumvals[id] = 0;
#pragma omp for
        for (int i = 0; i < N; i++)
        {
            sumvals[id] += x[i];
        }
    }


    for (int i = 0; i < MAX_THREADS; i++)
    {
        sum += sumvals[i];
    }

    t2 = mysecond();
    printf("Time=%11.8f\nSum == %f\n", (t2 - t1), sum);
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