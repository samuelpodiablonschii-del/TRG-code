@echo off
setlocal EnableExtensions
pushd "%~dp0" >nul 2>nul
if errorlevel 1 goto :failed_no_push

title TRG Studio - Android APK Builder
set "PROJECT_DIR=%~dp0android-app"
set "GRADLEW=%PROJECT_DIR%\gradlew.bat"
set "SDK_DIR="
set "APK=%PROJECT_DIR%\app\build\outputs\apk\debug\app-debug.apk"

echo ========================================
echo       TRG STUDIO - ANDROID APK
echo ========================================
echo.

if not exist "%GRADLEW%" (
    echo ERROR: The Android Gradle wrapper is missing:
    echo   %GRADLEW%
    echo Make sure you downloaded the complete TRG-code repository.
    goto :failed
)

rem Prefer a valid SDK path already configured by Android Studio.
if defined ANDROID_SDK_ROOT if exist "%ANDROID_SDK_ROOT%\platforms" set "SDK_DIR=%ANDROID_SDK_ROOT%"
if not defined SDK_DIR if defined ANDROID_HOME if exist "%ANDROID_HOME%\platforms" set "SDK_DIR=%ANDROID_HOME%"

rem Try common Android Studio SDK install locations.
if not defined SDK_DIR if defined LOCALAPPDATA if exist "%LOCALAPPDATA%\Android\Sdk\platforms" set "SDK_DIR=%LOCALAPPDATA%\Android\Sdk"
if not defined SDK_DIR if defined USERPROFILE if exist "%USERPROFILE%\AppData\Local\Android\Sdk\platforms" set "SDK_DIR=%USERPROFILE%\AppData\Local\Android\Sdk"
if not defined SDK_DIR if exist "C:\Android\android-sdk\platforms" set "SDK_DIR=C:\Android\android-sdk"
if not defined SDK_DIR if defined ProgramFiles if exist "%ProgramFiles%\Android\Android Studio\sdk\platforms" set "SDK_DIR=%ProgramFiles%\Android\Android Studio\sdk"
if not defined SDK_DIR if defined ProgramFiles(x86) if exist "%ProgramFiles(x86)%\Android\android-sdk\platforms" set "SDK_DIR=%ProgramFiles(x86)%\Android\android-sdk"

if defined SDK_DIR (
    set "ANDROID_HOME=%SDK_DIR%"
    set "ANDROID_SDK_ROOT=%SDK_DIR%"
    echo Android SDK: %SDK_DIR%
) else (
    if not exist "%PROJECT_DIR%\local.properties" (
        echo ERROR: Android SDK was not found.
        echo.
        echo Install Android Studio and its Android SDK, then open:
        echo   Android Studio ^> Tools ^> SDK Manager
        echo Install these components:
        echo   Android SDK Platform 35
        echo   Android SDK Build-Tools 36.0.0
        echo   NDK 28.2.13676358
        echo   CMake 3.22.1
        echo.
        echo Or set ANDROID_HOME / ANDROID_SDK_ROOT to your SDK folder.
        goto :failed
    )
    echo Android SDK: will use android-app\local.properties
)

rem Find a JDK. Android Studio's bundled JBR is preferred when JAVA_HOME is absent.
if defined JAVA_HOME if exist "%JAVA_HOME%\bin\java.exe" goto :java_ready
set "JAVA_HOME="
if defined ProgramFiles if exist "%ProgramFiles%\Android\Android Studio\jbr\bin\java.exe" (
    set "JAVA_HOME=%ProgramFiles%\Android\Android Studio\jbr"
    goto :java_ready
)
if defined LOCALAPPDATA if exist "%LOCALAPPDATA%\Programs\Android Studio\jbr\bin\java.exe" (
    set "JAVA_HOME=%LOCALAPPDATA%\Programs\Android Studio\jbr"
    goto :java_ready
)
if defined ProgramFiles(x86) if exist "%ProgramFiles(x86)%\Android\Android Studio\jbr\bin\java.exe" (
    set "JAVA_HOME=%ProgramFiles(x86)%\Android\Android Studio\jbr"
    goto :java_ready
)
where java.exe >nul 2>nul
if errorlevel 1 (
    echo ERROR: Java was not found.
    echo Install Android Studio or a JDK 17 or newer, then set JAVA_HOME.
    goto :failed
)

:java_ready
if defined JAVA_HOME set "PATH=%JAVA_HOME%\bin;%PATH%"
echo Java:
java.exe -version
if errorlevel 1 (
    echo ERROR: Java could not be started. Check JAVA_HOME.
    goto :failed
)
echo.

rem Check SDK components when the SDK folder was detected.
if defined SDK_DIR (
    if not exist "%SDK_DIR%\platforms\android-35\android.jar" (
        echo ERROR: Android SDK Platform 35 is not installed.
        echo Install it using Android Studio SDK Manager.
        goto :failed
    )
    if not exist "%SDK_DIR%\build-tools\36.0.0\aapt.exe" (
        echo ERROR: Android SDK Build-Tools 36.0.0 is not installed.
        echo Install it using Android Studio SDK Manager.
        goto :failed
    )
    if not exist "%SDK_DIR%\ndk\28.2.13676358\source.properties" (
        echo ERROR: Android NDK 28.2.13676358 is not installed.
        echo Install it using Android Studio SDK Manager.
        goto :failed
    )
    if not exist "%SDK_DIR%\cmake\3.22.1\bin\cmake.exe" (
        echo ERROR: CMake 3.22.1 is not installed in the Android SDK.
        echo Install it using Android Studio SDK Manager.
        goto :failed
    )
)

echo Building TRG Studio debug APK...
echo First builds may need internet access to download Gradle and dependencies.
echo.
pushd "%PROJECT_DIR%" >nul
if errorlevel 1 (
    echo ERROR: Could not enter the Android project folder.
    goto :failed
)
call gradlew.bat --no-daemon --stacktrace assembleDebug
set "BUILD_RESULT=%ERRORLEVEL%"
popd

if not "%BUILD_RESULT%"=="0" (
    echo.
    echo ERROR: Gradle could not build the Android app.
    goto :failed
)

if not exist "%APK%" (
    echo.
    echo ERROR: Gradle reported success but the APK was not found:
    echo   %APK%
    goto :failed
)

for %%F in ("%APK%") do if %%~zF LEQ 0 (
    echo ERROR: The generated APK is empty.
    goto :failed
)

echo.
echo ========================================
echo           BUILD SUCCESSFUL
echo ========================================
echo APK:
echo   %APK%
echo.
echo Copy this APK to your Android device and install it.
if not defined CI if not defined TRG_NO_PAUSE (
    if exist "%WINDIR%\explorer.exe" start "" explorer.exe /select,"%APK%"
)
goto :success

:failed
echo.
echo ========================================
echo             BUILD FAILED
echo ========================================
echo Fix the error shown above and run build-apk.bat again.
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
