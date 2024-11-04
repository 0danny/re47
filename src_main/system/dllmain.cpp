#include "common.h"

#include "zsysfile.h"
#include "zsysinterface.h"
#include "zsysmem.h"

static i32 g_initCount = 0;

// TODO: Implement this fully
extern "C" __declspec(dllexport) int RunEngine(int p_unkInt)
{
    g_pSysInterface->Restart(p_unkInt);
    return 0;
}

extern "C" __declspec(dllexport) void EditPrint(char *p_str)
{
    g_pSysCom->LogMessage("%s", p_str);
}

BOOL APIENTRY DllMain(HMODULE p_hinstDLL, u32 p_fdwReason, LPVOID p_lpReserved)
{
    i32 l_count = 0;

    if (p_fdwReason == 1)
    {
        if (!g_initCount)
        {
            ZSysCom *l_sysCom = new ZSysCom();
            ZSysMem *l_sysMem = new ZSysMem();
            ZSysFile *l_sysFile = new ZSysFile();
            ZSysInterface *l_sysInterface = new ZSysInterface(p_hinstDLL);

            g_pSysCom->ProcessDebugWnd(g_pSysInterface->m_mainHwnd);

            g_pSysInterface->InitializeResources();
        }

        l_count = g_initCount + 1;
    RETURN_LABEL:

        g_initCount = l_count;
        return 1;
    }

    if (!p_fdwReason)
    {
        if (g_initCount == 1)
        {
            if (g_pSysInterface)
                g_pSysInterface->~ZSysInterface();

            if (g_pSysFile)
                g_pSysFile->~ZSysFile();

            g_pSysCom->DestroyArrays();

            if (g_pSysMem)
                g_pSysMem->~ZSysMem();

            if (g_pSysCom)
                g_pSysCom->~ZSysCom();
        }

        l_count = g_initCount - 1;
        goto RETURN_LABEL;
    }

    return 1;
}
