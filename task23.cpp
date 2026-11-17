#include <iostream>
#include <cstring>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 0;
    char buf[100];
    memset(buf, 0, sizeof(buf));

   
    if (rank == 0) {
        if (argc >= 3) {
            n = atoi(argv[1]);
            strncpy(buf, argv[2], 99);
        } else if (argc == 2) {
            strncpy(buf, argv[1], 99);
            n = strlen(buf);
        } else {
            cout << "Enter n and a string (for example: 9 aaaaaaaaa or 6 rvtabc):" << endl;
            if (!(cin >> n >> buf)) {
        
                n = 6;
                strcpy(buf, "rvtabc");
            }
        }
    }


    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);


    MPI_Bcast(buf, n, MPI_CHAR, 0, MPI_COMM_WORLD);
    buf[n] = '\0';


    char a = 'a';
    for (int i = rank; i < 26; i += size) {
        char current_char = (char)((int)a + i);
        int count = 0;

        for (int j = 0; j < n; ++j) {
            if (buf[j] == current_char) {
                count++;
            }
        }


        if (count > 0) {
            cout << current_char << " = " << count << endl;
        }
    }

    MPI_Finalize();
    return 0;
}

