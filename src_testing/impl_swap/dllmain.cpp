#include <windows.h>
#include <minhook.h>
#include <iostream>
#include <fstream>

#include "loader.h"
#include "utilities.h"

void OpenConsole();

void OpenConsole()
{
    AllocConsole();

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);

    printf("[DLL_MAIN]: Impl_Swap Loaded.\n");

    SetConsoleTitleA(Utilities::TITLE);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        OpenConsole();

        if (MH_Initialize() != MH_OK)
        {
            printf("[DLL_MAIN]: Failed to initialize MinHook.\n");
            return FALSE;
        }

        i64 result = GetRDTSC();

        printf("Testing RDTSC: %lld\n", result);

        Loader::Init();
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();

        printf("[DLL_MAIN]: Hooks uninstalled successfully.\n");
    }

    return TRUE;
}
