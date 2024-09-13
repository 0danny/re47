#include <windows.h>
#include "System/zsysinterface.h"

__declspec(dllexport) ZSysInterface *g_pSysInterface;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}