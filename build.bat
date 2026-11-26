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

for %%F in ("TRG.cpp" "Checker.cpp" "TRG.rc" "trg.ico") do if not exist "%ROOT%%%~F" (
    echo ERROR: Required build file is missing: %%~F
    goto :failed
)

if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"
if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"

where g++.exe >nul 2>nul
if not errorlevel 1 goto :build_mingw

where cl.exe >nul 2>nul
if not errorlevel 1 goto :build_msvc

echo ERROR: No supported C++ compiler was found.
echo Install MinGW-w64 or open a Visual Studio Developer Command Prompt.
goto :failed

:build_mingw
echo Using MinGW:
where g++.exe
where windres.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: windres.exe was not found.
    goto :failed
)

echo.
echo [1/4] Compiling the TRG icon resource...
windres.exe "%ROOT%TRG.rc" -O coff -o "%RESOURCE_OBJ%"
if errorlevel 1 goto :failed

echo.
echo [2/4] Building UI-app.exe...
g++.exe -std=c++17 -O2 -pipe -static -static-libgcc -static-libstdc++ -mwindows -municode -DTRG_FORCE_UI_APP "%ROOT%TRG.cpp" "%RESOURCE_OBJ%" -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -lwinhttp -lwinmm -o "%OUTPUT%"
if errorlevel 1 goto :failed
copy /Y "%OUTPUT%" "%ROOT%UI-app.exe" >nul
if errorlevel 1 goto :failed
copy /Y "%OUTPUT%" "%ROOT%TRG.exe" >nul
if errorlevel 1 (
    echo WARNING: Could not update TRG.exe. Close TRG Studio and rebuild.
)

echo.
echo [3/4] Building Console-app.exe...
g++.exe -std=c++17 -O2 -pipe -static -static-libgcc -static-libstdc++ -mwindows -municode -DTRG_FORCE_CONSOLE_APP "%ROOT%TRG.cpp" "%RESOURCE_OBJ%" -lcomdlg32 -lshell32 -lgdiplus -lgdi32 -luser32 -lwinhttp -lwinmm -o "%ROOT%Console-app.exe"
if errorlevel 1 goto :failed

echo.
echo [4/4] Building Checker.exe...
g++.exe -std=c++17 -O2 -pipe -static -static-libgcc -static-libstdc++ -mwindows -municode "%ROOT%Checker.cpp" "%RESOURCE_OBJ%" -lshell32 -luser32 -o "%ROOT%Checker.exe"
if errorlevel 1 goto :failed
goto :check_output

:build_msvc
echo Using Microsoft C++ compiler:
where cl.exe
where rc.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: rc.exe was not found. Use a Visual Studio Developer Command Prompt.
    goto :failed
)

echo.
echo [1/4] Compiling the TRG icon resource...
rc.exe /nologo /fo "%RESOURCE_RES%" "%ROOT%TRG.rc"
if errorlevel 1 goto :failed

echo.
echo [2/4] Building UI-app.exe...
cl.exe /nologo /std:c++17 /EHsc /O2 /MT /DTRG_FORCE_UI_APP "%ROOT%TRG.cpp" "%RESOURCE_RES%" /link /SUBSYSTEM:WINDOWS comdlg32.lib shell32.lib gdiplus.lib user32.lib gdi32.lib winhttp.lib winmm.lib /OUT:"%OUTPUT%"
if errorlevel 1 goto :failed
copy /Y "%OUTPUT%" "%ROOT%UI-app.exe" >nul
if errorlevel 1 goto :failed
copy /Y "%OUTPUT%" "%ROOT%TRG.exe" >nul
if errorlevel 1 (
    echo WARNING: Could not update TRG.exe. Close TRG Studio and rebuild.
)

echo.
echo [3/4] Building Console-app.exe...
cl.exe /nologo /std:c++17 /EHsc /O2 /MT /DTRG_FORCE_CONSOLE_APP "%ROOT%TRG.cpp" "%RESOURCE_RES%" /link /SUBSYSTEM:WINDOWS comdlg32.lib shell32.lib gdiplus.lib user32.lib gdi32.lib winhttp.lib winmm.lib /OUT:"%ROOT%Console-app.exe"
if errorlevel 1 goto :failed

echo.
echo [4/4] Building Checker.exe...
cl.exe /nologo /std:c++17 /EHsc /O2 /MT "%ROOT%Checker.cpp" "%RESOURCE_RES%" /link /SUBSYSTEM:WINDOWS shell32.lib user32.lib /OUT:"%ROOT%Checker.exe"
if errorlevel 1 goto :failed

:check_output
for %%F in ("%ROOT%TRG-build.exe" "%ROOT%TRG.exe" "%ROOT%UI-app.exe" "%ROOT%Console-app.exe" "%ROOT%Checker.exe") do (
    if not exist "%%~F" (
        echo ERROR: The build did not produce %%~nxF.
        goto :failed
    )
    for %%S in ("%%~F") do if %%~zS LEQ 0 (
        echo ERROR: %%~nxF is empty.
        goto :failed
    )
)

echo.
echo ========================================
echo           BUILD SUCCESSFUL
echo ========================================
echo Created:
echo   %ROOT%Checker.exe
echo   %ROOT%UI-app.exe
echo   %ROOT%Console-app.exe
echo   %ROOT%TRG.exe  (editor)
echo.
echo Double-click a .trg file after installing the file handler.
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
