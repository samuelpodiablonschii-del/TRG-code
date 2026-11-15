@echo off
setlocal EnableExtensions
cd /d "%~dp0"

if not exist "TRG-build.exe" if not exist "TRG.exe" (
    echo Building TRG first...
    call build.bat
    if errorlevel 1 exit /b 1
)

if exist "TRG-build.exe" (
    set "TRG_RUNTIME=TRG-build.exe"
) else (
    set "TRG_RUNTIME=TRG.exe"
)

for %%F in (TRG-Example.trg README.md trg.ico TRG-Installer.cpp resource.h) do (
    if not exist "%%F" (
        echo ERROR: Missing %%F.
        exit /b 1
    )
)

where windres >nul 2>nul
if errorlevel 1 (
    echo ERROR: windres was not found. Install MinGW-w64.
    exit /b 1
)
where g++ >nul 2>nul
if errorlevel 1 (
    echo ERROR: g++ was not found. Install MinGW-w64.
    exit /b 1
)

(
    echo #include "resource.h"
    echo #define IDR_RUNTIME 201
    echo #define IDR_EXAMPLE 202
    echo #define IDR_README 203
    echo #define IDR_ICON 204
    echo IDI_TRG ICON "trg.ico"
    echo IDR_RUNTIME RCDATA "%TRG_RUNTIME%"
    echo IDR_EXAMPLE RCDATA "TRG-Example.trg"
    echo IDR_README RCDATA "README.md"
    echo IDR_ICON RCDATA "trg.ico"
) > TRG-Installer-generated.rc

windres TRG-Installer-generated.rc -O coff -o trg-installer-resource.o
if errorlevel 1 exit /b 1

echo Building TRG Installer...
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -mwindows -municode TRG-Installer.cpp trg-installer-resource.o -lcomctl32 -lshell32 -ladvapi32 -lole32 -luuid -luser32 -lgdi32 -o TRG-Installer.exe
if errorlevel 1 exit /b 1

echo.
echo BUILD SUCCESS: TRG-Installer.exe
echo Runtime embedded: %TRG_RUNTIME%
exit /b 0
