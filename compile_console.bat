@echo off
setlocal EnableExtensions
cd /d "%~dp0"

if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

where g++ >nul 2>nul
if errorlevel 1 (
    echo ERROR: g++ was not found.
    exit /b 1
)

echo Building the TRG console test runner...
g++ -std=c++17 -O2 -DTRG_TEST_CONSOLE -mconsole TRG.cpp -lwinhttp -lbluetoothapis -o TRG-console.exe
if errorlevel 1 (
    echo BUILD FAILED.
    exit /b 1
)
echo BUILD SUCCESS: TRG-console.exe
