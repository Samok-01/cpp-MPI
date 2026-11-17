#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Процесс 0 выводит общее количество процессов
        cout << size << " processes." << endl;
    } else {
        // Расчет на первый-второй:
        // Процесс 1 - FIRST, процесс 2 - SECOND, процесс 3 - FIRST и т.д.
        if (rank % 2 == 1) {
            cout << "I am " << rank << " process: FIRST!" << endl;
        } else {
            cout << "I am " << rank << " process: SECOND!" << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
