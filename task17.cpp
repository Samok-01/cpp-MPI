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

    const int TAG = 0;

    if (rank == 0) {
        int message = 45;
        
        MPI_Send(&message, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
    } else if (rank == 1) {
        int message = 0;
        MPI_Status status;
        
        MPI_Recv(&message, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
        cout << "receive message '" << message << "'" << endl;
    }

    MPI_Finalize();
    return 0;
}
