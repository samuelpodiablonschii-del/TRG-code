@echo off
setlocal

if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

where g++ >nul 2>nul
if not errorlevel 1 (
    where windres >nul 2>nul
    if errorlevel 1 (
        echo Could not find windres to build the TRG application icon.
        exit /b 1
    )
    windres TRG.rc -O coff -o trg-resource.o
    if errorlevel 1 exit /b 1
    g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -mwindows -municode TRG.cpp trg-resource.o -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -o TRG.exe
    if errorlevel 1 (
        echo Could not replace TRG.exe. Building TRG-fixed.exe instead.
        g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -mwindows -municode TRG.cpp trg-resource.o -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -o TRG-fixed.exe
        if errorlevel 1 exit /b 1
        echo Close the running TRG.exe and run build.bat again to replace it.
        exit /b 0
    )
    echo Built TRG.exe with static MinGW runtime libraries.
    exit /b 0
)

where cl >nul 2>nul
if not errorlevel 1 (
    rc /nologo /fo trg-resource.res TRG.rc
    if errorlevel 1 exit /b 1
    cl /nologo /std:c++17 /EHsc /O2 /MT TRG.cpp trg-resource.res /link /SUBSYSTEM:WINDOWS comdlg32.lib shell32.lib gdiplus.lib user32.lib gdi32.lib /OUT:TRG.exe
    if errorlevel 1 (
        echo Could not replace TRG.exe. Building TRG-fixed.exe instead.
        cl /nologo /std:c++17 /EHsc /O2 /MT TRG.cpp trg-resource.res /link /SUBSYSTEM:WINDOWS comdlg32.lib shell32.lib gdiplus.lib user32.lib gdi32.lib /OUT:TRG-fixed.exe
        if errorlevel 1 exit /b 1
        echo Close the running TRG.exe and run build.bat again to replace it.
        exit /b 0
    )
    echo Built TRG.exe with the static MSVC runtime.
    exit /b 0
)

echo No C++ compiler found. Install MinGW-w64 or Visual Studio C++ Build Tools, then run build.bat again.
exit /b 1
