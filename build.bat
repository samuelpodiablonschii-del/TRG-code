@echo off
setlocal EnableExtensions
cd /d "%~dp0"

if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

where g++ >nul 2>nul
if not errorlevel 1 goto build_mingw

where cl >nul 2>nul
if not errorlevel 1 goto build_msvc

echo ERROR: No C++ compiler found.
echo Install MinGW-w64 or Visual Studio C++ Build Tools.
exit /b 1

:build_mingw
where windres >nul 2>nul
if errorlevel 1 (
    echo ERROR: windres was not found.
    exit /b 1
)
windres TRG.rc -O coff -o trg-resource.o
if errorlevel 1 exit /b 1

echo Building TRG with MinGW...
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -mwindows -municode TRG.cpp trg-resource.o -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -lwinhttp -lbluetoothapis -o TRG-build.exe
if errorlevel 1 exit /b 1
goto publish

:build_msvc
where rc >nul 2>nul
if errorlevel 1 (
    echo ERROR: rc.exe was not found. Use a Developer Command Prompt.
    exit /b 1
)
rc /nologo /fo trg-resource.res TRG.rc
if errorlevel 1 exit /b 1
echo Building TRG with MSVC...
cl /nologo /std:c++17 /EHsc /O2 /MT TRG.cpp trg-resource.res /link /SUBSYSTEM:WINDOWS comdlg32.lib shell32.lib gdiplus.lib user32.lib gdi32.lib winhttp.lib bluetoothapis.lib /OUT:TRG-build.exe
if errorlevel 1 exit /b 1

:publish
copy /y TRG-build.exe TRG.exe >nul
if errorlevel 1 (
    echo.
    echo TRG.exe is probably still running or locked.
    echo The fresh build is available as TRG-build.exe.
    echo Close TRG and run build.bat again to replace TRG.exe.
    exit /b 0
)
del /q TRG-build.exe
echo.
echo BUILD SUCCESS: TRG.exe
exit /b 0
