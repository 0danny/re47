#include <windows.h>
#include <iostream>
#include <fstream>

#include <MinHook.h>

#include "system/reftab/reftab.h"

typedef RefTab *(__fastcall *RefTabConstructor)(RefTab *_this, int p_poolSize, int p_size);
RefTabConstructor oRefTabConstructor = 0;

void OpenConsole()
{
    AllocConsole();

    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);

    printf("Console allocated successfully.\n");
}

RefTab *__fastcall HookedRefTabConstructor(RefTab *_this, void *_EDX, int p_poolSize, int p_size)
{
    printf("Hooked RefTab constructor called!\n");
    printf("p_poolSize: %d, p_size: %d\n", p_poolSize, p_size);

    return new RefTab(p_poolSize, p_size);
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

        if (MH_CreateHook((LPVOID)0x0FFBED50, (LPVOID)&HookedRefTabConstructor, reinterpret_cast<LPVOID *>(&oRefTabConstructor)) != MH_OK)
        {
            printf("Failed to create hook.\n");
            return FALSE;
        }

        if (MH_EnableHook((LPVOID)0x0FFBED50) != MH_OK)
        {
            printf("Failed to enable hook.\n");
            return FALSE;
        }
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        MH_DisableHook((LPVOID)0x0FFBED50);
        MH_Uninitialize();

        printf("Hook uninstalled successfully.\n");
    }

    return TRUE;
}
