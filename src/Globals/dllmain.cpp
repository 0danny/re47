#include <windows.h>

#include "System/zsysinterface.h"
#include "System/zsyscom.h"
#include "System/zsysfile.h"
#include "System/zsysmem.h"

// Not sure what dll this class is implemented in.
class ZSysInput;

__declspec(dllexport) long GlobalEngineVersion = 192;
__declspec(dllexport) ZSysCom *g_pSysCom;
__declspec(dllexport) ZSysFile *g_pSysFile;
__declspec(dllexport) ZSysInput *g_pSysInput;
__declspec(dllexport) ZSysInterface *g_pSysInterface;
__declspec(dllexport) ZSysMem *g_pSysMem;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}