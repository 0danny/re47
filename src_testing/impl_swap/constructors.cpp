#include "constructors.h"

// Hooks constructors so that we can test our implementations.
// Ensure you use inplace "this" constructors to avoid memory leaks.

namespace Constructors
{
    // These hooks get injected once the render dll is loaded
    void CreateRenderingHooks()
    {
        ZMasterControl *control = new ZMasterControl();

        printf("[CONSTRUCTOR HOOK]: Creating rendering constructor hooks...\n");

        /*
        if (MH_CreateHook(linkSortRefTabAddress, (LPVOID)&Constructors::LinkSortRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook LinkSortRefTab constructor.\n");
        }


        if (MH_CreateHook(linkRefTabRenderingAddress, (LPVOID)&Constructors::LinkRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook LinkRefTab rendering constructor.\n");
        }
        */

        if (MH_CreateHook(zCounterAddress, (LPVOID)&Constructors::ZCounterConstructorHook, reinterpret_cast<LPVOID *>(&originalZCounter)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZCounter constructor.\n");
        }
    }

    void CreateHooks()
    {
        printf("[CONSTRUCTOR HOOK]: Creating constructor hooks...\n");

        if (MH_CreateHook(zInputActionsCtor, (LPVOID)&Constructors::ZInputActionsConstructorHook, reinterpret_cast<LPVOID *>(&originalZInputActions)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZInputActions constructor.\n");
        }

        /*
        if (MH_CreateHook(zMasterControlAddress, (LPVOID)&Constructors::ZMasterControlConstructorHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZMasterControl constructor.\n");
        }


        if (MH_CreateHook(zActionMapAddress, (LPVOID)&Constructors::ZActionMapConstructorHook, reinterpret_cast<LPVOID *>(&originalZActionMap)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZActionMap constructor.\n");
        }

        if (MH_CreateHook(zInputActionDefCtor, (LPVOID)&Constructors::ZInputActionDefCtorHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZInputAction constructor.\n");
        }

        if (MH_CreateHook(linkRefTabAddress, (LPVOID)&Constructors::LinkRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook LinkRefTab constructor.\n");
        }

        if (MH_CreateHook(refTabAddress, (LPVOID)&Constructors::RefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook RefTab constructor.\n");
        }

        // When the RefTab32 and AllocRefTab hooks are both active at the same time, it causes a crash. However, alone they work fine, weird....

        if (MH_CreateHook(ref32RefTabAddress, (LPVOID)&Constructors::RefTab32Hook, reinterpret_cast<LPVOID *>(&originalRefTab32)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook RefTab32 constructor.\n");
        }

        if (MH_CreateHook(staticRefTabAddress, (LPVOID)&Constructors::StaticRefTabHook, reinterpret_cast<LPVOID *>(&originalStaticRefTab)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook StaticRefTab constructor.\n");
        }

        if (MH_CreateHook(zConsoleAddress, (LPVOID)&Constructors::ZConsoleConstructorHook, reinterpret_cast<LPVOID *>(&originalZConsole)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZConsole constructor.\n");
        }

        if (MH_CreateHook(zSysComAddress, (LPVOID)&Constructors::ZSysComConstructorHook, reinterpret_cast<LPVOID *>(&originalZSysCom)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZSysCom constructor.\n");
        }

        if (MH_CreateHook(equRefTabAddress, (LPVOID)&Constructors::EquRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook EquRefTab constructor.\n");
        }

        if (MH_CreateHook(strRefTabAddress, (LPVOID)&Constructors::StrRefTabHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook StrRefTab constructor.\n");
        }

        if (MH_CreateHook(allocRefTabAddress, (LPVOID)&Constructors::AllocRefTabHook, reinterpret_cast<LPVOID *>(&originalAllocRefTab)) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook AllocRefTab constructor.\n");
        }

        if (MH_CreateHook(zrbTreeAddress, (LPVOID)&Constructors::ZRBTreeConstructorHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZRBTree constructor.\n");
        }

        if (MH_CreateHook(zvalTreeAddress, (LPVOID)&Constructors::ZValTreeConstructorHook, NULL) != MH_OK)
        {
            printf("[CONSTRUCTOR HOOK]: Could not hook ZValTree constructor.\n");
        }*/
    }

    RefTab *__fastcall RefTabHook(RefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("[CONSTRUCTOR HOOK]: RefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size); // spams the console

        return new (_this) RefTab(p_poolSize, p_size);
    }

    RefTab32 *__fastcall RefTab32Hook(RefTab32 *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: RefTab32 called\n");

        return new (_this) RefTab32();
    }

    StaticRefTab *__fastcall StaticRefTabHook(StaticRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        printf("[CONSTRUCTOR HOOK]: StaticRefTab called -> Pool Size: %d, Size: %d\n", p_poolSize, p_size);

        return new (_this) StaticRefTab(p_poolSize, p_size);
    }

    AllocRefTab *__fastcall AllocRefTabHook(AllocRefTab *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: AllocRefTab called\n");

        return new (_this) AllocRefTab();
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

    LinkSortRefTab *__fastcall LinkSortRefTabHook(LinkSortRefTab *_this, void *_EDX, int p_poolSize, int p_size)
    {
        LinkSortRefTab *newPtr = new (_this) LinkSortRefTab(p_poolSize, p_size);

        // printf("[CONSTRUCTOR HOOK]: LinkSortRefTab called -> Pool Size: %d, Size: %d\n", newPtr->m_poolSize, newPtr->m_size);

        return newPtr;
    }

    ZRBTree *__fastcall ZRBTreeConstructorHook(ZRBTree *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: ZRBTree called\n");

        return new (_this) ZRBTree();
    }

    ZValTree *__fastcall ZValTreeConstructorHook(ZValTree *_this, void *_EDX, int p_poolSize)
    {
        printf("[CONSTRUCTOR HOOK]: ZValTree called -> Pool Size: %d\n", p_poolSize);

        return new (_this) ZValTree(p_poolSize);
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

    ZSysCom *__fastcall ZSysComConstructorHook(ZSysCom *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: ZSysCom called\n");

        // return originalZSysCom(_this);

        return new (_this) ZSysCom();
    }

    ZCounter *__fastcall ZCounterConstructorHook(ZCounter *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: ZCounter called\n");

        return new (_this) ZCounter();
    }

    ZInputAction *__fastcall ZInputActionDefCtorHook(ZInputAction *_this, void *_EDX, ZActionMap *p_actionMap, SActionOverride *p_actionOverride)
    {
        printf("[CONSTRUCTOR HOOK]: ZInputAction called\n");

        return new (_this) ZInputAction(p_actionMap, p_actionOverride);
    }

    ZActionMap *__fastcall ZActionMapConstructorHook(ZActionMap *_this, void *_EDX, ZInputActions *p_inputActions, ZActionMapDefinition *p_actionMapDef, char *p_mapName)
    {
        printf("[CONSTRUCTOR HOOK]: ZActionMap called\n");

        return new (_this) ZActionMap(p_inputActions, p_actionMapDef, p_mapName);
    }

    ZInputActions *__fastcall ZInputActionsConstructorHook(ZInputActions *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: ZInputActions called, sizeof -> %d\n", sizeof(ZInputActions));

        void *l_malloc = operator new(sizeof(ZInputActions));

        // ZInputActions *og = originalZInputActions((ZInputActions *)l_malloc, _EDX);

        ZInputActions *newest = new (_this) ZInputActions();

        // set newest vtable to original vtable
        //*(void ***)newest = *(void ***)og;

        /*
        newest->m_inputNodeList = og->m_inputNodeList;
        newest->m_unkInt2 = og->m_unkInt2;
        newest->m_unkInt3 = og->m_unkInt3;
        newest->m_inputNodeCount = og->m_inputNodeCount;
        newest->m_actionMapDefinitions = og->m_actionMapDefinitions;
        newest->m_refTab2 = og->m_refTab2;
        newest->m_curStates = og->m_curStates;
        newest->m_unkFloat1 = og->m_unkFloat1;
        newest->m_unkFloat2 = og->m_unkFloat2;
        newest->m_unkFloat3 = og->m_unkFloat3;
        newest->m_fastLookup = og->m_fastLookup;
        newest->m_actionMap = og->m_actionMap;
        newest->m_overrideList = og->m_overrideList;*/

        printf("[CONSTRUCTOR HOOK]: Node Count -> %d\n", newest->m_inputNodeCount);

        return newest;
    }

    ZMasterControl *__fastcall ZMasterControlConstructorHook(ZMasterControl *_this, void *_EDX)
    {
        printf("[CONSTRUCTOR HOOK]: ZMasterControl called\n");

        return new (_this) ZMasterControl();
    }
}
