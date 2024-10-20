#include "methods.h"

namespace Methods
{
    void CreateHooks()
    {
        printf("[METHOD HOOK]: Creating method hooks...\n");

        /*
                if (MH_CreateHook(zMalloc_RemoveFreeHeaderFromBins, (LPVOID)&Methods::ZMalloc_RemoveFreeHeaderFromBinsHook, NULL) != MH_OK)
                {
                    printf("[METHOD HOOK]: Could not hook ZMalloc_RemoveFreeHeaderFromBins method.\n");
                }


                if (MH_CreateHook(zSysMem_NewRef, (LPVOID)&Methods::ZSysMem_NewRefHook, reinterpret_cast<LPVOID *>(&originalZSysMem_NewRef)) != MH_OK)
                {
                    printf("[METHOD HOOK]: Could not hook ZSysMem_NewRef method.\n");
                }*/
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
}
