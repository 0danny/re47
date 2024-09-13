#include <windows.h>

#include "System/zsysinterface.h"
#include "System/zsyscom.h"

__declspec(dllexport) ZSysInterface *g_pSysInterface;
__declspec(dllexport) ZSysCom *g_pSysCom;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}