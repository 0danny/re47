#pragma once

#include "utilities.h"
#include "zstdlib/zmalloc.h"
#include "system/zsysmem.h"

#include <minhook.h>

namespace Methods
{
    static LPVOID zSysMem_NewRef = (LPVOID)0x0FFB1E30;
    static LPVOID zMalloc_RemoveFreeHeaderFromBins = (LPVOID)0x0FFC7CD0;

    typedef void(__fastcall *ZSysMem_NewRef)(ZSysMem *_this, void *p_link);
    typedef void(__fastcall *ZMalloc_RemoveFreeHeaderFromBins)(ZMalloc *_this, void *_EDX, SMallocFreeHeader *p_freeHeader, SBinTreeNode *p_mallocBin);

    static ZSysMem_NewRef originalZSysMem_NewRef = 0;

    void __fastcall ZSysMem_NewRefHook(ZSysMem *_this, void *_EDX, void *p_link);
    void __fastcall ZMalloc_RemoveFreeHeaderFromBinsHook(ZMalloc *_this, void *_EDX, SMallocFreeHeader *p_freeHeader, SBinTreeNode *p_mallocBin);

    void CreateHooks();
}