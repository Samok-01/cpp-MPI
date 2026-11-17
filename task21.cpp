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

    int send_buf = rank;
    int recv_buf = 0;

    int dest = (rank + 1) % size;
    int src = (rank - 1 + size) % size;

    MPI_Request reqs[2];
    MPI_Status stats[2];
    const int TAG = 0;

    
    MPI_Isend(&send_buf, 1, MPI_INT, dest, TAG, MPI_COMM_WORLD, &reqs[0]);
    
    MPI_Irecv(&recv_buf, 1, MPI_INT, src, TAG, MPI_COMM_WORLD, &reqs[1]);

    MPI_Waitall(2, reqs, stats);

    cout << "[" << rank << "]: receive message '" << recv_buf << "'" << endl;

    MPI_Finalize();
    return 0;
}

