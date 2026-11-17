// #include <stdio.h>
// #include <stdlib.h>
// #include <mpi.h>
// int main(int argc, char **argv) {
// int rank, size;
// const int N = 16;
// double *a = NULL, *b = NULL;

// MPI_Init(&argc, &argv);
// MPI_Comm_rank(MPI_COMM_WORLD, &rank);
// MPI_Comm_size(MPI_COMM_WORLD, &size);
// if (N % size != 0) {
// if (rank == 0)
// printf("N must be divisible by P.\n");
// MPI_Finalize(); return 0;
// }
// int local_n = N / size;
// double *local_a = static_cast<double *>(malloc(local_n * sizeof(double)));
// double *local_b = static_cast<double *>(malloc(local_n * sizeof(double)));
// if (rank == 0) {
// 	a = static_cast<double *>(malloc(N * sizeof(double)));
// 	b = static_cast<double *>(malloc(N * sizeof(double)));
// for (int i = 0; i < N; ++i) a[i] = i + 1;
// }
// MPI_Scatter(a, local_n, MPI_DOUBLE,
// local_a, local_n, MPI_DOUBLE,
// 0, MPI_COMM_WORLD);
// for (int i = 0; i < local_n; ++i)
// local_b[i] = local_a[i]*local_a[i]
// + 2.0*local_a[i];
// MPI_Gather(local_b, local_n, MPI_DOUBLE,
// b, local_n, MPI_DOUBLE,
// 0, MPI_COMM_WORLD);
// if (rank == 0) {
// for (int i = 0; i < N; ++i)
// printf("%.1f ", b[i]);
// printf("\n");
// free(a); free(b);
// }
// free(local_a); free(local_b);
// MPI_Finalize(); return 0;
// }




#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char **argv) {
int rank, size;
long long N = 100000000;
double local_sum = 0.0, global_sum;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
double h = 1.0 / (double)N;
long long start = rank * N / size;
long long end = (rank + 1) * N / size;
double t1 = MPI_Wtime();
for (long long i = start; i < end; ++i) {
double x = (i + 0.5) * h;
local_sum += 4.0 / (1.0 + x * x);
}
MPI_Reduce(&local_sum, &global_sum, 1,
MPI_DOUBLE, MPI_SUM,
0, MPI_COMM_WORLD);
double t2 = MPI_Wtime();
if (rank == 0) {
printf("pi = %.15f\n", h * global_sum);
printf("Time = %f s\n", t2 - t1);
}
MPI_Finalize(); return 0;
}