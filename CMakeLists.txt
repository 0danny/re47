cmake_minimum_required(VERSION 3.5.0)

cmake_policy(SET CMP0091 NEW)

project(HitmanC47 C CXX RC)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src_main)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src_testing)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/include)

# Linker, compiler and resource flags
# Copied straight from the Isle Decompilation project
set(CMAKE_CXX_FLAGS "/W3 /GX /D \"WIN32\" /D \"_WINDOWS\"")
set(CMAKE_CXX_FLAGS_DEBUG "/Gm /Zi /Od /D \"_DEBUG\"")
set(CMAKE_CXX_FLAGS_RELEASE "/O2 /D \"NDEBUG\"")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/Zi /O2 /D \"NDEBUG\"")
set(CMAKE_CXX_FLAGS_MINSIZEREL "/Os /D \"NDEBUG\"")

set(CMAKE_EXE_LINKER_FLAGS "/machine:I386 /ignore:4098,4099,4237,4200") # Supress warning about missing .pdb files
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "/incremental:yes /debug")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/incremental:no")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "/incremental:no /debug")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "/incremental:no")

set(CMAKE_SHARED_LINKER_FLAGS "/machine:I386 /ignore:4098,4099,4237,4200")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "/incremental:yes /debug")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/incremental:no")
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "/incremental:no /debug")
set(CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL "/incremental:no")

set(CMAKE_RC_FLAGS "/I\"${CMAKE_CURRENT_SOURCE_DIR}/resources\"")
set(RESOURCE_FILE "resources/hitman.rc")

# Build ZLIB
add_library(zlib STATIC
    external/include/zlib/adler32.c
    external/include/zlib/compress.c
    external/include/zlib/crc32.c
    external/include/zlib/deflate.c
    external/include/zlib/gzio.c
    external/include/zlib/infblock.c
    external/include/zlib/infcodes.c
    external/include/zlib/inffast.c
    external/include/zlib/inflate.c
    external/include/zlib/inftrees.c
    external/include/zlib/infutil.c
    external/include/zlib/trees.c
    external/include/zlib/uncompr.c
    external/include/zlib/zutil.c
)

set_target_properties(zlib PROPERTIES
    LINKER_LANGUAGE C
    COMPILE_FLAGS "/MT"
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# Add drawing library (drawing)
add_library(drawing STATIC
    src_main/drawing/zcounter.cpp
    src_main/drawing/zcounter.h
)

# Add common library (zstdlib)
add_library(common STATIC
    src_main/common.h
    src_main/memory.cpp

    # RefTabs
    src_main/zstdlib/reftab/reftab.cpp
    src_main/zstdlib/reftab/reftab.h

    src_main/zstdlib/reftab/allocreftab.cpp
    src_main/zstdlib/reftab/allocreftab.h

    src_main/zstdlib/reftab/reftab32.cpp
    src_main/zstdlib/reftab/reftab32.h

    src_main/zstdlib/reftab/strreftab.cpp
    src_main/zstdlib/reftab/strreftab.h

    src_main/zstdlib/reftab/equreftab.cpp
    src_main/zstdlib/reftab/equreftab.h

    src_main/zstdlib/reftab/linkreftab.cpp
    src_main/zstdlib/reftab/linkreftab.h

    src_main/zstdlib/reftab/linksortreftab.cpp
    src_main/zstdlib/reftab/linksortreftab.h

    src_main/zstdlib/reftab/staticreftab.cpp
    src_main/zstdlib/reftab/staticreftab.h

    # Main
    src_main/zstdlib/zdynamicloader.cpp
    src_main/zstdlib/zdynamicloader.h

    src_main/zstdlib/zmalloc.cpp
    src_main/zstdlib/zmalloc.h

    # Console
    src_main/zstdlib/zconsole/zconsolecommand.cpp
    src_main/zstdlib/zconsole/zconsolecommand.h

    src_main/zstdlib/zconsole/zconsole.cpp
    src_main/zstdlib/zconsole/zconsole.h

    # Trees
    src_main/zstdlib/trees/zrbtree.cpp
    src_main/zstdlib/trees/zrbtree.h

    src_main/zstdlib/trees/zvaltree.cpp
    src_main/zstdlib/trees/zvaltree.h

    # CFastLookup2
    src_main/zstdlib/cfastlookup2.cpp
    src_main/zstdlib/cfastlookup2.h

    # CFastLookupFileCache
    src_main/zstdlib/cfastlookupfilecache.cpp
    src_main/zstdlib/cfastlookupfilecache.h

    # Input
    src_main/zstdlib/input/zinputaction.cpp
    src_main/zstdlib/input/zinputaction.h

    src_main/zstdlib/input/zinputactions.cpp
    src_main/zstdlib/input/zinputactions.h

    src_main/zstdlib/input/zactionmap.cpp
    src_main/zstdlib/input/zactionmap.h

    src_main/zstdlib/input/zactionmapdefinition.h
    src_main/zstdlib/input/sinputnode.h

    # Master Control
    src_main/zstdlib/zmastercontrol.cpp
    src_main/zstdlib/zmastercontrol.h

    # FSZip
    src_main/zstdlib/fszip.cpp
    src_main/zstdlib/fszip.h

    # Utilities
    src_main/zstdlib/utilities/dynamicarray.h
    src_main/zstdlib/utilities/mystr.h
)

# Add System DLL
add_library(system SHARED
    src_main/system/dllmain.cpp

    src_main/system/zsyscom.cpp
    src_main/system/zsyscom.h

    src_main/system/zsysinterface.cpp
    src_main/system/zsysinterface.h

    src_main/system/zsysmem.cpp
    src_main/system/zsysmem.h

    src_main/system/zsysfile.cpp
    src_main/system/zsysfile.h
)

target_sources(system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dll_defs/system.def)
target_link_libraries(system globals common)

# Add directplay dll
add_library(directplay SHARED
    src_main/directplay/dllmain.cpp

    src_main/directplay/directplaydll.cpp
    src_main/directplay/directplaydll.h

    src_main/directplay/directplay.cpp
    src_main/directplay/directplay.h
)

target_link_directories(directplay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/libs)
target_link_libraries(directplay globals common dplayx dxguid)
target_sources(directplay PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dll_defs/directplay.def)

# Add globals dll
add_library(globals SHARED src_main/globals/dllmain.cpp)
target_sources(globals PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dll_defs/globals.def)

# Add the hitman executable
add_executable(hitman WIN32 src_main/hitman/main.cpp ${RESOURCE_FILE})
target_link_libraries(hitman globals common zlib)

# Add the probe dll
add_library(systemprobe SHARED
    src_main/systemprobe/dllmain.cpp

    src_main/systemprobe/csystemprobe.cpp
    src_main/systemprobe/csystemprobe.h
)

target_link_directories(systemprobe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/libs)
target_link_libraries(systemprobe common globals ddraw dxguid opengl32 glu32 glide3x)
target_sources(systemprobe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dll_defs/systemprobe.def)
set_target_properties(systemprobe PROPERTIES OUTPUT_NAME "systemprobedll")

# Build the testing dll(s)
add_library(impl_swap SHARED
    src_testing/impl_swap/dllmain.cpp

    src_testing/impl_swap/constructors.cpp
    src_testing/impl_swap/constructors.h

    src_testing/impl_swap/menu/menu.cpp
    src_testing/impl_swap/menu/menu.h

    src_testing/impl_swap/methods.cpp
    src_testing/impl_swap/methods.h

    src_testing/impl_swap/wnd_patches.cpp
    src_testing/impl_swap/wnd_patches.h

    src_testing/impl_swap/loader.cpp
    src_testing/impl_swap/loader.h

    src_testing/impl_swap/utilities.h

    # Mods
    src_testing/impl_swap/menu/mods/freecam.cpp
    src_testing/impl_swap/menu/mods/freecam.h

    # Statically link in ZSysCom
    src_main/system/zsyscom.cpp
    src_main/system/zsyscom.h
)

target_link_directories(impl_swap PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external/libs)
target_link_libraries(impl_swap PRIVATE system minhook_x86 globals common drawing opengl32 zlib)
