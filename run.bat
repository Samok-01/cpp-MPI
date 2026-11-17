@echo off
setlocal enabledelayedexpansion

set "PATH=C:\msys64\ucrt64\bin;C:\Program Files\Microsoft MPI\Bin;%PATH%"

set "TARGET=%~1"
set "CUSTOM_NP=%~2"
set "OPENMP_FLAG="

if "%TARGET%"=="" set "TARGET=main"

set "FILE=%TARGET%.cpp"
set "EXE=%TARGET%.exe"
set "NP=4"

if "%TARGET%"=="main" set "FILE=main.cpp" & set "EXE=mpi.exe" & set "NP=4"
if "%TARGET%"=="15" set "FILE=task15.cpp" & set "EXE=task15.exe" & set "NP=3"
if "%TARGET%"=="16" set "FILE=task16.cpp" & set "EXE=task16.exe" & set "NP=4"
if "%TARGET%"=="17" set "FILE=task17.cpp" & set "EXE=task17.exe" & set "NP=2"
if "%TARGET%"=="18" set "FILE=task18.cpp" & set "EXE=task18.exe" & set "NP=4"
if "%TARGET%"=="19" set "FILE=task19.cpp" & set "EXE=task19.exe" & set "NP=4"
if "%TARGET%"=="20" set "FILE=task20.cpp" & set "EXE=task20.exe" & set "NP=2"
if "%TARGET%"=="21" set "FILE=task21.cpp" & set "EXE=task21.exe" & set "NP=4"
if "%TARGET%"=="22" set "FILE=task22.cpp" & set "EXE=task22.exe" & set "NP=3"
if "%TARGET%"=="23" set "FILE=task23.cpp" & set "EXE=task23.exe" & set "NP=3"
if "%TARGET%"=="24" set "FILE=task24.cpp" & set "EXE=task24.exe" & set "NP=4"
if "%TARGET%"=="25" set "FILE=task25.cpp" & set "EXE=task25.exe" & set "NP=2"
if "%TARGET%"=="26" set "FILE=task26.cpp" & set "EXE=task26.exe" & set "NP=3"
if "%TARGET%"=="27" set "FILE=task27.cpp" & set "EXE=task27.exe" & set "NP=2"
if "%TARGET%"=="28" set "FILE=task28.cpp" & set "EXE=task28.exe" & set "NP=4"
if "%TARGET%"=="29" set "FILE=task29.cpp" & set "EXE=task29.exe" & set "NP=4"
if "%TARGET%"=="30" set "FILE=task30.cpp" & set "EXE=task30.exe" & set "NP=2" & set "OPENMP_FLAG=-fopenmp"
if "%TARGET%"=="31" set "FILE=task31.cpp" & set "EXE=task31.exe" & set "NP=2" & set "OPENMP_FLAG=-fopenmp"
if /i "%TARGET%"=="example9lecture" set "FILE=example9lecture.cpp" & set "EXE=example9lecture.exe" & set "NP=2"
if /i "%TARGET%"=="taskfromlecture10" set "FILE=taskfromlecture10.cpp" & set "EXE=taskfromlecture10.exe" & set "NP=4"

if not "%CUSTOM_NP%"=="" set "NP=%CUSTOM_NP%"

echo =======================================================
echo [1/2] Compiling %FILE% -> %EXE%...
echo =======================================================
call C:\msys64\ucrt64\bin\mpic++.exe %OPENMP_FLAG% "%~dp0%FILE%" -o "%~dp0%EXE%"
if errorlevel 1 (
    echo [ERROR] Compilation failed!
    pause
    exit /b 1
)

echo.
echo =======================================================
echo [2/2] Running %EXE% (-n %NP%)...
echo =======================================================
call "C:\Program Files\Microsoft MPI\Bin\mpiexec.exe" -n %NP% "%~dp0%EXE%"
echo.
echo =======================================================

pause
