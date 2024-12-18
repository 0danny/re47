#include "methods.h"

namespace Methods
{
    void CreateHooks()
    {
        printf("[METHOD HOOK]: Creating method hooks...\n");

        if (MH_CreateHook(zInputActions_Dtor, (LPVOID)&Methods::ZInputActions_DtorHook, NULL) != MH_OK)
        {
            printf("[METHOD HOOK]: Could not hook ZInputActions_Dtor method.\n");
        }

        /*
        if (MH_CreateHook(zInputActions_OverrideAction, (LPVOID)&Methods::ZInputActions_OverrideActionHook, reinterpret_cast<LPVOID *>(&originalZInputActions_OverrideAction)) != MH_OK)
        {
            printf("[METHOD HOOK]: Could not hook ZInputActions_OverrideAction method.\n");
        }

        if (MH_CreateHook(cFastLookup_RemoveLowerCase, (LPVOID)&Methods::CFastLookup_RemoveLowerCaseHook, NULL) != MH_OK)
        {
            printf("[METHOD HOOK]: Could not hook CFastLookup_RemoveLowerCase method.\n");
        }

        if (MH_CreateHook(cFastLookup_GetLowerCase, (LPVOID)&Methods::CFastLookup_GetLowerCaseHook, NULL) != MH_OK)
        {
            printf("[METHOD HOOK]: Could not hook CFastLookup_GetLowerCase method.\n");
        }

        if (MH_CreateHook(zMalloc_RemoveFreeHeaderFromBins, (LPVOID)&Methods::ZMalloc_RemoveFreeHeaderFromBinsHook, NULL) != MH_OK)
        {
            printf("[METHOD HOOK]: Could not hook ZMalloc_RemoveFreeHeaderFromBins method.\n");
        }

        if (MH_CreateHook(zSysMem_NewRef, (LPVOID)&Methods::ZSysMem_NewRefHook, reinterpret_cast<LPVOID *>(&originalZSysMem_NewRef)) != MH_OK)
        {
            printf("[METHOD HOOK]: Could not hook ZSysMem_NewRef method.\n");
        }*/
    }

    void CreateRenderingHooks()
    {
    }

    void __fastcall ZSysMem_NewRefHook(ZSysMem *_this, void *_EDX, void *p_link)
    {
        printf("[METHOD HOOK]: ZSysMem_NewRefHook called.\n");

        SMallocBlockHeader *header = (SMallocBlockHeader *)((char *)p_link - sizeof(SMallocBlockHeader));

        UINT l_newRef = _this->m_allocTab->NewRef();

        _this->m_refArray[l_newRef & _this->m_allocTab->m_maxRefIndex] = (UINT)p_link;

        header->referenceNumber = l_newRef;

        if ((header->flag & 0x80000000) != 0)
        {
            int l_local = header->checksumComponents[0] ^
                          header->checksumComponents[1] ^
                          header->checksumComponents[2] ^
                          header->checksumComponents[3] ^
                          header->unkInt ^
                          header->flag ^
                          header->referenceNumber;

            printf("[METHOD HOOK]: Calculated checksum: %d\n", l_local);

            if (!l_local)
                l_local = 1;

            header->checksum = l_local;
        }
    }

    void __fastcall ZMalloc_RemoveFreeHeaderFromBinsHook(ZMalloc *_this, void *_EDX, SMallocFreeHeader *p_freeHeader, SBinTreeNode *p_mallocBin)
    {
        _this->RemoveFreeHeaderFromBins(p_freeHeader, p_mallocBin);
    }

    void __fastcall CFastLookup_RemoveLowerCaseHook(CFastLookup2 *_this, void *_EDX, const char *p_str, u32 p_size)
    {
        printf("[METHOD HOOK]: CFastLookup_RemoveLowerCaseHook called, [%s]\n", p_str);

        _this->RemoveLowerCase(p_str, p_size);
    }

    void __fastcall CFastLookup_GetLowerCaseHook(CFastLookup2 *_this, void *_EDX, const char *p_str)
    {
        printf("[METHOD HOOK]: CFastLookup_GetLowerCaseHook called, [%s]\n", p_str);

        _this->GetLowerCase(p_str);
    }

    void __fastcall ZInputActions_OverrideActionHook(ZInputActions *_this, void *_EDX, SActionOverride *p_actionOverride)
    {
        printf("[METHOD HOOK]: ZInputActions_OverrideActionHook called for %s\n", p_actionOverride->actionName.m_pointer);

        printf("[METHOD HOOK]: Address of this %p\n", _this);
        // originalZInputActions_OverrideAction(_this, _EDX, p_actionOverride);
    }

    void __fastcall ZInputActions_DtorHook(ZInputActions *_this, void *_EDX)
    {
        printf("[METHOD HOOK]: ZInputActions_DtorHook called.\n");

        _this->~ZInputActions();
    }
}
