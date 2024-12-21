#ifndef METHODS_H
#define METHODS_H

#include "utilities.h"
#include "zstdlib/zmalloc.h"
#include "system/zsysmem.h"
#include "zstdlib/cfastlookup2.h"
#include "drawing/zcounter.h"
#include "zstdlib/input/zinputactions.h"

#include <minhook.h>

namespace Methods
{
    static LPVOID zSysMem_NewRef = (LPVOID)0x0FFB1E30;
    static LPVOID zMalloc_RemoveFreeHeaderFromBins = (LPVOID)0x0FFC7CD0;
    static LPVOID cFastLookup_RemoveLowerCase = (LPVOID)0x0FFBE9D0;
    static LPVOID cFastLookup_GetLowerCase = (LPVOID)0x0FFBEB80;
    static LPVOID zInputActions_OverrideAction = (LPVOID)0x0FFA14A0;
    static LPVOID zInputActions_Dtor = (LPVOID)0x0FFA1250;
    static LPVOID zSysCom_LogMessage = (LPVOID)0x0FFA47C0;

    typedef void(__fastcall *ZSysMem_NewRef)(ZSysMem *_this, void *p_link);
    typedef void(__fastcall *ZMalloc_RemoveFreeHeaderFromBins)(ZMalloc *_this, void *_EDX, SMallocFreeHeader *p_freeHeader, SBinTreeNode *p_mallocBin);
    typedef void(__fastcall *CFastLookup_RemoveLowerCase)(CFastLookup2 *_this, void *_EDX, const char *p_str, u32 p_size);
    typedef void(__fastcall *CFastLookup_GetLowerCase)(CFastLookup2 *_this, void *_EDX, const char *p_str);
    typedef void(__fastcall *ZInputActions_OverrideAction)(ZInputActions *_this, void *_EDX, SActionOverride *p_actionOverride);
    typedef void(__fastcall *ZSysCom_LogMessage)(ZSysCom *_this, void *_EDX, char *p_format, ...);

    typedef void(__fastcall *ZInputActions_Dtor)(ZInputActions *_this, void *_EDX);

    static ZSysMem_NewRef originalZSysMem_NewRef = 0;
    static ZInputActions_OverrideAction originalZInputActions_OverrideAction = 0;

    void __fastcall ZSysMem_NewRefHook(ZSysMem *_this, void *_EDX, void *p_link);
    void __fastcall ZMalloc_RemoveFreeHeaderFromBinsHook(ZMalloc *_this, void *_EDX, SMallocFreeHeader *p_freeHeader, SBinTreeNode *p_mallocBin);
    void __fastcall CFastLookup_RemoveLowerCaseHook(CFastLookup2 *_this, void *_EDX, const char *p_str, u32 p_size);
    void __fastcall CFastLookup_GetLowerCaseHook(CFastLookup2 *_this, void *_EDX, const char *p_str);
    void __fastcall ZInputActions_OverrideActionHook(ZInputActions *_this, void *_EDX, SActionOverride *p_actionOverride);
    void __fastcall ZInputActions_DtorHook(ZInputActions *_this, void *_EDX);
    void __fastcall ZSysCom_LogMessageHook(ZSysCom *_this, void *_EDX, char *p_format, ...);

    void CreateHooks();
    void CreateRenderingHooks();
}

#endif