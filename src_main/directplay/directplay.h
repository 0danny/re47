#pragma once

#include "system/zsyscom.h"
#include "dplay.h"

#pragma pack(push, 1)

class DirectPlay
{
public:
    bBool m_foundSession;
    char *m_sessionName;
    char *m_str2;
    char *m_str3;
    int m_unused;
    LPDIRECTPLAY2 m_lpDP;
    GUID *m_tcpGUID;

    DirectPlay();
    virtual ~DirectPlay();

    virtual int FindSession();
    virtual inline void FreeData();
    virtual int EnumProxy();
    virtual bBool GetSessions();
    virtual HRESULT OpenSessionName(DWORD p_maxPlayers);
    virtual HRESULT OpenSession();
};

int __stdcall EnumCallback(LPGUID lpguidSP, LPSTR lpSPName, DWORD dwMajorVersion, DWORD dwMinorVersion, LPVOID lpContext);
int __stdcall EnumSessions(LPCDPSESSIONDESC2 lpThisSD, LPDWORD lpdwTimeOut, DWORD dwFlags, LPVOID lpContext);

#pragma pack(pop)
