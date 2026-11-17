#include <iostream>
#include <iomanip>
#include <vector>
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
                N = 1000000000LL;
            }
        }
    }

    // Окно для передачи параметра N
    MPI_Win win_N;
    MPI_Win_create(&N, sizeof(long long), sizeof(long long), MPI_INFO_NULL, MPI_COMM_WORLD, &win_N);

    MPI_Win_fence(0, win_N);
    if (rank != 0) {
        // Процессы считывают N из памяти процесса 0 через MPI_Get
        MPI_Get(&N, 1, MPI_LONG_LONG, 0, 0, 1, MPI_LONG_LONG, win_N);
    }
    MPI_Win_fence(0, win_N);
    MPI_Win_free(&win_N);

    // Локальные вычисления
    double step = 1.0 / (double)N;
    double local_sum = 0.0;

    for (long long i = rank; i < N; i += size) {
        double x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    // Окно на процессе 0 для сбора частичных сумм от всех процессов
    vector<double> results;
    MPI_Aint win_size = (rank == 0) ? (size * sizeof(double)) : 0;
    if (rank == 0) {
        results.resize(size, 0.0);
    }

    MPI_Win win_res;
    MPI_Win_create(rank == 0 ? results.data() : NULL, win_size, sizeof(double), MPI_INFO_NULL, MPI_COMM_WORLD, &win_res);

    MPI_Win_fence(0, win_res);
    // Каждый процесс отправляет свою сумму в ячейку с номером rank на процессе 0 через MPI_Put
    MPI_Put(&local_sum, 1, MPI_DOUBLE, 0, rank, 1, MPI_DOUBLE, win_res);
    MPI_Win_fence(0, win_res);

    if (rank == 0) {
        double global_sum = 0.0;
        for (int i = 0; i < size; ++i) {
            global_sum += results[i];
        }
        double pi = global_sum * step;
        cout << fixed << setprecision(8) << pi << endl;
    }

    MPI_Win_free(&win_res);

    MPI_Finalize();
    return 0;
}
