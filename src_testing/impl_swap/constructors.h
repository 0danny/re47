#include "zstdlib/reftab/reftab.h"
#include "zstdlib/reftab/equreftab.h"
#include "zstdlib/reftab/strreftab.h"

#include "MinHook.h"

namespace Constructors
{
    // Addresses
    LPVOID refTabAddress = (LPVOID)0x0FFBED50;
    LPVOID equRefTabAddress = (LPVOID)0x0FFBFE80;
    LPVOID strRefTabAddress = (LPVOID)0x0FFC0D30;

    // Typedefs
    typedef RefTab *(__fastcall *RefTabConstructor)(RefTab *_this, int p_poolSize, int p_size);
    typedef EquRefTab *(__fastcall *EquRefTabConstructor)(EquRefTab *_this, int p_poolSize, int p_size);
    typedef StrRefTab *(__fastcall *StrRefTabConstructor)(StrRefTab *_this, int p_poolSize, int p_size);

    // Functions
    RefTab *__fastcall RefTabHook(RefTab *_this, void *_EDX, int p_poolSize, int p_size);
    EquRefTab *__fastcall EquRefTabHook(EquRefTab *_this, void *_EDX, int p_poolSize, int p_size);
    StrRefTab *__fastcall StrRefTabHook(StrRefTab *_this, void *_EDX, int p_poolSize, int p_size);

    void CreateHooks();
}