@echo off

if "%VS6_BIN_PATH%"=="" (
    echo VS6_BIN_PATH is not set
    exit /b 1
)

cd /d %VS6_BIN_PATH%

call VCVARS32.BAT

if "%errorlevel%" neq "0" (
    echo Failed to set up Visual Studio 6 environment
    exit /b %errorlevel%
)

cd /d %~dp0

if not exist "build" (
    mkdir build
)

cd build

cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RELWITHDEBINFO

nmake && nmake

cd /d %~dp0
