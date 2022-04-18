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

    int x[N];
    double t1, t2;
    double maxval, realmaxval = 0.0;
    int maxloc, realmaxloc = 0;

    // init array and find the real maxval and maxloc
    srand(time(0)); // seed
    for (int i = 0; i < N; i++)
    {
        // Generate random number between 0 and 1
        x[i] = ((double)(rand()) / RAND_MAX) * ((double)(rand()) / RAND_MAX) * ((double)(rand()) / RAND_MAX) * 1000;
        if (x[i] > realmaxval)
        {
            realmaxval = x[i];
            realmaxloc = i;
        }
    }

    printf("Real maxval = %f\nReal maxloc = %d\n", realmaxval, realmaxloc);

    t1 = mysecond();

// Parallel section
#pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        if (x[i] > maxval)
        {
            maxval = x[i];
            maxloc = i;
        }
    }

    t2 = mysecond();
    printf("Time=%11.8f\nFound maxval=%f\nFound maxloc=%d\n", (t2 - t1), maxval, maxloc);
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