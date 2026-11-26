@echo off
setlocal EnableExtensions
pushd "%~dp0" >nul 2>nul
if errorlevel 1 goto :failed_no_push

title TRG - Windows Desktop Builder
set "ROOT=%~dp0"
set "OUTPUT=%ROOT%TRG-build.exe"
set "RESOURCE_OBJ=%ROOT%trg-resource.o"
set "RESOURCE_RES=%ROOT%trg-resource.res"

echo ========================================
echo       TRG - WINDOWS DESKTOP BUILD
echo ========================================
echo.

if not exist "%ROOT%TRG.cpp" (
    echo ERROR: TRG.cpp was not found. Run this script from the downloaded repository.
    goto :failed
)
if not exist "%ROOT%TRG.rc" (
    echo ERROR: TRG.rc was not found.
    goto :failed
)
if not exist "%ROOT%trg.ico" (
    echo ERROR: trg.ico was not found. The desktop app embeds this icon.
    goto :failed
)

rem Add common MinGW locations if they exist.
if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

rem Prefer MinGW when available. MSVC works from a Developer Command Prompt.
where g++.exe >nul 2>nul
if not errorlevel 1 goto :build_mingw

where cl.exe >nul 2>nul
if not errorlevel 1 goto :build_msvc

echo ERROR: No supported C++ compiler was found.
echo.
echo Choose one of these setups:
echo   1. Install MinGW-w64 and add its bin folder to PATH.
echo      Common location: C:\msys64\mingw64\bin
echo   2. Install Visual Studio Build Tools with the C++ desktop workload,
echo      then open an x64 Native Tools / Developer Command Prompt.
echo.
goto :failed

:build_mingw
echo Using MinGW:
where g++.exe
where windres.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: windres.exe was not found beside your MinGW toolchain.
    echo Install the MinGW-w64 binutils package and ensure its bin folder is on PATH.
    goto :failed
)

echo.
echo [1/2] Compiling the TRG icon resource...
windres.exe "%ROOT%TRG.rc" -O coff -o "%RESOURCE_OBJ%"
if errorlevel 1 goto :failed

echo.
echo [2/2] Compiling the Windows desktop app...
g++.exe -std=c++17 -O2 -pipe -static -static-libgcc -static-libstdc++ -mwindows -municode "%ROOT%TRG.cpp" "%RESOURCE_OBJ%" -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -lwinhttp -lwinmm -o "%OUTPUT%"
if errorlevel 1 goto :failed
goto :check_output

:build_msvc
echo Using Microsoft C++ compiler:
where cl.exe
where rc.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: rc.exe was not found.
    echo Run this script from a Visual Studio Developer Command Prompt.
    goto :failed
)

echo.
echo [1/2] Compiling the TRG icon resource...
rc.exe /nologo /fo "%RESOURCE_RES%" "%ROOT%TRG.rc"
if errorlevel 1 goto :failed

echo.
echo [2/2] Compiling the Windows desktop app...
cl.exe /nologo /std:c++17 /EHsc /O2 /MT "%ROOT%TRG.cpp" "%RESOURCE_RES%" /link /SUBSYSTEM:WINDOWS comdlg32.lib shell32.lib gdiplus.lib user32.lib gdi32.lib winhttp.lib winmm.lib /OUT:"%OUTPUT%"
if errorlevel 1 goto :failed

:check_output
if not exist "%OUTPUT%" (
    echo ERROR: The compiler did not produce TRG-build.exe.
    goto :failed
)
for %%F in ("%OUTPUT%") do if %%~zF LEQ 0 (
    echo ERROR: The generated TRG-build.exe is empty.
    goto :failed
)

echo.
echo Updating TRG.exe...
copy /Y "%OUTPUT%" "%ROOT%TRG.exe" >nul
if errorlevel 1 (
    echo WARNING: Windows could not replace TRG.exe.
    echo This normally means TRG.exe is currently running.
    echo Your new build is still available here:
    echo   %OUTPUT%
) else (
    echo Updated TRG.exe successfully.
)
copy /Y "%OUTPUT%" "%ROOT%Runtime.exe" >nul
if errorlevel 1 (
    echo WARNING: Could not update Runtime.exe. Close any running TRG runtime and rebuild.
) else (
    echo Updated Runtime.exe successfully.
)

echo.
echo ========================================
echo           BUILD SUCCESSFUL
echo ========================================
echo New executable:
echo   %OUTPUT%
if exist "%ROOT%TRG.exe" echo Main executable:
if exist "%ROOT%TRG.exe" echo   %ROOT%TRG.exe
echo.
echo Run TRG-build.exe if the existing TRG.exe was open during the build.
goto :success

:failed
echo.
echo ========================================
echo             BUILD FAILED
echo ========================================
echo The build output above contains the compiler error.
popd
if not defined CI if not defined TRG_NO_PAUSE pause
endlocal
exit /b 1

:success
popd
if not defined CI if not defined TRG_NO_PAUSE pause
endlocal
exit /b 0

:failed_no_push
echo ERROR: Could not enter the TRG repository folder.
if not defined CI if not defined TRG_NO_PAUSE pause
endlocal
exit /b 1
