#include <windows.h>

#include "reftab/reftab.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    RefTab *refTab = new RefTab(32, 4);

    return TRUE;
}