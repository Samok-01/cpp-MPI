#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <mpi.h>

using namespace std;

// ==========================================================
// Задание 17. Простые блокирующие обмены (MPI_Send / MPI_Recv)
// ==========================================================
void run_task17(int rank, int size) {
    if (rank == 0) {
        cout << "\n=== [Задание 17] Блокирующий обмен (точка-точка) ===" << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (size < 2) {
        if (rank == 0) cerr << "Задание 17 требует минимум 2 процесса!" << endl;
        return;
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

    MPI_Barrier(MPI_COMM_WORLD);
}

// ==========================================================
// Задание 18. Схема «эстафетная палочка»
// ==========================================================
void run_task18(int rank, int size) {
    if (rank == 0) {
        cout << "\n=== [Задание 18] Схема «эстафетная палочка» ===" << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (size < 2) {
        if (rank == 0) cerr << "Задание 18 требует минимум 2 процесса!" << endl;
        return;
    }

    int buf = 0;
    const int TAG = 0;

    if (rank == 0) {
        buf = 0;
        MPI_Send(&buf, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);

        MPI_Status status;
        MPI_Recv(&buf, 1, MPI_INT, size - 1, TAG, MPI_COMM_WORLD, &status);
        cout << "[" << rank << "]: receive message '" << buf << "'" << endl;
    } else {
        MPI_Status status;
        MPI_Recv(&buf, 1, MPI_INT, rank - 1, TAG, MPI_COMM_WORLD, &status);
        cout << "[" << rank << "]: receive message '" << buf << "'" << endl;

        buf++;
        int next = (rank == size - 1) ? 0 : rank + 1;
        MPI_Send(&buf, 1, MPI_INT, next, TAG, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

// ==========================================================
// Задание 19. Схема «мастер-рабочие» (master-slave)
// ==========================================================
void run_task19(int rank, int size) {
    if (rank == 0) {
        cout << "\n=== [Задание 19] Схема «мастер-рабочие» ===" << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (size < 2) {
        if (rank == 0) cerr << "Задание 19 требует минимум 2 процесса!" << endl;
        return;
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

    MPI_Barrier(MPI_COMM_WORLD);
}

// ==========================================================
// Задание 20. Неблокирующие обмены (MPI_Isend / MPI_Irecv / MPI_Wait)
// ==========================================================
void run_task20(int rank, int size) {
    if (rank == 0) {
        cout << "\n=== [Задание 20] Неблокирующий обмен (точка-точка) ===" << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (size < 2) {
        if (rank == 0) cerr << "Задание 20 требует минимум 2 процесса!" << endl;
        return;
    }

    const int TAG = 0;

    if (rank == 0) {
        int buf = 45;
        MPI_Request request;
        MPI_Status status;
        MPI_Isend(&buf, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
    } else if (rank == 1) {
        int buf = 0;
        MPI_Request request;
        MPI_Status status;
        MPI_Irecv(&buf, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, &status);
        cout << "receive message '" << buf << "'" << endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);
}

// ==========================================================
// Задание 21. Схема «сдвиг по кольцу»
// ==========================================================
void run_task21(int rank, int size) {
    if (rank == 0) {
        cout << "\n=== [Задание 21] Схема «сдвиг по кольцу» ===" << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (size < 2) {
        if (rank == 0) cerr << "Задание 21 требует минимум 2 процесса!" << endl;
        return;
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

    MPI_Barrier(MPI_COMM_WORLD);
}

// ==========================================================
// Задание 22. Схема «каждый каждому» (All-to-all)
// ==========================================================
void run_task22(int rank, int size) {
    if (rank == 0) {
        cout << "\n=== [Задание 22] Схема «каждый каждому» ===" << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (size < 2) {
        if (rank == 0) cerr << "Задание 22 требует минимум 2 процесса!" << endl;
        return;
    }

    int send_buf = rank;
    vector<int> recv_buf(size, 0);

    vector<MPI_Request> send_reqs;
    vector<MPI_Request> recv_reqs;
    const int TAG = 0;

    // Инициируем неблокирующий прием от всех остальных процессов
    for (int i = 0; i < size; ++i) {
        if (i != rank) {
            MPI_Request req;
            MPI_Irecv(&recv_buf[i], 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &req);
            recv_reqs.push_back(req);
        }
    }

    // Инициируем неблокирующую отправку всем остальным процессам
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

    MPI_Barrier(MPI_COMM_WORLD);
}

// ==========================================================
// Задание 23. Широковещательная рассылка MPI_Bcast
// ==========================================================
void run_task23(int rank, int size) {
    if (rank == 0) {
        cout << "\n=== [Задание 23] Широковещательная рассылка (MPI_Bcast) ===" << endl;
    }
    MPI_Barrier(MPI_COMM_WORLD);

    int n = 0;
    char buf[100];
    memset(buf, 0, sizeof(buf));

    if (rank == 0) {
        const char* sample = "rvtabc";
        n = strlen(sample);
        strcpy(buf, sample);
        cout << "Процесс 0 рассылает строку длины " << n << ": \"" << buf << "\"" << endl;
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

    MPI_Barrier(MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int choice = 0;

    if (argc > 1) {
        choice = atoi(argv[1]);
    } else {
        if (rank == 0) {
            cout << "=============================================" << endl;
            cout << "Выберите номер задания для запуска:" << endl;
            cout << "  17 - Задание 17 (блокирующий обмен)" << endl;
            cout << "  18 - Задание 18 (эстафетная палочка)" << endl;
            cout << "  19 - Задание 19 (мастер-рабочие)" << endl;
            cout << "  20 - Задание 20 (неблокирующий обмен)" << endl;
            cout << "  21 - Задание 21 (сдвиг по кольцу)" << endl;
            cout << "  22 - Задание 22 (каждый каждому)" << endl;
            cout << "  23 - Задание 23 (MPI_Bcast подсчет букв)" << endl;
            cout << "  0  - Запустить ВСЕ задания по очереди" << endl;
            cout << "Ваш выбор: ";
            if (!(cin >> choice)) {
                choice = 0;
            }
            cout << "=============================================" << endl;
        }
        MPI_Bcast(&choice, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    switch (choice) {
        case 17: run_task17(rank, size); break;
        case 18: run_task18(rank, size); break;
        case 19: run_task19(rank, size); break;
        case 20: run_task20(rank, size); break;
        case 21: run_task21(rank, size); break;
        case 22: run_task22(rank, size); break;
        case 23: run_task23(rank, size); break;
        default:
            run_task17(rank, size);
            run_task18(rank, size);
            run_task19(rank, size);
            run_task20(rank, size);
            run_task21(rank, size);
            run_task22(rank, size);
            run_task23(rank, size);
            break;
    }

    MPI_Finalize();
    return 0;
}
