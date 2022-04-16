#include <stdio.h>

/*
The input to the problem is the mass, 
position, and velocity of each particle 
at the start of the simulation, and the 
output is the position and velocity of each
particle at a sequence of user-specified times.
*/

#define DIM 2
typedef double vect_t[DIM];

int n;
int main() {
    scanf("%d", &n);

    vect_t *forces = malloc(n * sizeof(vect_t));


    for (int step = 1; step <= n; step++) {
        forces = memset(forces, 0, n * sizeof(vect_t));

        for (int part = 0; part < n - 1; part++) {
            compute_force(part, forces)
        }
    }

    return 0;
}