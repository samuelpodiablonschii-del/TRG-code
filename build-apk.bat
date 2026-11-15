@echo off
setlocal EnableExtensions
pushd "%~dp0"
if errorlevel 1 (
    echo ERROR: Could not enter the TRG project folder.
    goto :failed_no_push
)
title TRG - ANDROID APK BUILDER

set "GRADLEW=%~dp0android-app\gradlew.bat"
set "APK=%~dp0android-app\app\build\outputs\apk\debug\app-debug.apk"

if not exist "%GRADLEW%" (
    echo ERROR: Android Gradle wrapper was not found:
    echo %GRADLEW%
    goto :failed
)

if defined JAVA_HOME if exist "%JAVA_HOME%\bin\java.exe" goto :java_ready
set "JAVA_HOME="
if exist "%ProgramFiles%\Android\Android Studio\jbr\bin\java.exe" (
    set "JAVA_HOME=%ProgramFiles%\Android\Android Studio\jbr"
    set "PATH=%JAVA_HOME%\bin;%PATH%"
    goto :java_ready
)
if exist "%LOCALAPPDATA%\Programs\Android Studio\jbr\bin\java.exe" (
    set "JAVA_HOME=%LOCALAPPDATA%\Programs\Android Studio\jbr"
    set "PATH=%JAVA_HOME%\bin;%PATH%"
    goto :java_ready
)
where java.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: Java was not found.
    echo Install a JDK or point JAVA_HOME to Android Studio's jbr folder.
    goto :failed
)
:java_ready
if not defined ANDROID_HOME if not defined ANDROID_SDK_ROOT if exist "%LOCALAPPDATA%\Android\Sdk\platform-tools\adb.exe" (
    set "ANDROID_HOME=%LOCALAPPDATA%\Android\Sdk"
    set "ANDROID_SDK_ROOT=%LOCALAPPDATA%\Android\Sdk"
)

if not defined ANDROID_HOME if not defined ANDROID_SDK_ROOT (
    echo WARNING: ANDROID_HOME and ANDROID_SDK_ROOT are not set.
    echo Gradle may still find the SDK through android-app\local.properties.
    echo.
)

echo ========================================
echo       TRG - ANDROID APK BUILD
echo ========================================
echo.
echo Building the debug APK with Gradle...
echo.

call "%GRADLEW%" --no-daemon assembleDebug
if errorlevel 1 goto :failed

if not exist "%APK%" (
    echo ERROR: Gradle finished but the APK was not found:
    echo %APK%
    goto :failed
)

echo.
echo ========================================
echo          BUILD SUCCESSFUL
echo ========================================
echo APK:
echo %APK%
echo.
if not defined CI if not defined TRG_NO_PAUSE (
    explorer.exe /select,"%APK%"
)
goto :success

:failed
echo.
echo ========================================
echo            BUILD FAILED
echo ========================================
echo Read the Gradle error messages above.
goto :finish_failed

:success
popd
if not defined CI if not defined TRG_NO_PAUSE pause
endlocal
exit /b 0

:finish_failed
popd
if not defined CI if not defined TRG_NO_PAUSE pause
endlocal
exit /b 1

:failed_no_push
echo ERROR: Could not enter the TRG project folder.
if not defined CI if not defined TRG_NO_PAUSE pause
endlocal
exit /b 1
