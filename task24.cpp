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

    // Ввод количества интервалов N процессом 0
    if (rank == 0) {
        if (argc > 1) {
            N = atoll(argv[1]);
        } else {
            if (!(cin >> N)) {
                N = 1000000000LL;
            }
        }
    }

    // Рассылаем N всем процессам
    MPI_Bcast(&N, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    double step = 1.0 / (double)N;
    double local_sum = 0.0;

    // Каждый процесс считает свою часть суммы
    for (long long i = rank; i < N; i += size) {
        double x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    double global_sum = 0.0;

    // Собираем суммы всех процессов через MPI_Reduce
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = global_sum * step;
        cout << fixed << setprecision(8) << pi << endl;
    }

    MPI_Finalize();
    return 0;
}

