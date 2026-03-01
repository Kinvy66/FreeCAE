@echo off
setlocal

:: FreeCAE installer build script (ASCII only to avoid encoding issues)
:: Requires: NSIS installed (makensis in PATH)

set "SCRIPT_DIR=%~dp0"
set "INSTALLER_DIR=%SCRIPT_DIR%"
set "NSI_FILE=%SCRIPT_DIR%FreeCAE_installer.nsi"
set "OUTPUT_DIR=%SCRIPT_DIR%..\Output"

if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

if not exist "%NSI_FILE%" (
    echo [ERROR] NSI file not found: %NSI_FILE%
    exit /b 1
)

set "MAKENSIS="
where makensis >nul 2>&1
if %errorlevel% equ 0 (
    for /f "tokens=*" %%a in ('where makensis 2^>nul') do (
        set "MAKENSIS=%%a"
        goto :found_makensis
    )
)
if exist "C:\Program Files (x86)\NSIS\makensis.exe" (
    set "MAKENSIS=C:\Program Files (x86)\NSIS\makensis.exe"
    goto :found_makensis
)
if exist "C:\Program Files\NSIS\makensis.exe" (
    set "MAKENSIS=C:\Program Files\NSIS\makensis.exe"
    goto :found_makensis
)
echo [ERROR] makensis not found. Please install NSIS or add it to PATH.
echo Download: https://nsis.sourceforge.io/Download
exit /b 1

:found_makensis
echo Using NSIS: %MAKENSIS%
echo.

echo Building FreeCAE installer...
echo Working dir: %INSTALLER_DIR%
echo.

cd /d "%INSTALLER_DIR%"
"%MAKENSIS%" "%NSI_FILE%"

if %errorlevel% neq 0 (
    echo.
    echo [FAILED] Build did not succeed.
    exit /b 1
)

echo.
echo [OK] Installer generated at: %OUTPUT_DIR%\
exit /b 0
