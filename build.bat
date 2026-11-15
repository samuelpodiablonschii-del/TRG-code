@echo off
setlocal EnableExtensions
pushd "%~dp0" || exit /b 1
title TRG - BUILD

if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

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
    echo ERROR: windres.exe was not found. Check your MinGW installation.
    goto :failed
)
echo [1/2] Building the TRG icon resource...
windres.exe TRG.rc -O coff -o trg-resource.o
if errorlevel 1 goto :failed

echo.
echo [2/2] Building the latest TRG runtime...
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -mwindows -municode TRG.cpp trg-resource.o -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -lwinhttp -lwinmm -o TRG-build.exe
if errorlevel 1 goto :failed
goto :install_latest

:build_msvc
where rc.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: rc.exe was not found. Use a Visual Studio Developer Command Prompt.
    goto :failed
)
echo [1/2] Building the TRG icon resource...
rc.exe /nologo /fo trg-resource.res TRG.rc
if errorlevel 1 goto :failed

echo.
echo [2/2] Building the latest TRG runtime...
cl.exe /nologo /std:c++17 /EHsc /O2 /MT TRG.cpp trg-resource.res /link /SUBSYSTEM:WINDOWS comdlg32.lib shell32.lib gdiplus.lib user32.lib gdi32.lib winhttp.lib winmm.lib /OUT:TRG-build.exe
if errorlevel 1 goto :failed

:install_latest
if not exist "TRG-build.exe" (
    echo ERROR: The compiler did not produce TRG-build.exe.
    goto :failed
)

echo.
echo Updating TRG.exe when it is not currently in use...
copy /Y "TRG-build.exe" "TRG.exe" >nul
if errorlevel 1 (
    echo WARNING: Windows could not replace TRG.exe, probably because it is running.
    echo The newest build is available as TRG-build.exe.
) else (
    echo Updated TRG.exe successfully.
)

echo.
echo BUILD SUCCESSFUL
echo Latest build: TRG-build.exe
echo Main app: TRG.exe
popd
endlocal
exit /b 0

:failed
echo.
echo BUILD FAILED. See the error above.
popd
endlocal
exit /b 1
