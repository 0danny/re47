#include "directplaydll.h"

extern "C"
{
    __declspec(dllexport) DirectPlayDll *Create();
}

DirectPlayDll *Create()
{
    return new DirectPlayDll();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}
