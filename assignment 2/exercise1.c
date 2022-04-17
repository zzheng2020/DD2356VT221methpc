// -fopenmp
#include <omp.h>
#include <stdio.h>

int main() {

  #pragma omp parallel num_threads(3)
  {
    printf("Hello World from Thread %d!\n", omp_get_thread_num());
  }

  return 0;
}