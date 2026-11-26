@echo off
setlocal EnableExtensions
title Install TRG Runtime

set "ROOT=%~dp0"
if not exist "%ROOT%Runtime.exe" (
    echo ERROR: Runtime.exe was not found.
    echo Run build.bat first to build TRG.exe and Runtime.exe.
    if not defined CI pause
    exit /b 1
)
if not exist "%ROOT%TRG.exe" (
    echo ERROR: TRG.exe was not found. Run build.bat first.
    if not defined CI pause
    exit /b 1
)

rem Program Files needs elevation. Registry registration is per-user (HKCU).
net session >nul 2>&1
if errorlevel 1 (
    echo Requesting permission to install TRG under Program Files...
    powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
    exit /b
)

set "INSTALL=%ProgramFiles%\TRG Code"
if not exist "%INSTALL%" mkdir "%INSTALL%"
if errorlevel 1 goto :failed

copy /Y "%ROOT%Runtime.exe" "%INSTALL%\Runtime.exe" >nul
if errorlevel 1 goto :failed
copy /Y "%ROOT%TRG.exe" "%INSTALL%\TRG.exe" >nul
if errorlevel 1 goto :failed

reg add "HKCU\Software\Classes\.trg" /ve /d "TRGCode.File" /f >nul
if errorlevel 1 goto :failed
reg add "HKCU\Software\Classes\TRGCode.File" /ve /d "TRG source file" /f >nul
if errorlevel 1 goto :failed
reg add "HKCU\Software\Classes\TRGCode.File\DefaultIcon" /ve /d "\"%INSTALL%\Runtime.exe\",0" /f >nul
if errorlevel 1 goto :failed
reg add "HKCU\Software\Classes\TRGCode.File\shell\open\command" /ve /d "\"%INSTALL%\Runtime.exe\" \"%%1\"" /f >nul
if errorlevel 1 goto :failed
reg add "HKCU\Software\Classes\TRGCode.File\shell\Edit in TRG\command" /ve /d "\"%INSTALL%\TRG.exe\" edit \"%%1\"" /f >nul
if errorlevel 1 goto :failed

echo.
echo ========================================
echo       TRG RUNTIME INSTALLED
echo ========================================
echo.
echo .trg files now open with:
echo   %INSTALL%\Runtime.exe
echo.
echo Right-click any .trg file and choose:
echo   Edit in TRG
echo.
echo No startup task or auto-start entry was created.
echo.
if not defined CI pause
exit /b 0

:failed
echo.
echo ERROR: Installation failed. Check permissions and try again.
if not defined CI pause
exit /b 1
