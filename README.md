# Практические работы по параллельному программированию (MPI + OpenMP на C++)

В данном репозитории собраны выполненные практические задания по параллельному программированию на C++ с использованием технологий **MPI** (Message Passing Interface) и **OpenMP**.

---

## Требования для работы на любом компьютере

1. **Среда MPI:**
   - **Microsoft MPI (MS-MPI)** (v10+): скачивается с официального сайта Microsoft (нужны `msmpisetup.exe` и `msmpisdk.msi`).
2. **Компилятор C++:**
   - **MSYS2 (UCRT64)** с установленными пакетами `gcc`, `g++`, `msmpi` (`C:\msys64\ucrt64\bin\mpic++.exe`).
   - *Или* Visual Studio (MSVC) с поддержкой OpenMP и подключенными путями к MS-MPI.
3. **Редактор:** Visual Studio Code (или Visual Studio).

---

## Список заданий

| Файл | Описание задания | Используемые механизмы |
| :--- | :--- | :--- |
| `main.cpp` | Единый файл с интерактивным выбором и запуском задач | `MPI_Bcast`, меню выбора |
| `task15.cpp` | Программа «I am!» | `MPI_Comm_rank`, `MPI_Comm_size` |
| `task16.cpp` | Программа «На первый-второй рассчитайся!» | Распределение четных/нечетных рангов |
| `task17.cpp` | Простые блокирующие обмены (точка-точка) | `MPI_Send`, `MPI_Recv` |
| `task18.cpp` | Схема «эстафетная палочка» (кольцевая передача) | `MPI_Send`, `MPI_Recv` |
| `task19.cpp` | Схема «мастер-рабочие» (Master-Slave) | `MPI_Send`, `MPI_Recv` |
| `task20.cpp` | Простые неблокирующие обмены | `MPI_Isend`, `MPI_Irecv`, `MPI_Wait` |
| `task21.cpp` | Схема «сдвиг по кольцу» | `MPI_Isend`, `MPI_Irecv`, `MPI_Waitall` |
| `task22.cpp` | Схема «каждый каждому» (All-to-All) | `MPI_Isend`, `MPI_Irecv`, `MPI_Waitall` |
| `task23.cpp` | Широковещательная рассылка и подсчет букв в строке | `MPI_Bcast` |
| `task24.cpp` | Численное вычисление числа $\pi$ методом редукции | `MPI_Bcast`, `MPI_Reduce(MPI_SUM)` |
| `task25.cpp` | Умножение квадратных матриц $A \times B = C$ | `MPI_Scatter`, `MPI_Bcast`, `MPI_Gather` |
| `task26.cpp` | Группы и коммуникаторы (рассылка только по четным) | `MPI_Comm_group`, `MPI_Group_incl`, `MPI_Comm_create` |
| `task27.cpp` | Динамическое порождение процессов (MPI-2) | `MPI_Comm_spawn`, `MPI_Comm_get_parent` |
| `task28.cpp` | Односторонние коммуникации RMA (MPI-2) | `MPI_Win_create`, `MPI_Get`, `MPI_Put`, `MPI_Win_fence` |
| `task29.cpp` | Исследование масштабируемости (замер времени) | `MPI_Wtime`, `MPI_Reduce(MPI_MAX)` |
| `task30.cpp` | Базовый гибридный проект MPI + OpenMP | `MPI_Init`, `#pragma omp parallel` |
| `task31.cpp` | Программа «I am» с гибридными нитями | MPI + OpenMP (`#pragma omp parallel num_threads`) |
| `taskfromlecture10.cpp` | Комплексное распределенное вычисление массива | `Bcast` + `Scatter` + `Gather` + `Reduce` + `Allreduce` |

---

## Как компилировать и запускать

### Способ 1. Через универсальный скрипт `run.bat` (рекомендуется)
В терминале перейдите в папку проекта и вызовите `run.bat` с номером задачи:

```cmd
.\run.bat 17          :: Запуск задания 17
.\run.bat 24          :: Запуск задания 24
.\run.bat 25          :: Запуск задания 25
.\run.bat 31          :: Запуск задания 31
.\run.bat             :: Запуск главного меню (main.cpp)
```

Можно явно переопределить количество процессов вторым аргументом:
```cmd
.\run.bat 24 8        :: Запустить задание 24 на 8 процессах
```

### Способ 2. Вручную через консоль
```powershell
$env:PATH = "C:\msys64\ucrt64\bin;C:\Program Files\Microsoft MPI\Bin;" + $env:PATH

# Компиляция (для задач с OpenMP добавить флаг -fopenmp):
mpic++ -fopenmp task31.cpp -o task31.exe

# Запуск на N процессах:
mpiexec -n 4 task31.exe
```

### Способ 3. В VS Code
- Открыть папку в VS Code.
- Нажать `Ctrl + Shift + P` -> **Tasks: Run Task** и выбрать нужную задачу из списка.
