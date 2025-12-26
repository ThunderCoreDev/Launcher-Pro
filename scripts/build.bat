@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ==========================================
echo    CONSTRUYENDO THUNDERNET LAUNCHER PRO   
echo ==========================================

:: Configuración
set BUILD_TYPE=Release
set BUILD_DIR=build
set INSTALL_DIR=C:\Program Files\ThunderNet Launcher Pro
set GENERATOR="Visual Studio 17 2022"

:: Verificar CMake
where cmake >nul 2>nul
if errorlevel 1 (
    echo Error: CMake no encontrado
    echo Instala CMake desde: https://cmake.org/download/
    pause
    exit /b 1
)

:: Verificar Qt
set QT_PATH=
for /f "tokens=*" %%i in ('where qmake') do set QT_PATH=%%i
if "%QT_PATH%"=="" (
    echo Error: Qt no encontrado
    echo Instala Qt desde: https://www.qt.io/download
    pause
    exit /b 1
)

:: Limpiar construcción anterior
if exist "%BUILD_DIR%" (
    echo Limpiando construcción anterior...
    rmdir /s /q "%BUILD_DIR%"
)

mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"

:: Configurar
echo Configurando CMake...
cmake .. -G %GENERATOR% -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if errorlevel 1 (
    echo Error en configuración de CMake
    pause
    exit /b 1
)

:: Compilar
echo Compilando...
cmake --build . --config %BUILD_TYPE% -- /m
if errorlevel 1 (
    echo Error en la compilación
    pause
    exit /b 1
)

cd ..

echo.
echo ¡Construcción completada exitosamente!
echo.
echo El ejecutable está en: %BUILD_DIR%\%BUILD_TYPE%\ThunderNet-Launcher-Pro.exe
echo.
pause