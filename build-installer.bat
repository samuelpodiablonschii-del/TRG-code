@echo off
setlocal EnableExtensions
pushd "%~dp0"
if errorlevel 1 (
    echo ERROR: Could not enter the TRG project folder.
    goto :failed_no_push
)
title TRG - BUILD INSTALLER

if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

echo [1/3] Building the TRG desktop apps and Checker...
set "TRG_NO_PAUSE=1"
call build.bat
if errorlevel 1 (
    echo ERROR: The desktop build failed. Installer build cancelled.
    goto :failed
)
set "TRG_NO_PAUSE="

if not exist "TRG-build.exe" (
    echo ERROR: TRG-build.exe was not produced by build.bat.
    goto :failed
)
for %%F in ("TRG-build.exe" "UI-app.exe" "Console-app.exe" "Checker.exe") do if not exist "%%~F" (
    echo ERROR: Required application was not built: %%~F.
    goto :failed
)
for %%F in ("TRG-build.exe" "UI-app.exe" "Console-app.exe" "Checker.exe") do if %%~zF LEQ 0 (
    echo ERROR: TRG-build.exe is empty.
    goto :failed
)
for %%F in ("TRG-Example.trg" "2DEditor.trg" "3DEditor.trg" "trg.ico" "README.md" "TRG-Installer.rc" "TRG-Installer.cpp" "resource.h") do if not exist "%%~F" (
    echo ERROR: Required installer file is missing: %%~F
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
echo.
echo [2/3] Embedding the fresh runtime and installer assets...
windres.exe TRG-Installer.rc -O coff -o trg-installer-resource.o
if errorlevel 1 goto :failed

echo.
echo [3/3] Building TRG-Installer.exe...
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ -mwindows -municode TRG-Installer.cpp trg-installer-resource.o -lcomctl32 -lshell32 -ladvapi32 -lole32 -luuid -luser32 -lgdi32 -o TRG-Installer.exe
if errorlevel 1 goto :failed
goto :success

:build_msvc
where rc.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: rc.exe was not found. Use a Visual Studio Developer Command Prompt.
    goto :failed
)
echo.
echo [2/3] Embedding the fresh runtime and installer assets...
rc.exe /nologo /fo trg-installer-resource.res TRG-Installer.rc
if errorlevel 1 goto :failed

echo.
echo [3/3] Building TRG-Installer.exe...
cl.exe /nologo /std:c++17 /EHsc /O2 /MT TRG-Installer.cpp trg-installer-resource.res /link /SUBSYSTEM:WINDOWS comctl32.lib shell32.lib advapi32.lib ole32.lib uuid.lib user32.lib gdi32.lib /OUT:TRG-Installer.exe
if errorlevel 1 goto :failed

:success
if not exist "TRG-Installer.exe" (
    echo ERROR: The installer executable was not produced.
    goto :failed
)
echo.
echo BUILD SUCCESSFUL
echo Output: TRG-Installer.exe
echo Embedded files: Checker.exe, UI-app.exe, Console-app.exe, TRG-build.exe
goto :finish_success

:failed
echo.
echo BUILD FAILED. The window will remain open so you can read the error.
goto :finish_failed

:finish_success
popd
if not defined CI pause
endlocal
exit /b 0

:finish_failed
popd
if not defined CI pause
endlocal
exit /b 1

:failed_no_push
echo ERROR: Could not enter the TRG project folder.
pause
endlocal
exit /b 1
