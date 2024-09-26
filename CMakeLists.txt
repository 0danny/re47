cmake_minimum_required(VERSION 3.5.0)
project(HitmanC47 CXX RC)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src_main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src_testing)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external_include)

if (MSVC_VERSION GREATER_EQUAL 1900)
    message(STATUS "Using modern MSVC")
   
    # Build the testing dll(s)
    add_library(impl_swap SHARED 
    src_testing/impl_swap/dllmain.cpp
    src_main/system/reftab/reftab.cpp
    src_main/system/reftab/strreftab.cpp
    )

    set_target_properties(impl_swap PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

    target_link_directories(impl_swap PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external_libs)
    target_link_libraries(impl_swap PRIVATE MinHook.x86 globals system)

else ()
    message(STATUS "Building using VS6")

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

    # Add System DLL
    add_library(system SHARED
    src_main/system/system.h

    src_main/system/dllmain.cpp 

    src_main/system/zsysinterface.cpp
    src_main/system/zsysinterface.h 

    src_main/system/zsysmem.cpp 
    src_main/system/zsysmem.h 

    src_main/system/zsysfile.cpp 
    src_main/system/zsysfile.h 

    src_main/system/zsyscom.cpp 
    src_main/system/zsyscom.h

    src_main/system/reftab/reftab.cpp
    src_main/system/reftab/reftab.h

    src_main/system/reftab/strreftab.cpp
    src_main/system/reftab/strreftab.h
    )

    target_link_libraries(system globals)

    # Add Globals DLL
    add_library(globals SHARED src_main/globals/dllmain.cpp)

    # Add the hitman executable
    add_executable(hitman WIN32 src_main/hitman/main.cpp ${RESOURCE_FILE})
    target_link_libraries(hitman globals)
endif ()
