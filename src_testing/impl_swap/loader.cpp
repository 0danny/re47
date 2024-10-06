#include "loader.h"

namespace Loader
{
    void Init()
    {
        if (MH_CreateHookApi(L"kernel32", "LoadLibraryA", &LoadLibraryAHook, reinterpret_cast<LPVOID *>(&originalLoadLibraryA)) != MH_OK)
        {
            printf("[LOADER] -> Failed to hook LoadLibraryA!\n");
        }

        if (MH_CreateHookApi(L"user32", "RegisterWindowMessageA", &RegisterWindowMessageAHook, reinterpret_cast<LPVOID *>(&originalRegisterWindowMessageA)) != MH_OK)
        {
            printf("[LOADER] -> Failed to hook RegisterWindowMessageA!\n");
        }

        EnableHooks();
    }

    void InjectHooks()
    {
        printf("[LOADER]: Injecting hooks...\n");

        Constructors::CreateHooks();
        WndPatches::CreateHooks();

        EnableHooks();
    }

    HMODULE __stdcall LoadLibraryAHook(LPCSTR lpLibFileName)
    {
        printf("[LOADER_HOOK]: Library Loaded -> %s\n", Utilities::GetFileName(lpLibFileName));

        return originalLoadLibraryA(lpLibFileName);
    }

    UINT WINAPI RegisterWindowMessageAHook(LPCSTR lpString)
    {
        // This function gets called in the ZSysCom constructor, it is the earliest point we can hook into the game
        // We can use this to inject our hooks

        if (strcmp(lpString, "ZSystemMessage") == 0)
        {
            printf("[LOADER_HOOK]: RegisterWindowMessageA called with string: %s\n", lpString);

            InjectHooks();
        }

        return originalRegisterWindowMessageA(lpString);
    }

    void EnableHooks()
    {
        if (MH_EnableHook(MH_ALL_HOOKS))
        {
            printf("[LOADER]: Failed to enable all hooks.\n");
        }
    }
}
