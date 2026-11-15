@echo off
setlocal EnableExtensions
pushd "%~dp0" || exit /b 1
if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"
where g++.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: MinGW g++.exe was not found.
    popd
    endlocal
    exit /b 1
)

echo Building TRG-console.exe...
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -mconsole -DTRG_TEST_CONSOLE TRG.cpp -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -lwinhttp -lwinmm -o TRG-console.exe
if errorlevel 1 (
    echo BUILD FAILED
    popd
    endlocal
    exit /b 1
)
echo Built TRG-console.exe
popd
endlocal
exit /b 0
