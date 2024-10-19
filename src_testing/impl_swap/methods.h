#pragma once

#include "utilities.h"
#include "zstdlib/zmalloc.h"
#include "system/zsysmem.h"

#include <minhook.h>

namespace Methods
{
    static LPVOID zSysMem_NewRef = (LPVOID)0x0FFB1E30;

    typedef void(__fastcall *ZSysMem_NewRef)(ZSysMem *_this, void *p_link);

    static ZSysMem_NewRef originalZSysMem_NewRef = 0;

    void __fastcall ZSysMem_NewRefHook(ZSysMem *_this, void *_EDX, void *p_link);

    void CreateHooks();
}