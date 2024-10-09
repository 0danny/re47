#include "constructors.h"

// Hooks constructors so that we can test our implementations.
// Ensure you use inplace "this" constructors to avoid memory leaks.

namespace Constructors
{
    void CreateHooks()
    {
        printf("[CONSTRUCTOR HOOK]: Creating constructor hooks...\n");

        if (MH_CreateHook(refTabAddress, (LPVOID)&Constructors::RefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook RefTab constructor.\n");
        }

        if (MH_CreateHook(ref32RefTabAddress, (LPVOID)&Constructors::RefTab32Hook, reinterpret_cast<LPVOID *>(&originalRefTab32)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook RefTab32 constructor.\n");
        }

        if (MH_CreateHook(staticRefTabAddress, (LPVOID)&Constructors::StaticRefTabHook, reinterpret_cast<LPVOID *>(&originalStaticRefTab)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook StaticRefTab constructor.\n");
        }

        if (MH_CreateHook(equRefTabAddress, (LPVOID)&Constructors::EquRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook EquRefTab constructor.\n");
        }

        if (MH_CreateHook(strRefTabAddress, (LPVOID)&Constructors::StrRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook StrRefTab constructor.\n");
        }

        if (MH_CreateHook(linkRefTabAddress, (LPVOID)&Constructors::LinkRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook LinkRefTab constructor.\n");
        }

        if (MH_CreateHook(zConsoleAddress, (LPVOID)&Constructors::ZConsoleConstructorHook, reinterpret_cast<LPVOID *>(&originalZConsole)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZConsole constructor.\n");
        }
    }

    RefTab *__fastcall RefTabHook(RefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        // printf("[CONSTRUCTOR HOOK]: RefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size); //spams the console

        return new (_this) RefTab(p_poolSize, p_size);
    }

    RefTab32 *__fastcall RefTab32Hook(RefTab32 *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: RefTab32 called\n");

        // using the _this ptr, we place our reftab32 object at the same address as the original object
        return new (_this) RefTab32();
    }

    StaticRefTab *__fastcall StaticRefTabHook(StaticRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("[CONSTRUCTOR HOOK]: StaticRefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new (_this) StaticRefTab(p_poolSize, p_size);
    }

    EquRefTab *__fastcall EquRefTabHook(EquRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("[CONSTRUCTOR HOOK]: EquRefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new (_this) EquRefTab(p_poolSize, p_size);
    }

    StrRefTab *__fastcall StrRefTabHook(StrRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("[CONSTRUCTOR HOOK]: StrRefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new (_this) StrRefTab(p_poolSize, p_size);
    }

    LinkRefTab *__fastcall LinkRefTabHook(LinkRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("[CONSTRUCTOR HOOK]: LinkRefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new (_this) LinkRefTab(p_poolSize, p_size);
    }

    ZConsole *__fastcall ZConsoleConstructorHook(ZConsole *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: ZConsole called\n");

        ZConsole *res = new (_this) ZConsole();

        res->AddCmdText("------------ Implementation Swap Loaded ------------");
        res->AddCmdText("re47 project");
        res->AddCmdText("----------------------------------------------------");

        return res;
    }
}
