@echo off

echo [Building test dlls...]

if not defined DevEnvDir (
    call "%MODERN_MSVC_BIN%\vcvarsall.bat" x86
)

if not exist "build-test" (
    mkdir build-test
)

cd build-test

cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug

nmake && nmake

cd /d %~dp0