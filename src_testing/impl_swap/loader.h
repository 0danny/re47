#ifndef LOADER_H
#define LOADER_H

#include "constructors.h"
#include "wnd_patches.h"
#include "methods.h"
#include "menu/menu.h"
#include "utilities.h"

#include <minhook.h>

namespace Loader
{
    typedef HMODULE(__stdcall *LoadLibraryA_t)(LPCSTR);
    typedef LPSTR(WINAPI *GetCommandLineA_t)();

    HMODULE __stdcall LoadLibraryAHook(LPCSTR lpLibFileName);
    LPSTR WINAPI GetCommandLineAHook();

    static LoadLibraryA_t originalLoadLibraryA = 0;
    static GetCommandLineA_t originalGetCommandLineA = 0;

    static BOOL g_enableSwaps = TRUE;
    static int g_cmdLineCount = 0;

    void Init();
    void InjectHooks();
    void EnableHooks();
}

#endif