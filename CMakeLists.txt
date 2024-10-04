cmake_minimum_required(VERSION 3.5.0)
project(HitmanC47 CXX RC)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src_main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src_testing)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external_include)

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

set(CMAKE_SHARED_LINKER_FLAGS "/machine:I386")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/incremental:yes /debug")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/incremental:no")
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "/incremental:no /debug")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "/incremental:no")

set(CMAKE_RC_FLAGS "/I\"${CMAKE_CURRENT_SOURCE_DIR}/resources\"")
set(RESOURCE_FILE "resources/hitman.rc")

#  Add common library (zstdlib)
add_library(common STATIC 
src_main/common.h

src_main/zstdlib/reftab/reftab.cpp
src_main/zstdlib/reftab/reftab.h

src_main/zstdlib/reftab/strreftab.cpp
src_main/zstdlib/reftab/strreftab.h

src_main/zstdlib/reftab/equreftab.cpp
src_main/zstdlib/reftab/equreftab.h

src_main/zstdlib/zdynamicloader.cpp
src_main/zstdlib/zdynamicloader.h

src_main/zstdlib/zconsole/zconsolecommand.cpp
src_main/zstdlib/zconsole/zconsolecommand.h
)

# Add System DLL
add_library(system SHARED
    src_main/system/dllmain.cpp 

    src_main/system/zsysinterface.cpp
    src_main/system/zsysinterface.h 

    src_main/system/zsysmem.cpp 
    src_main/system/zsysmem.h 

    src_main/system/zsysfile.cpp 
    src_main/system/zsysfile.h 

    src_main/system/zsyscom.cpp 
    src_main/system/zsyscom.h
)

target_sources(system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dll_defs/system.def)
target_link_libraries(system globals common)

# Add DirectPlay DLL
add_library(directplay SHARED
    src_main/directplay/dllmain.cpp

    src_main/directplay/directplayproxy.cpp
    src_main/directplay/directplayproxy.h

    src_main/directplay/directplay.cpp
    src_main/directplay/directplay.h
)

target_sources(directplay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dll_defs/directplay.def)
target_link_directories(directplay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external_libs)
target_link_libraries(directplay globals common)

# Add Globals DLL
add_library(globals SHARED src_main/globals/dllmain.cpp)

target_sources(globals PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dll_defs/globals.def)

# Add the hitman executable
add_executable(hitman WIN32 src_main/hitman/main.cpp ${RESOURCE_FILE})
target_link_libraries(hitman globals common)

# Build the testing dll(s)
add_library(impl_swap SHARED 
src_testing/impl_swap/dllmain.cpp

src_testing/impl_swap/constructors.cpp
src_testing/impl_swap/constructors.h

src_testing/impl_swap/wnd_patches.cpp
src_testing/impl_swap/wnd_patches.h

src_testing/impl_swap/loader.cpp
src_testing/impl_swap/loader.h

src_testing/impl_swap/utilities.h
)
    
target_link_directories(impl_swap PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external_libs)
target_link_libraries(impl_swap PRIVATE MinHook.x86 globals system kernel32 common)