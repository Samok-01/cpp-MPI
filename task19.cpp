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
        
        for (int src = 1; src < size; ++src) {
            int buf = 0;
            MPI_Status status;
            MPI_Recv(&buf, 1, MPI_INT, src, TAG, MPI_COMM_WORLD, &status);
    
            cout << "receive message '" << buf << "' from " << src << endl;
        }
    } else {
      
        int buf = rank;
        MPI_Send(&buf, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}

