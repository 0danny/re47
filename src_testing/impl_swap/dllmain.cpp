#include <windows.h>
#include <iostream>
#include <fstream>

#include <MinHook.h>

#include "system/reftab/reftab.h"

typedef RefTab *(__thiscall *RefTabConstructor)(RefTab *_this, int p_poolSize, int p_size);
RefTabConstructor oRefTabConstructor = nullptr;

void OpenConsole()
{
    AllocConsole();

    FILE *f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);

    std::cout << "Console allocated successfully." << std::endl;
}

RefTab *__fastcall HookedRefTabConstructor(RefTab *_this, void *_EDX, int p_poolSize, int p_size)
{
    std::cout << "Hooked RefTab constructor called!" << std::endl;
    std::cout << "p_poolSize: " << p_poolSize << ", p_size: " << p_size << std::endl;

    return oRefTabConstructor(_this, p_poolSize, p_size);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        OpenConsole();

        if (MH_Initialize() != MH_OK)
        {
            std::cout << "Failed to initialize MinHook." << std::endl;
            return FALSE;
        }

        if (MH_CreateHook((LPVOID)0x0FFBED50, (LPVOID)&HookedRefTabConstructor, reinterpret_cast<LPVOID *>(&oRefTabConstructor)) != MH_OK)
        {
            std::cout << "Failed to create hook." << std::endl;
            return FALSE;
        }

        if (MH_EnableHook((LPVOID)0x0FFBED50) != MH_OK)
        {
            std::cout << "Failed to enable hook." << std::endl;
            return FALSE;
        }

        std::cout << "Hook installed successfully!" << std::endl;
    }
    else if (ul_reason_for_call == DLL_PROCESS_DETACH)
    {
        MH_DisableHook((LPVOID)0x0FFBED50);
        MH_Uninitialize();

        std::cout << "Hook uninstalled successfully!" << std::endl;
    }

    return TRUE;
}
