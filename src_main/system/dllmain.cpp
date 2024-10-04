#include <windows.h>
#include "zstdlib/reftab/reftab.h"

// TODO: Implement this
extern "C" __declspec(dllexport) int RunEngine(int p_unkInt)
{
    return 0;
}

// TODO: Implement this
extern "C" __declspec(dllexport) void EditPrint(int p_unkInt)
{
    return;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}