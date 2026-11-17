#include <iostream>
#include <algorithm>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0) {
            cerr << "Error: This assignment requires exactly 4 MPI processes (-n 4)!" << endl;
        }
        MPI_Finalize();
        return 1;
    }

    // Step 1: Broadcast a Multiplier
    int multiplier = 0;
    if (rank == 0) {
        multiplier = 3;
    }

    MPI_Bcast(&multiplier, 1, MPI_INT, 0, MPI_COMM_WORLD);
    cout << "Rank " << rank << ": multiplier = " << multiplier << endl;

    MPI_Barrier(MPI_COMM_WORLD);

    // Step 2: Scatter the Array
    int data[8];
    int local_data[2];

    if (rank == 0) {
        int initial_data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        for (int i = 0; i < 8; ++i) {
            data[i] = initial_data[i];
        }
    }

    // Разделяем массив: каждому процессу по 2 элемента
    MPI_Scatter(data, 2, MPI_INT, local_data, 2, MPI_INT, 0, MPI_COMM_WORLD);


    // Step 3: Local Computation

    // Каждый процесс умножает свои элементы на multiplier
    for (int i = 0; i < 2; ++i) {
        local_data[i] *= multiplier;
    }

    
    // Step 4: Gather Results
    int gathered_data[8];
    MPI_Gather(local_data, 2, MPI_INT, gathered_data, 2, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "\nGathered on Rank 0: ";
        for (int i = 0; i < 8; ++i) {
            cout << gathered_data[i] << " ";
        }
        cout << endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);


    // Step 5: Global Sum & Max via Reduce
    
    int local_sum = local_data[0] + local_data[1];
    int local_max = max(local_data[0], local_data[1]);

    int global_sum = 0;
    int global_max = 0;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "\n[MPI_Reduce on Rank 0]:" << endl;
        cout << "Global sum = " << global_sum << endl;
        cout << "Global max = " << global_max << endl;
        cout << endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);


    // Step 6: Allreduce for All
    
    int allreduce_sum = 0;
    MPI_Allreduce(&local_sum, &allreduce_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    cout << "Rank " << rank << ": Global sum = " << allreduce_sum << endl;

    MPI_Finalize();
    return 0;
}

