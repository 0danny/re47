#pragma once

#include "zstdlib/reftab/equreftab.h"
#include "zstdlib/reftab/strreftab.h"
#include "zstdlib/zconsole/zconsolecommand.h"

#include "MinHook.h"

namespace Constructors
{
    // Addresses
    static LPVOID refTabAddress = (LPVOID)0x0FFBED50;
    static LPVOID equRefTabAddress = (LPVOID)0x0FFBFE80;
    static LPVOID strRefTabAddress = (LPVOID)0x0FFC0D30;
    static LPVOID zconsoleCommandAddress = (LPVOID)0x0FFC8900;

    // Typedefs
    typedef RefTab *(__fastcall *RefTabConstructor)(RefTab *_this, int p_poolSize, int p_size);
    typedef EquRefTab *(__fastcall *EquRefTabConstructor)(EquRefTab *_this, int p_poolSize, int p_size);
    typedef StrRefTab *(__fastcall *StrRefTabConstructor)(StrRefTab *_this, int p_poolSize, int p_size);
    typedef ZConsoleCommand *(__fastcall *ZConsoleCommandConstructor)(ZConsoleCommand *_this);

    static ZConsoleCommandConstructor originalZConsoleCommand = 0;

    // Functions
    RefTab *__fastcall RefTabHook(RefTab *_this, void *_EDX, int p_poolSize, int p_size);
    EquRefTab *__fastcall EquRefTabHook(EquRefTab *_this, void *_EDX, int p_poolSize, int p_size);
    StrRefTab *__fastcall StrRefTabHook(StrRefTab *_this, void *_EDX, int p_poolSize, int p_size);
    ZConsoleCommand *__fastcall ZConsoleCommandHook(ZConsoleCommand *_this, void *_EDX);

    void CreateHooks();
}