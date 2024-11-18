#ifndef DIRECTPLAY_H
#define DIRECTPLAY_H

#include "system/zsyscom.h"
#include "common.h"
#include <directx7/dplay.h>

#pragma pack(push, 1)

class DirectPlay
{
public:
    boolean m_foundSession; // 4
    char *m_sessionName;    // 5
    char *m_str2;           // 9
    char *m_str3;           // 13
    i32 m_unused;           // 17
    LPDIRECTPLAY2 m_lpDP;   // 21
    GUID *m_tcpGUID;        // 25

    DirectPlay();
    virtual ~DirectPlay();

    virtual i32 FindSession();
    virtual inline void FreeData();
    virtual i32 EnumProxy();
    virtual boolean GetSessions();
    virtual HRESULT OpenSessionName(DWORD p_maxPlayers);
    virtual HRESULT OpenSession();
};

i32 __stdcall EnumCallback(LPGUID lpguidSP, LPSTR lpSPName, DWORD dwMajorVersion, DWORD dwMinorVersion, LPVOID lpContext);
i32 __stdcall EnumSessions(LPCDPSESSIONDESC2 lpThisSD, LPDWORD lpdwTimeOut, DWORD dwFlags, LPVOID lpContext);

#pragma pack(pop)

#endif