#include "directplayproxy.h"

extern "C"
{
    __declspec(dllexport) DirectPlayProxy *Create();
}

DirectPlayProxy *Create()
{
    return new DirectPlayProxy();
}

// TODO: This DLL doesn't link properly to DPLAYX.dll.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}
