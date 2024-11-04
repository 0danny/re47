#include "loader.h"

namespace Loader
{
    void Init()
    {
        printf("[LOADER]: Initializing loader...\n");

        if (MH_CreateHookApi(L"kernel32", "GetCommandLineA", &GetCommandLineAHook, reinterpret_cast<LPVOID *>(&originalGetCommandLineA)) != MH_OK)
        {
            printf("[LOADER] -> Failed to hook GetCommandLineA!\n");
        }

        if (MH_CreateHookApi(L"kernel32", "LoadLibraryA", &LoadLibraryAHook, reinterpret_cast<LPVOID *>(&originalLoadLibraryA)) != MH_OK)
        {
            printf("[LOADER] -> Failed to hook LoadLibraryA!\n");
        }

        /*
        if (MH_CreateHookApi(L"user32", "RegisterWindowMessageA", &RegisterWindowMessageAHook, reinterpret_cast<LPVOID *>(&originalRegisterWindowMessageA)) != MH_OK)
        {
            printf("[LOADER] -> Failed to hook RegisterWindowMessageA!\n");
        }*/

        EnableHooks();
    }

    void InjectHooks()
    {
        printf("[LOADER]: Injecting hooks...\n");

        if (g_enableSwaps)
        {
            Constructors::CreateHooks();
            // Methods::CreateHooks();
            Menu::CreateHooks();
        }

        WndPatches::CreateHooks();

        EnableHooks();
    }

    HMODULE __stdcall LoadLibraryAHook(LPCSTR lpLibFileName)
    {
        printf("[LOADER_HOOK]: Library Loaded -> %s\n", Utilities::GetFileName(lpLibFileName));

        HMODULE module = originalLoadLibraryA(lpLibFileName);

        return module;
    }

    UINT WINAPI RegisterWindowMessageAHook(LPCSTR lpString)
    {
        if (strcmp(lpString, "ZSystemMessage") == 0)
        {
            printf("[LOADER_HOOK]: RegisterWindowMessageA called with string: %s\n", lpString);
        }

        return originalRegisterWindowMessageA(lpString);
    }

    LPSTR __stdcall GetCommandLineAHook()
    {
        if (g_cmdLineCount < 2)
            g_cmdLineCount++;
        else
            g_cmdLineCount = 300;

        // Second time GetCommandLineA is called, is when CRTInit is called on system.dll, this is better than
        // window message hook because it runs before DLLMain, meaning we can hook the constructors before they are called.
        if (g_cmdLineCount == 2)
        {
            // Time to inject
            InjectHooks();
        }

        return originalGetCommandLineA();
    }

    void EnableHooks()
    {
        if (MH_EnableHook(MH_ALL_HOOKS))
        {
            printf("[LOADER]: Failed to enable all hooks.\n");
        }
    }
}
