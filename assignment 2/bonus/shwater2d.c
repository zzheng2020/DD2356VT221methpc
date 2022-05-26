/*
 *  shwater2d.c solves the two dimensional shallow water equations 
 *  using the Lax-Friedrich's scheme
 * 
 *  Some advice is given below
 *  - How should the work be distributed among threads
 *  - Don’t parallelize everything
 *  - What’s the difference between (Note we are using OpenMP in Fortran in this example).
 * 
 *  1. Avoid the critical region construct
 *  2. Maximize parallel regions
 *  3. Avoid parallel regions in inner loops
 * 
 *  reference: (OpenMP: 并行域内的子函数并行化)
 *  https://blog.csdn.net/Augusdi/article/details/8806160
 * 
 *  Please run with following commands and make sure you have been allocated enough core!
 *  env OMP_NUM_THREADS=1 srun -n 1 ./shwater2d.out
 *  env OMP_NUM_THREADS=2 srun -n 1 ./shwater2d.out
 *  ....
 *  env OMP_NUM_THREADS=16 srun -n 1 ./shwater2d.out
 * 
 *  You can also run with the script for changing OMP_NUM_THREADS
 * 
 *  #!/bin/csh
    foreach n (`seq 1 1 16`)
      env OMP_NUM_THREADS=$n srun -n 1 ./a.out
    end
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <omp.h>
#include <sys/time.h>

#define cell_size 3
#define xstart 0.0
#define ystart 0.0
#define xend 4.0
#define yend 4.0

#define Q(i, j, k) Q[((k) + n * ((j) + m * (i)))]

/* Timing function */
double gettime(void) {
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return tv.tv_sec + 1e-6*tv.tv_usec;
}

/* Check that the solution is finite */
void validate(double *Q, int m, int n) {
  int i, j, k;
  for (i = 0; i < n; i++) 
    for (j = 0; j < m; j++) 
      for (k = 0; k < cell_size;  k++)
	if (!isfinite(Q(k, j, i))) {
	  fprintf(stderr, "Invalid solution\n");
	  exit(-1);
	}
}

/* Flux function in the x-direction */
void fx(double *Q, double **fq, int m, int n, int j) {  
  int i;
  const double g = 9.81;

  #pragma omp for
  for (i = 0; i < m; i++) {
    fq[0][i] = Q(1, i, j);
    fq[1][i] = (pow(Q(1, i, j), 2) / Q(0, i, j)) + 
      (g * pow(Q(0, i, j), 2)) / 2.0;
    fq[2][i] = (Q(1, i, j) * Q(2, i, j)) / Q(0, i, j);
  }
}

/* Flux function in the y-direction */
void fy(double *Q, double **fq, int m, int n, int i) {
  int j;
  const double g= 9.81;
  
  #pragma omp for
  for (j = 0; j < n; j++) {
    fq[0][j] = Q(2, i, j);
    fq[1][j] = (Q(1, i, j) * Q(2, i, j)) / Q(0, i, j);    
    fq[2][j] = (pow(Q(2, i, j), 2) / Q(0, i, j)) + 
      (g * pow(Q(0, i, j), 2)) / 2.0;
  }
}


/*
  This is the Lax-Friedrich's scheme for updating volumes
  Try to parallelize it in an efficient way!
*/
void laxf_scheme_2d(double *Q, double **ffx, double **ffy, double **nFx, double **nFy,
		    int m, int n, double dx, double dy, double dt) {
    //#pragma omp parallel
    //{
      /* Calculate and update fluxes in the x-direction */
      for (int i = 1; i < n; i++) {
        //there is "omp for" in this function
        //Work sharing loop 1
        fx(Q, ffx, m, n, i);
        //Work sharing loop 2
        #pragma omp for
        for (int j = 1; j < m; j++) 
          for (int k = 0; k < cell_size;  k++) 
            nFx[k][j] = 0.5 * ((ffx[k][j-1] + ffx[k][j]) - dx/dt * (Q(k, j, i) - Q(k, j-1, i)));
        //Work sharing loop 3
        #pragma omp for
        for (int j = 1; j < m-1; j++)
          for (int k = 0; k < cell_size;  k++) 
            Q(k, j, i) = Q(k, j, i)  - dt/dx * ((nFx[k][j+1] - nFx[k][j]));
      }

      /* Calculate and update fluxes in the y-direction */
      for (int i = 1; i < m; i++) {
        //there is "omp for" in this function
        //Work sharing loop 4
        fy(Q, ffy, m, n, i);

        //Work sharing loop 5
        #pragma omp for
        for (int j = 1; j < n; j++)
          for (int k = 0; k < cell_size; k++)
            nFy[k][j] = 0.5 * ((ffy[k][j-1] + ffy[k][j]) - dy/dt * (Q(k, i, j) - Q(k, i, j -1)));
        //Work sharing loop 6
        #pragma omp for
        for (int j = 1; j <  n-1; j++) 
          for (int k = 0; k < cell_size; k++)
            Q(k,i,j) = Q(k,i,j) -  dt/dy * ((nFy[k][j+1]  -  nFy[k][j]));
      }
    //}     
}

/*
  This is the main solver routine, parallelize this. 
  But don't forget the subroutine laxf_scheme_2d
*/
void solver(double *Q, double **ffx, double **ffy, double **nFx, double **nFy,
	    int m, int n, double tend, double dx, double dy, double dt) {
  double bc_mask[3] = {1.0, -1.0, -1.0};
  #pragma omp parallel
  {
    int i, j, k, steps;
    steps = ceil(tend / dt);

    for (i = 0; i < steps; i++) {
        /* Apply boundary condition */
        #pragma omp for
        for (j = 1; j < n - 1 ; j++) {
          for (k = 0; k < cell_size; k++) {
            Q(k, 0, j) = bc_mask[k] *  Q(k, 1, j);
            Q(k, m-1, j) = bc_mask[k] *  Q(k, m-2, j);
          }
        }
        #pragma omp for
        for (j = 0; j < m; j++)  {
          for (k = 0; k < cell_size; k++) {
            Q(k, j, 0) = bc_mask[k] * Q(k, j, 1);
            Q(k, j, n-1) = bc_mask[k] * Q(k, j, n-2);
          }
        }
        /* Update all volumes with the Lax-Friedrich's scheme */     
        laxf_scheme_2d(Q, ffx, ffy, nFx, nFy, m, n, dx, dy, dt);
    }  
  }
}

void save_vtk(double *Q, double *x, double *y, int m, int n) {

  int i, j;
  FILE *fp = fopen("result.vtk", "w");

  /* Write vtk Datafile header */
  fprintf(fp, "# vtk DataFile Version 2.0\n");
  fprintf(fp, "VTK\nASCII\nDATASET POLYDATA\n");

  /* Store water height as polydata */
  fprintf(fp, "\nPOINTS %d double\n", m*n);

  for (j = 0; j < n; j++)
    for (i = 0; i < m; i++)
      fprintf(fp, "%e %e %e\n", x[i], y[j], Q(0, i, j));

  fprintf(fp,"\nVERTICES %d %d\n", n, n *(m+1));
  for (j = 0; j < n; j++)  {
    fprintf(fp, "%d ", m);
    for (i = 0; i < m; i++)
      fprintf(fp, "%d ", i+j*m);
    fprintf(fp,"\n");
  }

  /* Store lookup table */
  fprintf(fp,
	  "POINT_DATA %d\nSCALARS height double 1\nLOOKUP_TABLE default\n",m*n);
  for (j = 0; j < n; j++)
    for (i = 0; i < m; i++)
      fprintf(fp, "%e\n", Q(0, i, j));
  fclose(fp);
}

/*
  This is the main routine of the program, which allocates memory 
  and setup all parameters for the problem.
  
  You don't need to parallelize anything here!
  
  However, it might be useful to change the m and n parameters 
  during debugging
*/
int main(int argc, char **argv) {
  
  int i, j, m, n;
  double *Q;
  double *x, *y;
  double **ffx, **nFx, **ffy, **nFy;
  double dx, dt, epsi, delta, dy, tend, tmp, stime, etime;

  /* Use m volumes in the x-direction and n volumes in the y-direction */    
  m = 2000;
  n = 2000; 
  /*
    epsi      Parameter used for initial condition
    delta     Parameter used for initial condition
    dx        Distance between two volumes (x-direction)
    dy        Distance between two volumes (y-direction)
    dt        Time step
    tend      End time
  */
  epsi = 2.0;
  delta = 0.5;
  dx = (xend - xstart) / (double) m;
  dy = (yend - ystart) / (double) n;
  dt = dx / sqrt( 9.81 * 5.0);
  tend = 0.1;
  
  /* Add two ghost volumes at each side of the domain */
  m = m + 2;
  n = n + 2;

  /* Allocate memory for the domain */
  Q = (double *) malloc(m * n * cell_size *  sizeof(double));

  x = (double *) malloc(m * sizeof(double));
  y = (double *) malloc(n * sizeof(double));	

  /* Allocate memory for fluxes */
  ffx = (double **) malloc(cell_size * sizeof(double *));
  ffy = (double **) malloc(cell_size * sizeof(double *));
  nFx = (double **) malloc(cell_size * sizeof(double *));
  nFy = (double **) malloc(cell_size * sizeof(double *));

  ffx[0] = (double *) malloc(cell_size * m * sizeof(double));
  ffy[0] = (double *) malloc(cell_size * n * sizeof(double));
  nFx[0] = (double *) malloc(cell_size * m * sizeof(double));
  nFy[0] = (double *) malloc(cell_size * n * sizeof(double));

  for (i = 0; i < cell_size; i++) {
    ffx[i] =  ffx[0] + i * m;
    nFx[i] =  nFx[0] + i * m;
    ffy[i] =  ffy[0] + i * n;
    nFy[i] =  nFy[0] + i * n;
  }
    
  for (i = 0,tmp= -dx/2 + xstart; i < m; i++, tmp += dx)
    x[i] = tmp;

  for (i = 0,tmp= -dy/2 + ystart; i < n; i++, tmp += dy)
    y[i] = tmp;

  /* Set initial Gauss hump */
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      Q(0, i, j) = 4.0;
      Q(1, i, j) = 0.0;
      Q(2, i, j) = 0.0;
    }
  }

  for (i = 1; i < m-1; i++) {
    for (j = 1; j < n-1; j++) {
      Q(0, i, j) = 4.0 + epsi * exp(-(pow(x[i] - xend / 4.0, 2) + pow(y[j] - yend / 4.0, 2)) /
					  (pow(delta, 2)));
    }
  }

  stime = gettime();
  solver(Q, ffx, ffy, nFx, nFy, m, n, tend, dx, dy, dt);
  etime = gettime();

  validate(Q, m,  n);

  printf("Solver took %g seconds\n", etime - stime);


  /* Uncomment this line if you want visualize the result in ParaView */
  /* save_vtk(Q, x, y, m, n); */


  free(Q);
  free(x);
  free(y);

  free(ffx[0]);
  free(ffy[0]);
  free(nFx[0]);
  free(nFy[0]);

  free(ffx);
  free(ffy);
  free(nFx);
  free(nFy);

  return 0;    
}
