#include "constructors.h"

namespace Constructors
{
    void CreateHooks()
    {
        if (MH_CreateHook(refTabAddress, (LPVOID)&Constructors::RefTabHook, NULL) != MH_OK)
        {
            printf("Could not hook RefTab constructor.\n");
        }

        if (MH_CreateHook(equRefTabAddress, (LPVOID)&Constructors::EquRefTabHook, NULL) != MH_OK)
        {
            printf("Could not hook EquRefTab constructor.\n");
        }

        if (MH_CreateHook(strRefTabAddress, (LPVOID)&Constructors::StrRefTabHook, NULL) != MH_OK)
        {
            printf("Could not hook StrRefTab constructor.\n");
        }
    }

    RefTab *__fastcall RefTabHook(RefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("RefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new RefTab(p_poolSize, p_size);
    }
    EquRefTab *__fastcall EquRefTabHook(EquRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("EquRefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new EquRefTab(p_poolSize, p_size);
    }
    StrRefTab *__fastcall StrRefTabHook(StrRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("StrRefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new StrRefTab(p_poolSize, p_size);
    }
}