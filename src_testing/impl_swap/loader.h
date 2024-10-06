#pragma once

#include "constructors.h"
#include "wnd_patches.h"
#include "utilities.h"

#include <minhook.h>

namespace Loader
{
    typedef HMODULE(__stdcall *LoadLibraryA_t)(LPCSTR);
    typedef UINT(WINAPI *RegisterWindowMessageA_t)(LPCSTR lpString);

    HMODULE __stdcall LoadLibraryAHook(LPCSTR lpLibFileName);
    UINT WINAPI RegisterWindowMessageAHook(LPCSTR lpString);

    static LoadLibraryA_t originalLoadLibraryA = 0;
    static RegisterWindowMessageA_t originalRegisterWindowMessageA = 0;

    void Init();
    void InjectHooks();
    void EnableHooks();
}
