#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

double mysecond();

int main(int argc, char *argv[])
{

    //Variable declaration
    #define DIM 2
    typedef double vect_t[DIM];

    // Define parameters
    int n, X, Y, q, k, cycle, n_cycles;
    double G, delta_t;
    double x_diff, y_diff, dist, dist_cubed;

    // Define vectors
    double *masses, *force_qk, *t_e;

    // Define continous memory arrays
    vect_t *forces, *pos, *vel;

    // Define parameters for the timer
    int times;
    double start, end, totaltime, mean, std_dev;

    //Allocation
    // Assign constant values
    n = 2000;
    X = 0;
    Y = 1;
    G = 6.67408 * pow(10, -11);
    delta_t = 0.05;
    n_cycles = 100;
    times = 10;

    // Allocate space
    masses = (double *)malloc(n * sizeof(double));
    force_qk = (double *)malloc(2 * sizeof(double));
    forces = malloc(n * sizeof(vect_t));
    pos = malloc(n * sizeof(vect_t));
    vel = malloc(n * sizeof(vect_t));
    t_e = (double*)malloc(times*sizeof(double));

    //Initialize particle
    for (int i = 0; i < n; i++) {
        //position
        pos[i][X] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        pos[i][Y] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        //velocity
        vel[i][X] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        vel[i][Y] = (rand() / (double)(RAND_MAX)) * 2 - 1;
        //mass
        masses[i] = fabs((rand() / (double)(RAND_MAX)) * 2 - 1);
    }

    //avoid cold start, only entirely execute once is enough
    for (q = 0; q < n; q++){
        {
            for (k = 0; k < n; k++)
            {
                if (k != q)
                {
                    x_diff = pos[q][X] - pos[k][X];
                    y_diff = pos[q][Y] - pos[k][Y];
                    dist = sqrt(x_diff * x_diff + y_diff * y_diff);
                    dist_cubed = dist * dist * dist;
                    forces[q][X] -= G * masses[q] * masses[k] / dist_cubed * x_diff;
                    forces[q][Y] -= G * masses[q] * masses[k] / dist_cubed * y_diff;
                }
            }
        }


        // Compute the position
        for (q = 0; q < n; q++)
        {
            vel[q][X] += delta_t / masses[q] * forces[q][X];
            vel[q][Y] += delta_t / masses[q] * forces[q][Y];
            pos[q][X] += delta_t * vel[q][X];
            pos[q][Y] += delta_t * vel[q][Y];
        }
    }


    totaltime = 0.0;
    for (int i = 0; i < times; i++)
    {
        start = mysecond();
	//simple algorithm//
        for (cycle = 1; cycle <= n_cycles; cycle++)
        {
            for (q = 0; q < n; q++)
            {
                for (k = 0; k < n; k++)
                {
                    if (k != q)
                    {
                        x_diff = pos[q][X] - pos[k][X];
                        y_diff = pos[q][Y] - pos[k][Y];
                        dist = sqrt(x_diff * x_diff + y_diff * y_diff);
                        dist_cubed = dist * dist * dist;
                        forces[q][X] -= G * masses[q] * masses[k] / dist_cubed * x_diff;
                        forces[q][Y] -= G * masses[q] * masses[k] / dist_cubed * y_diff;
                    }
                }
            }
            // Compute the position
            for (q = 0; q < n; q++)
            {
                vel[q][X] += delta_t / masses[q] * forces[q][X];
                vel[q][Y] += delta_t / masses[q] * forces[q][Y];
                pos[q][X] += delta_t * vel[q][X];
                pos[q][Y] += delta_t * vel[q][Y];
            }
        }
        end = mysecond();

        //caculate
        t_e[i]=end-start;
        totaltime+=t_e[i];
    }

    mean=totaltime/(float)times;
    std_dev = 0.0;
    for (int k = 0; k < times; k++) {
        printf("Execution time iteration %d: %11.8f s\n", k+1, t_e[k]);
        std_dev += pow(t_e[k]-mean, 2);
    }
    printf("Mean: %11.8f s \n", mean);
    printf("Std deviation: %11.8f s \n", sqrt(std_dev/times));
    
    double n_operations = n_cycles * n * (n-1) * 18 + n * 10;
    double n_read_write = n_cycles * n * (n-1) * 10 + n * 11;
    printf("Performance Model: %11.8f s \n", n_operations /(3200*pow(10,6)));
    printf("Performance Model with memory: %11.8f s \n", n_operations / (3200*pow(10,6)) + n_read_write*8/(30100*pow(10,6)));
    return 0;
}

double mysecond(){
  struct timeval tp;
  struct timezone tzp;
  int i;

  i = gettimeofday(&tp,&tzp);
  return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}
