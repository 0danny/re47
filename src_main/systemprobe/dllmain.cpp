#include "common.h"
#include "csystemprobe.h"

extern "C" __declspec(dllexport) CSystemProbe *Create()
{
    CSystemProbe *systemProbe = new CSystemProbe();

    g_probeInstance = systemProbe;

    return systemProbe;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        DisableThreadLibraryCalls(hModule);

    return TRUE;
}
