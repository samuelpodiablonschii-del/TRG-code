@echo off
setlocal EnableExtensions
title Install TRG File Handler

set "ROOT=%~dp0"
for %%F in ("Checker.exe" "UI-app.exe" "Console-app.exe" "TRG.exe") do if not exist "%ROOT%%%~F" (
    echo ERROR: %%~F was not found.
    echo Run build.bat first.
    if not defined CI pause
    exit /b 1
)

rem Program Files needs elevation. Registration is per-user (HKCU).
net session >nul 2>&1
if errorlevel 1 (
    echo Requesting permission to install TRG under Program Files...
    powershell.exe -NoProfile -ExecutionPolicy Bypass -Command "Start-Process -FilePath '%~f0' -Verb RunAs -Wait"
    exit /b
)

set "INSTALL=%ProgramFiles%\TRG Code"
if not exist "%INSTALL%" mkdir "%INSTALL%"
if errorlevel 1 goto :failed

copy /Y "%ROOT%Checker.exe" "%INSTALL%\Checker.exe" >nul
if errorlevel 1 goto :failed
copy /Y "%ROOT%UI-app.exe" "%INSTALL%\UI-app.exe" >nul
if errorlevel 1 goto :failed
copy /Y "%ROOT%Console-app.exe" "%INSTALL%\Console-app.exe" >nul
if errorlevel 1 goto :failed
copy /Y "%ROOT%TRG.exe" "%INSTALL%\TRG.exe" >nul
if errorlevel 1 goto :failed

if exist "%INSTALL%\Runtime.exe" del /F /Q "%INSTALL%\Runtime.exe" >nul 2>nul

reg add "HKCU\Software\Classes\.trg" /ve /d "TRGCode.File" /f >nul
if errorlevel 1 goto :failed
reg add "HKCU\Software\Classes\TRGCode.File" /ve /d "TRG source file" /f >nul
if errorlevel 1 goto :failed
reg add "HKCU\Software\Classes\TRGCode.File\DefaultIcon" /ve /d "\"%INSTALL%\Checker.exe\",0" /f >nul
if errorlevel 1 goto :failed
reg add "HKCU\Software\Classes\TRGCode.File\shell\open\command" /ve /d "\"%INSTALL%\Checker.exe\" \"%%1\"" /f >nul
if errorlevel 1 goto :failed
reg add "HKCU\Software\Classes\TRGCode.File\shell\Edit in TRG\command" /ve /d "\"%INSTALL%\TRG.exe\" edit \"%%1\"" /f >nul
if errorlevel 1 goto :failed

echo.
echo ========================================
echo       TRG FILE HANDLER INSTALLED
echo ========================================
echo.
echo .trg files now open through:
echo   %INSTALL%\Checker.exe
echo.
echo Checker selects UI-app.exe or Console-app.exe.
echo Right-click a .trg file and choose Edit in TRG.
echo No Windows startup entry was created.
echo.
if not defined CI pause
exit /b 0

:failed
echo.
echo ERROR: Installation failed. Check permissions and try again.
if not defined CI pause
exit /b 1
