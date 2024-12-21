#include "methods.h"

namespace Methods
{
    void CreateHooks()
    {
        printf("[METHOD HOOK]: Creating method hooks...\n");

        /*
            if (MH_CreateHook(zInputActions_Dtor, (LPVOID)&Methods::ZInputActions_DtorHook, NULL) != MH_OK)
            {
                printf("[METHOD HOOK]: Could not hook ZInputActions_Dtor method.\n");
            }

            if (MH_CreateHook(zSysCom_LogMessage, (LPVOID)&Methods::ZSysCom_LogMessageHook, NULL) != MH_OK)
            {
                printf("[METHOD HOOK]: Could not hook ZSysCom_LogMessage method.\n");
            }*/

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

    void __fastcall ZSysCom_LogMessageHook(ZSysCom *_this, void *_EDX, char *p_format, ...)
    {
        char *l_bufferPtr;    // edi
        ZConsole *l_console;  // eax
        HWND l_unkHwnd;       // edx
        COPYDATASTRUCT l_cds; // [esp+4h] [ebp-100Ch] BYREF
        char l_buffer[4096];  // [esp+10h] [ebp-1000h] BYREF
        va_list l_argList;    // [esp+101Ch] [ebp+Ch] BYREF

        va_start(l_argList, p_format);
        if ((!g_pSysInterface || g_pSysInterface->m_isMessagingEnabled || g_pSysInterface->m_enableDebugOptions != 0.0) && _this->m_debugWndReady)
        {
            while (_this->m_sendingZMessage)
                ;
            _this->m_sendingZMessage = 1;

            _this->FormatString(l_buffer, "(%s:%d)", _this->m_filePath, _this->m_lineNum);

            l_bufferPtr = &l_buffer[strlen(l_buffer)];

            vsprintf(l_bufferPtr, p_format, l_argList);

            if (*l_bufferPtr == '(')
                strcpy(l_buffer, l_bufferPtr);

            if (!l_buffer[0] || *((char *)&l_cds.lpData + strlen(l_buffer) + 3) != '\n')
                strcat(l_buffer, "\n");

            printf("[METHOD HOOK]: LogMessage -> %s\n", l_buffer);

            if (g_pSysMem->unkByte1 && g_pSysInterface && g_pSysInterface->GetConsole())
            {
                l_console = g_pSysInterface->GetConsole();
                l_console->AddCmdText(l_buffer);
            }

            l_unkHwnd = _this->m_mainHwnd;

            if (l_unkHwnd)
            {
                l_cds.cbData = strlen(l_buffer) + 1;
                l_cds.dwData = 0;
                l_cds.lpData = l_buffer;

                SendMessageA(l_unkHwnd, 74u, 0, (LPARAM)&l_cds);
            }

            _this->m_sendingZMessage = 0;
        }
    }
}
