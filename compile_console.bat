@echo off
setlocal
set PATH=C:\mingw64\bin;%PATH%
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -municode -DTRG_TEST_CONSOLE TRG.cpp trg-resource.o -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -o TRG-console.exe
if errorlevel 1 (
    echo Build failed.
    exit /b 1
)
echo Built TRG-console.exe