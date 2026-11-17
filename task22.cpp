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

    if (size < 2) {
        if (rank == 0) {
            cerr << "Error: This program requires at least 2 processes!" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    int send_buf = rank;
    vector<int> recv_buf(size, 0);

    vector<MPI_Request> send_reqs;
    vector<MPI_Request> recv_reqs;

    const int TAG = 0;


    for (int i = 0; i < size; ++i) {
        if (i != rank) {
            MPI_Request req;
            MPI_Irecv(&recv_buf[i], 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &req);
            recv_reqs.push_back(req);
        }
    }

  
    for (int i = 0; i < size; ++i) {
        if (i != rank) {
            MPI_Request req;
            MPI_Isend(&send_buf, 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &req);
            send_reqs.push_back(req);
        }
    }


    if (!send_reqs.empty()) {
        MPI_Waitall(send_reqs.size(), send_reqs.data(), MPI_STATUSES_IGNORE);
    }
    if (!recv_reqs.empty()) {
        MPI_Waitall(recv_reqs.size(), recv_reqs.data(), MPI_STATUSES_IGNORE);
    }

    for (int i = 0; i < size; ++i) {
        if (i != rank) {
            cout << "[" << rank << "]: receive message '" << recv_buf[i] << "' from " << i << endl;
        }
    }

    MPI_Finalize();
    return 0;
}

