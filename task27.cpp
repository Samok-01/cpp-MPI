#include <iostream>
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

    MPI_Comm parent = MPI_COMM_NULL;
    MPI_Comm_get_parent(&parent);

    if (parent == MPI_COMM_NULL) {
        
        cout << "I am " << rank << " process from " << size << " processes! My parent is none." << endl;

        int n = 3;
        if (argc > 1) {
            n = atoi(argv[1]);
        }

        MPI_Comm child_intercomm = MPI_COMM_NULL;
        vector<int> errcodes(n);

        // Коллективный вызов для всех родителей, root = 0 инициирует spawn
        MPI_Comm_spawn(argv[0], MPI_ARGV_NULL, n, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &child_intercomm, errcodes.data());

        if (child_intercomm != MPI_COMM_NULL) {
            MPI_Comm_disconnect(&child_intercomm);
        }
    } else {
        // Дочерние (порожденные) процессы
        cout << "I am " << rank << " process from " << size << " processes! My parent is 0." << endl;

        MPI_Comm_disconnect(&parent);
    }

    MPI_Finalize();
    return 0;
}
