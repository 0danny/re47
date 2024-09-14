@echo off
set VS6_BIN_PATH=C:\VS6\bin

cd /d %VS6_BIN_PATH%

call VCVARS32.BAT x86

if "%errorlevel%" neq "0" (
    echo Failed to set up Visual Studio 6 environment
    exit /b %errorlevel%
)

cd /d %~dp0

if not exist "build" (
    mkdir build
)

cd build

cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug

nmake && nmake

cd /d %~dp0