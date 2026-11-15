@echo off
set PATH=C:\mingw64\bin;%PATH%
cd /d "C:\Users\ipodi\OneDrive\Desktop\TRG code"
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -municode -mconsole -DTRG_TEST_CONSOLE TRG.cpp trg-resource.o -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -o "C:\Temp\TRG-console.exe"
if errorlevel 1 (
    echo BUILD FAILED
    exit /b 1
)
echo BUILD SUCCESS