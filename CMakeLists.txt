cmake_minimum_required(VERSION 3.5.0)
project(HitmanC47 CXX RC)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Linker, compiler and resource flags
# Copied straight from the Isle Decompilation project
set(CMAKE_CXX_FLAGS "/W3 /GX /D \"WIN32\" /D \"_WINDOWS\"")
set(CMAKE_CXX_FLAGS_DEBUG "/Gm /Zi /Od /D \"_DEBUG\"")
set(CMAKE_CXX_FLAGS_RELEASE "/O2 /D \"NDEBUG\"")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Zi /O2 /D \"NDEBUG\"")
set(CMAKE_CXX_FLAGS_MINSIZEREL "/Os /D \"NDEBUG\"")

set(CMAKE_EXE_LINKER_FLAGS "/machine:I386")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/incremental:yes /debug")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/incremental:no")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/incremental:no /debug")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "/incremental:no")

set(CMAKE_STATIC_LINKER_FLAGS "/machine:I386")

set(CMAKE_SHARED_LINKER_FLAGS "/machine:I386")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/incremental:yes /debug")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/incremental:no")
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "/incremental:no /debug")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "/incremental:no")

# Sort out the resource file
set(CMAKE_RC_FLAGS "/I\"${CMAKE_CURRENT_SOURCE_DIR}/resources\"")
set(RESOURCE_FILE "resources/hitman.rc")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add System DLL
add_library(system SHARED src/system/dllmain.cpp 
src/system/zsysinterface.cpp
src/system/zsysinterface.h 
src/system/zsysmem.cpp 
src/system/zsysmem.h 
src/system/zsysfile.cpp 
src/system/zsysfile.h 
src/system/zsyscom.cpp 
src/system/zsyscom.h)
target_link_libraries(system globals)

# Add Globals DLL
add_library(globals SHARED src/globals/dllmain.cpp)

# Add the hitman executable
add_executable(hitman WIN32 src/hitman/main.cpp ${RESOURCE_FILE})
target_link_libraries(hitman globals)