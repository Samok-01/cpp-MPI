#include <iostream>
#include <mpi.h>
#include <omp.h>

using namespace std;

int main(int argc, char** argv) {
    // Инициализация гибридной среды MPI + OpenMP
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    #pragma omp parallel
    {
        #pragma omp single
        {
            cout << "MPI Process " << rank << " of " << size
                 << " running with " << omp_get_num_threads()
                 << " OpenMP threads." << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
