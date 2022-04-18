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

    int x[N];
    double t1, t2;
    double realmaxval = 0.0;
    int realmaxloc = 0;

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

    double maxvals[MAX_THREADS], maxval;
    int maxlocs[MAX_THREADS], maxloc;

    t1 = mysecond();

// Parallel section
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        maxvals[id] = -1.0e30;
#pragma omp for
        for (int i = 0; i < N; i++)
        {
            if (x[i] > maxvals[id])
            {
                maxvals[id] = x[i];
                maxlocs[id] = i;
            }
        }
    }

    maxloc = maxlocs[0];
    maxval = maxvals[0];
    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (maxvals[i] > maxval)
        {
            maxval = maxvals[i];
            maxloc = maxlocs[i];
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