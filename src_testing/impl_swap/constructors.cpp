#include "constructors.h"

// Hooks constructors so that we can test our implementations.
// This causes a slight mem leak because the original allocated ptrs are not free'd when the constructors are hooked.

namespace Constructors
{
    void CreateHooks()
    {
        printf("[CONSTRUCTOR HOOK]: Creating constructor hooks...\n");

        if (MH_CreateHook(refTabAddress, (LPVOID)&Constructors::RefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook RefTab constructor.\n");
        }

        if (MH_CreateHook(equRefTabAddress, (LPVOID)&Constructors::EquRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook EquRefTab constructor.\n");
        }

        if (MH_CreateHook(strRefTabAddress, (LPVOID)&Constructors::StrRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook StrRefTab constructor.\n");
        }

        if (MH_CreateHook(zConsoleAddress, (LPVOID)&Constructors::ZConsoleConstructorHook, reinterpret_cast<LPVOID *>(&originalZConsole)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZConsole constructor.\n");
        }
    }

    RefTab *__fastcall RefTabHook(RefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        // printf("[CONSTRUCTOR HOOK]: RefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size); //spams the console

        return new RefTab(p_poolSize, p_size);
    }

    EquRefTab *__fastcall EquRefTabHook(EquRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("[CONSTRUCTOR HOOK]: EquRefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new EquRefTab(p_poolSize, p_size);
    }

    StrRefTab *__fastcall StrRefTabHook(StrRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("[CONSTRUCTOR HOOK]: StrRefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new StrRefTab(p_poolSize, p_size);
    }

    ZConsole *__fastcall ZConsoleConstructorHook(ZConsole *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: ZConsole called\n");

        ZConsole *res = new ZConsole();

        res->AddCmdText("-----------------------------------");
        res->AddCmdText("[IMPLEMENTATION SWAP] - Loaded");
        res->AddCmdText(" ** Re47 Project **");
        res->AddCmdText("-----------------------------------");

        return res;
    }
}
