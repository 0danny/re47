#pragma once

#include "zstdlib/reftab/equreftab.h"
#include "zstdlib/reftab/strreftab.h"
#include "zstdlib/zconsole/zconsole.h"

#include "MinHook.h"

namespace Constructors
{
    // Addresses
    static LPVOID refTabAddress = (LPVOID)0x0FFBED50;
    static LPVOID equRefTabAddress = (LPVOID)0x0FFBFE80;
    static LPVOID strRefTabAddress = (LPVOID)0x0FFC0D30;
    static LPVOID zConsoleAddress = (LPVOID)0x0FFC5400;

    // Typedefs
    typedef RefTab *(__fastcall *RefTabConstructor)(RefTab *_this, int p_poolSize, int p_size);
    typedef EquRefTab *(__fastcall *EquRefTabConstructor)(EquRefTab *_this, int p_poolSize, int p_size);
    typedef StrRefTab *(__fastcall *StrRefTabConstructor)(StrRefTab *_this, int p_poolSize, int p_size);
    typedef ZConsole *(__fastcall *ZConsoleConstructor)(ZConsole *_this);

    static ZConsoleConstructor originalZConsole = 0;

    // Functions
    RefTab *__fastcall RefTabHook(RefTab *_this, void *_EDX, int p_poolSize, int p_size);
    EquRefTab *__fastcall EquRefTabHook(EquRefTab *_this, void *_EDX, int p_poolSize, int p_size);
    StrRefTab *__fastcall StrRefTabHook(StrRefTab *_this, void *_EDX, int p_poolSize, int p_size);
    ZConsole *__fastcall ZConsoleConstructorHook(ZConsole *_this, void *_EDX);

    void CreateHooks();
}