#include <iostream>
#include <vector>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 0;
    vector<double> A_padded;
    vector<double> B;
    vector<double> C_gathered;

    if (rank == 0) {
        // Чтение размера матрицы n
        if (!(cin >> n)) {
            // Тестовый пример из методички
            n = 2;
            B = {5, 4, 3, 0};
            vector<double> A_orig = {1, 3, 4, 8};

            int rows_per_proc = (n + size - 1) / size;
            int total_rows = rows_per_proc * size;
            A_padded.assign(total_rows * n, 0.0);
            for (int i = 0; i < n * n; ++i) {
                A_padded[i] = A_orig[i];
            }
        } else {
            // Чтение матриц A и B
            vector<double> A_orig(n * n);
            for (int i = 0; i < n * n; ++i) {
                cin >> A_orig[i];
            }

            B.resize(n * n);
            for (int i = 0; i < n * n; ++i) {
                cin >> B[i];
            }

            int rows_per_proc = (n + size - 1) / size;
            int total_rows = rows_per_proc * size;
            A_padded.assign(total_rows * n, 0.0);
            for (int i = 0; i < n * n; ++i) {
                A_padded[i] = A_orig[i];
            }
        }
    }

    // Рассылаем размер n
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0) {
        B.resize(n * n);
    }

    // Рассылаем всю матрицу B всем процессам
    MPI_Bcast(B.data(), n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int rows_per_proc = (n + size - 1) / size;
    int elements_per_proc = rows_per_proc * n;

    vector<double> local_A(elements_per_proc, 0.0);
    vector<double> local_C(elements_per_proc, 0.0);

    // Распределяем строки матрицы A по процессам
    MPI_Scatter(A_padded.data(), elements_per_proc, MPI_DOUBLE,
                local_A.data(), elements_per_proc, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // Умножение строк матрицы A на матрицу B
    for (int r = 0; r < rows_per_proc; ++r) {
        int global_row = rank * rows_per_proc + r;
        if (global_row < n) {
            for (int col = 0; col < n; ++col) {
                double sum = 0.0;
                for (int k = 0; k < n; ++k) {
                    sum += local_A[r * n + k] * B[k * n + col];
                }
                local_C[r * n + col] = sum;
            }
        }
    }

    if (rank == 0) {
        C_gathered.resize(rows_per_proc * size * n, 0.0);
    }

    // Собираем вычисленные строки матрицы C на процессе 0
    MPI_Gather(local_C.data(), elements_per_proc, MPI_DOUBLE,
               C_gathered.data(), elements_per_proc, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    // Вывод результирующей матрицы
    if (rank == 0) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cout << C_gathered[i * n + j] << (j + 1 == n ? "" : " ");
            }
            cout << endl;
        }
    }

    MPI_Finalize();
    return 0;
}

