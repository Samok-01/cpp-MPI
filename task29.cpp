#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long N = 0;
    if (rank == 0) {
        if (argc > 1) {
            N = atoll(argv[1]);
        } else {
            if (!(cin >> N)) {
                N = 10000000LL;
            }
        }
    }

    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Синхронизация перед началом замера
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    double step = 1.0 / (double)N;
    double local_sum = 0.0;

    for (long long i = rank; i < N; i += size) {
        double x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double elapsed = end_time - start_time;

    // Находим максимальное время среди всех процессов
    double max_elapsed = 0.0;
    MPI_Reduce(&elapsed, &max_elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = global_sum * step;
        cout << fixed << setprecision(8);
        cout << "N = " << N << ", Processes = " << size << endl;
        cout << "pi = " << pi << endl;
        cout << "Elapsed time: " << setprecision(6) << max_elapsed << " sec" << endl;
    }

    MPI_Finalize();
    return 0;
}
