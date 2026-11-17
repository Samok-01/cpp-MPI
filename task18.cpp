#include <iostream>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        if (rank == 0) {
            cerr << "Error: This program requires at least 2 processes!" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    int buf = 0;
    const int TAG = 0;

    if (rank == 0) {

        buf = 0;
        MPI_Send(&buf, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);

        
        MPI_Status status;
        MPI_Recv(&buf, 1, MPI_INT, size - 1, TAG, MPI_COMM_WORLD, &status);
        cout << "[" << rank << "]: receive message '" << buf << "'" << endl;
    } 

    else {
        
        MPI_Status status;
        MPI_Recv(&buf, 1, MPI_INT, rank - 1, TAG, MPI_COMM_WORLD, &status);
        cout << "[" << rank << "]: receive message '" << buf << "'" << endl;

        
        buf++;

        
        int next = (rank == size - 1) ? 0 : rank + 1;
        MPI_Send(&buf, 1, MPI_INT, next, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
