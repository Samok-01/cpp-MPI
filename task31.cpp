#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <omp.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank_mpi = 0;
    int size_mpi = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);
    MPI_Comm_size(MPI_COMM_WORLD, &size_mpi);

    int n = 3;
    if (rank_mpi == 0) {
        if (argc > 1) {
            n = atoi(argv[1]);
        } else {
            if (!(cin >> n)) {
                n = 3;
            }
        }
    }

    // Рассылаем число нитей n всем процессам
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int total_hybrid = n * size_mpi;

    // По очереди для каждого процесса выводим строки нитей
    for (int p = 0; p < size_mpi; ++p) {
        if (p == rank_mpi) {
            #pragma omp parallel num_threads(n)
            {
                int rank_omp = omp_get_thread_num();

                #pragma omp critical
                {
                    cout << "I am " << rank_omp << " thread from " << rank_mpi
                         << " process. Number of hybrid threads = " << total_hybrid << endl;
                }
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
