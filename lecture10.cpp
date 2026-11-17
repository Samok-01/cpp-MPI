// #include <stdio.h>
// #include <mpi.h>

// int main(int argc, char **argv) {
// int rank, value = 0;
// MPI_Init(&argc, &argv);
// MPI_Comm_rank(MPI_COMM_WORLD, &rank);
// if (rank == 0) {
// value = 100;
// }
// MPI_Bcast(&value, 1, MPI_INT,
// 0, MPI_COMM_WORLD);
// printf("Rank %d: value = %d\n",
// rank, value);
// MPI_Finalize();
// return 0;
// }


// #include <stdio.h>
// #include <mpi.h>
// int main(int argc, char **argv) {
// int rank, size;
// int data[8], local_data[2], result[8];
// MPI_Init(&argc, &argv);
// MPI_Comm_rank(MPI_COMM_WORLD, &rank);
// MPI_Comm_size(MPI_COMM_WORLD, &size);
// if (rank == 0)
// for (int i = 0; i < 8; ++i)
// data[i] = i + 1;
// MPI_Scatter(data, 2, MPI_INT,
// local_data, 2, MPI_INT,
// 0, MPI_COMM_WORLD);
// /* Local computation */
// for (int i = 0; i < 2; ++i)
// local_data[i] *= 2;
// MPI_Gather(local_data, 2, MPI_INT,
// result, 2, MPI_INT,
// 0, MPI_COMM_WORLD);
// if (rank == 0) {
// printf("Result: ");
// for (int i = 0; i < 8; ++i)
// printf("%d ", result[i]);
// printf("\n");
// }
// MPI_Finalize();
// return 0;
// }


