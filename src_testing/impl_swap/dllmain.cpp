#include <windows.h>
#include <iostream>
#include <fstream>
#include <MinHook.h>

#include "constructors.h"
#include "wnd_patches.h"

void OpenConsole();
void CreateHooks();

void OpenConsole()
{
    AllocConsole();

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);

    printf("Console allocated successfully.\n");
}

void CreateHooks()
{
    WndPatches::CreateHooks();
    Constructors::CreateHooks();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        OpenConsole();

        if (MH_Initialize() != MH_OK)
        {
            printf("Failed to initialize MinHook.\n");
            return FALSE;
        }

        CreateHooks();

        if (MH_EnableHook(MH_ALL_HOOKS))
        {
            printf("Failed to enable all hooks.\n");
            return FALSE;
        }
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();

        printf("Hook uninstalled successfully.\n");
    }

    return TRUE;
}
