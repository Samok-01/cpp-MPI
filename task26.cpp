#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <mpi.h>

using namespace std;

int main(int argc, char** argv) {
    // 1. Инициализация среды MPI
    MPI_Init(&argc, &argv);

    int rank = 0;
    int size = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char message[11] = "no";

    // 2. Ввод данных (считывает ТОЛЬКО процесс 0 в MPI_COMM_WORLD)
    if (rank == 0) {
        if (argc > 1) {
            strncpy(message, argv[1], 10);
            message[10] = '\0';
        } else {
            string s;
            if (cin >> s) {
                strncpy(message, s.c_str(), 10);
                message[10] = '\0';
            } else {
                strcpy(message, "A");
            }
        }
    }

    // 3. Формируем список рангов только четных процессов (0, 2, 4, ...)
    vector<int> even_ranks;
    for (int i = 0; i < size; i += 2) {
        even_ranks.push_back(i);
    }

    // 4. Получаем базовую группу процессов из MPI_COMM_WORLD
    MPI_Group oldgroup;
    MPI_Comm_group(MPI_COMM_WORLD, &oldgroup);

    // 5. Создаем новую группу из четных процессов
    MPI_Group group;
    MPI_Group_incl(oldgroup, even_ranks.size(), even_ranks.data(), &group);

    // 6. Создаем новый коммуникатор для четных процессов
    //    (Внимание: MPI_Comm_create должны вызывать ВСЕ процессы из MPI_COMM_WORLD!)
    MPI_Comm newcomm;
    MPI_Comm_create(MPI_COMM_WORLD, group, &newcomm);

    // 7. Работаем с новым коммуникатором
    if (newcomm != MPI_COMM_NULL) {
        int newrank = 0;
        int newsize = 0;
        MPI_Comm_rank(newcomm, &newrank);
        MPI_Comm_size(newcomm, &newsize);

        // Рассылаем сообщение от 0-го процесса НОВОГО коммуникатора
        // всем процессам внутри newcomm
        MPI_Bcast(message, 11, MPI_CHAR, 0, newcomm);

        // Вывод для процессов, вошедших в новый коммуникатор
        cout << "MPI_COMM_WORLD: " << rank << " from " << size
             << ". New comm: " << newrank << " from " << newsize
             << ". Message = " << message << endl;

        // Освобождаем новый коммуникатор (только в процессах, где он создался)
        MPI_Comm_free(&newcomm);
    } else {
        // Вывод для нечетных процессов (не вошедших в newcomm)
        cout << "MPI_COMM_WORLD: " << rank << " from " << size
             << ". New comm: no from no. Message = no" << endl;
    }

    // 8. Освобождаем ресурсы групп (вызывают ВСЕ процессы)
    MPI_Group_free(&group);
    MPI_Group_free(&oldgroup);

    // 9. Завершаем работу с MPI
    MPI_Finalize();
    return 0;
}