#include "directplay.h"

DirectPlay::DirectPlay()
{
    m_sessionName = 0;
    m_str2 = 0;
    m_str3 = 0;
    m_lpDP = 0;
    m_tcpGUID = 0;

    m_sessionName = new char[strlen("lortesession") + 1];
    strcpy(m_sessionName, "lortesession");

    m_str2 = new char[strlen("lorteliv") + 1];
    strcpy(m_str2, "lorteliv");

    m_str3 = new char[strlen("lorte liv") + 1];
    strcpy(m_str3, "lorte liv");

    LPDIRECTPLAY l_dp1;

    DirectPlayEnumerateA(EnumCallback, &m_tcpGUID);
    DirectPlayCreate(m_tcpGUID, &l_dp1, 0);

    l_dp1->QueryInterface(IID_IDirectPlay2, (void **)&m_lpDP);
    l_dp1->Close();
    l_dp1->Release();

    if (GetSessions())
    {
        m_foundSession = false;
        OpenSession();
    }
    else
    {
        m_foundSession = true;
        OpenSessionName(10);
    }
}

DirectPlay::~DirectPlay()
{
    FreeData();
}

i32 __stdcall DirectPlay::EnumCallback(LPGUID lpguidSP, LPSTR lpSPName, DWORD dwMajorVersion, DWORD dwMinorVersion, LPVOID lpContext)
{
    if (!strstr(lpSPName, "TCP"))
        return 1;

    GUID *l_guid = new GUID;

    *(GUID **)lpContext = l_guid;

    *l_guid = *lpguidSP;

    return 0;
}

i32 __stdcall DirectPlay::EnumSessions(LPCDPSESSIONDESC2 lpThisSD, LPDWORD lpdwTimeOut, DWORD dwFlags, LPVOID lpContext)
{
    if (dwFlags)
    {
        *(BOOL *)lpContext = 0;
    }
    else
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\Network\\_Wintel\\DirectPlay\\Source\\DirectPlay.cpp", 223);
        l_sysCom->LogMessage("Found Session: %s", lpThisSD->lpszSessionName);

        *(BOOL *)lpContext = 0;
    }

    return 0;
}

i32 DirectPlay::FindSession()
{
    LPDIRECTPLAY l_lpDP;
    DirectPlayCreate(m_tcpGUID, &l_lpDP, 0);

    l_lpDP->QueryInterface(IID_IDirectPlay2, (void **)&m_lpDP);

    l_lpDP->Close();
    l_lpDP->Release();

    if (GetSessions())
    {
        m_foundSession = false;
        return OpenSession();
    }
    else
    {
        m_foundSession = true;
        return OpenSessionName(10);
    }
}

void DirectPlay::FreeData()
{
    m_lpDP->Close();
    m_lpDP->Release();

    delete[] m_sessionName;
    delete[] m_str2;
    delete[] m_str3;
    delete m_tcpGUID;
}

int DirectPlay::EnumProxy()
{
    return DirectPlayEnumerateA(EnumCallback, &m_tcpGUID);
}

boolean DirectPlay::GetSessions()
{
    DPSESSIONDESC2 l_sessionDesc;

    memset(&l_sessionDesc, 0, sizeof(l_sessionDesc));

    l_sessionDesc.guidInstance.Data1 = 0x26DF80B0;
    l_sessionDesc.guidInstance.Data2 = 0x11D3;
    l_sessionDesc.guidInstance.Data3 = 0x3DE8;

    l_sessionDesc.guidInstance.Data4[0] = 0x7D;
    l_sessionDesc.guidInstance.Data4[1] = 0x22;
    l_sessionDesc.guidInstance.Data4[2] = 0x90;
    l_sessionDesc.guidInstance.Data4[3] = 0x18;

    l_sessionDesc.guidInstance.Data4[4] = 0xE0;
    l_sessionDesc.guidInstance.Data4[5] = 0x00;
    l_sessionDesc.guidInstance.Data4[6] = 0x29;
    l_sessionDesc.guidInstance.Data4[7] = 0x81;

    l_sessionDesc.dwSize = 80;
    boolean l_context = 0;

    m_lpDP->EnumSessions(&l_sessionDesc, 1000, EnumSessions, &l_context, 0);

    return l_context;
}

HRESULT DirectPlay::OpenSessionName(DWORD p_maxPlayers)
{
    DPSESSIONDESC2 l_sessionDesc;

    memset(&l_sessionDesc, 0, sizeof(l_sessionDesc));

    l_sessionDesc.lpszSessionName = (LPWSTR)m_sessionName;
    l_sessionDesc.dwMaxPlayers = p_maxPlayers;

    l_sessionDesc.guidInstance.Data1 = 0x26DF80B0;
    l_sessionDesc.guidInstance.Data2 = 0x11D3;
    l_sessionDesc.guidInstance.Data3 = 0x3DE8;

    l_sessionDesc.guidInstance.Data4[0] = 0xE0;
    l_sessionDesc.guidInstance.Data4[1] = 0x00;
    l_sessionDesc.guidInstance.Data4[2] = 0x29;
    l_sessionDesc.guidInstance.Data4[3] = 0x81;
    l_sessionDesc.guidInstance.Data4[4] = 0x7D;
    l_sessionDesc.guidInstance.Data4[5] = 0x22;
    l_sessionDesc.guidInstance.Data4[6] = 0x90;
    l_sessionDesc.guidInstance.Data4[7] = 0x18;

    l_sessionDesc.dwFlags = 2;
    l_sessionDesc.dwSize = 80;
    l_sessionDesc.dwUser1 = 2;

    return m_lpDP->Open(&l_sessionDesc, 0);
}

HRESULT DirectPlay::OpenSession()
{
    DPSESSIONDESC2 l_sessionDesc;

    memset(&l_sessionDesc, 0, sizeof(l_sessionDesc));

    l_sessionDesc.guidInstance.Data1 = 0x26DF80B0;
    l_sessionDesc.guidInstance.Data2 = 0x11D3;
    l_sessionDesc.guidInstance.Data3 = 0x3DE8;

    l_sessionDesc.guidInstance.Data4[0] = 0xE0;
    l_sessionDesc.guidInstance.Data4[1] = 0x00;
    l_sessionDesc.guidInstance.Data4[2] = 0x29;
    l_sessionDesc.guidInstance.Data4[3] = 0x81;
    l_sessionDesc.guidInstance.Data4[4] = 0x7D;
    l_sessionDesc.guidInstance.Data4[5] = 0x22;
    l_sessionDesc.guidInstance.Data4[6] = 0x90;
    l_sessionDesc.guidInstance.Data4[7] = 0x18;

    l_sessionDesc.dwFlags = 1;
    l_sessionDesc.dwSize = 80;
    l_sessionDesc.dwUser1 = 2;

    return m_lpDP->Open(&l_sessionDesc, 0);
}
