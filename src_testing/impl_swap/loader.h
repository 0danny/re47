#pragma once

#include "constructors.h"
#include "wnd_patches.h"
#include "methods.h"
#include "menu/menu.h"
#include "utilities.h"

#include <minhook.h>

namespace Loader
{
    typedef HMODULE(__stdcall *LoadLibraryA_t)(LPCSTR);
    typedef UINT(WINAPI *RegisterWindowMessageA_t)(LPCSTR lpString);
    typedef LPSTR(WINAPI *GetCommandLineA_t)();

    HMODULE __stdcall LoadLibraryAHook(LPCSTR lpLibFileName);
    UINT WINAPI RegisterWindowMessageAHook(LPCSTR lpString);
    LPSTR WINAPI GetCommandLineAHook();

    static LoadLibraryA_t originalLoadLibraryA = 0;
    static RegisterWindowMessageA_t originalRegisterWindowMessageA = 0;
    static GetCommandLineA_t originalGetCommandLineA = 0;

    static BOOL g_enableSwaps = TRUE;
    static int g_cmdLineCount = 0;

    void Init();
    void InjectHooks();
    void EnableHooks();
}
