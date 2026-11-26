@echo off
rem Backwards-compatible installer name. The installed handler is Checker.exe.
call "%~dp0install-checker.bat" %*
exit /b %errorlevel%
