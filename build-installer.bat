@echo off
setlocal

if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

if not exist "TRG-fixed.exe" (
    echo Build TRG-fixed.exe first by running build.bat.
    exit /b 1
)
if not exist "TRG-Example.trg" (
    echo Could not find TRG-Example.trg.
    exit /b 1
)

where windres >nul 2>nul
if errorlevel 1 (
    echo Could not find windres. Install MinGW-w64 and run this file again.
    exit /b 1
)
where g++ >nul 2>nul
if errorlevel 1 (
    echo Could not find g++. Install MinGW-w64 and run this file again.
    exit /b 1
)

windres TRG-Installer.rc -O coff -o trg-installer-resource.o
if errorlevel 1 exit /b 1
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -mwindows -municode TRG-Installer.cpp trg-installer-resource.o -lcomctl32 -lshell32 -ladvapi32 -lole32 -luuid -luser32 -lgdi32 -o TRG-Installer.exe
if errorlevel 1 exit /b 1
echo Built TRG-Installer.exe. It contains the runtime, icon, README, and interactive example, but no C++ source.
