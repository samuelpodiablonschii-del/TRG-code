@echo off
setlocal EnableExtensions
pushd "%~dp0" || exit /b 1
title TRG - BUILD INSTALLER

if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

if not exist "TRG-build.exe" (
    echo ERROR: TRG-build.exe was not found.
    echo Run build.bat immediately before building the installer.
    goto :failed
)
if not exist "TRG-Example.trg" (
    echo ERROR: TRG-Example.trg is missing.
    goto :failed
)
if not exist "trg.ico" (
    echo ERROR: trg.ico is missing.
    goto :failed
)
if not exist "README.md" (
    echo ERROR: README.md is missing.
    goto :failed
)

where g++.exe >nul 2>nul
if not errorlevel 1 goto :build_mingw

where cl.exe >nul 2>nul
if not errorlevel 1 goto :build_msvc

echo ERROR: No C++ compiler was found.
echo Install MinGW-w64 or open a Visual Studio Developer Command Prompt.
goto :failed

:build_mingw
where windres.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: windres.exe was not found.
    goto :failed
)
echo Building installer resources with the fresh TRG-build.exe...
windres.exe TRG-Installer.rc -O coff -o trg-installer-resource.o
if errorlevel 1 goto :failed

echo Building TRG-Installer.exe...
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -mwindows -municode TRG-Installer.cpp trg-installer-resource.o -lcomctl32 -lshell32 -ladvapi32 -lole32 -luuid -luser32 -lgdi32 -o TRG-Installer.exe
if errorlevel 1 goto :failed
goto :success

:build_msvc
where rc.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: rc.exe was not found. Use a Visual Studio Developer Command Prompt.
    goto :failed
)
echo Building installer resources with the fresh TRG-build.exe...
rc.exe /nologo /fo trg-installer-resource.res TRG-Installer.rc
if errorlevel 1 goto :failed

echo Building TRG-Installer.exe...
cl.exe /nologo /std:c++17 /EHsc /O2 /MT TRG-Installer.cpp trg-installer-resource.res /link /SUBSYSTEM:WINDOWS comctl32.lib shell32.lib advapi32.lib ole32.lib uuid.lib user32.lib gdi32.lib /OUT:TRG-Installer.exe
if errorlevel 1 goto :failed

:success
echo.
echo BUILD SUCCESSFUL
echo Output: TRG-Installer.exe
echo Embedded runtime: TRG-build.exe
popd
endlocal
exit /b 0

:failed
echo.
echo BUILD FAILED. See the error above.
popd
endlocal
exit /b 1
