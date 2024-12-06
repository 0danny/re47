#include "zsyscom.h"
#include "system/zsysmem.h"
#include "system/zsysinterface.h"
#include "zstdlib/reftab/linkreftab.h"
#include "zstdlib/reftab/strreftab.h"

ZSysCom::ZSysCom()
{
    m_unkInt1 = 1234;
    m_linkRefTab = 0;
    m_unkBool1 = 0;

    m_logsArray = 0;
    m_sendingZMessage = 0;
    m_filePath = 0;
    m_lineNum = 0;
    m_debugWndReady = 0;
    m_mainHwnd = 0;
    m_msgID = RegisterWindowMessageA("ZSystemMessage");

    g_pSysCom = this;
}

ZSysCom::~ZSysCom()
{
    Destroy();
}

void ZSysCom::Destroy()
{
    if (!m_mainHwnd)
        m_mainHwnd = HWND_BROADCAST;

    PostMessageA(m_mainHwnd, m_msgID, (m_unkInt1 << 8) + 10, 0);

    g_pSysCom = 0;
}

i32 ZSysCom::GetUnkInt1()
{
    return m_unkInt1;
}

u32 ZSysCom::GetMsgID()
{
    return m_msgID;
}

i32 ZSysCom::FormatString(char *p_resultBuffer, char *p_format, ...)
{
    va_list l_argList;
    va_start(l_argList, p_format);
    return vsprintf(p_resultBuffer, p_format, l_argList);
}

void ZSysCom::DestroyArrays()
{
    if (m_logsArray)
        m_logsArray->~StrRefTab();

    if (m_linkRefTab)
        m_linkRefTab->~LinkRefTab();

    m_logsArray = 0;
}

void ZSysCom::ProcessDebugWnd(HWND p_hWnd)
{
    struct tagMSG l_msg;

    m_debugHwnd = p_hWnd;
    SendDebugMsg(9, (WPARAM)p_hWnd, 0);

    HWND l_findWnd = FindWindowA(0, "ZDebug");

    if (l_findWnd)
    {
        i32 l_count = 0;

        while (1)
        {
            while (PeekMessageA(&l_msg, p_hWnd, 0, 0, 0))
            {
                if (!GetMessageA(&l_msg, p_hWnd, 0, 0))
                    break;

                TranslateMessage(&l_msg);
                DispatchMessageA(&l_msg);
            }

            if (m_mainHwnd)
                break;

            Sleep(5);

            if (++l_count >= 200)
            {
                break;
            }
        }

        m_debugWndReady = 1;
    }
    else
    {
        m_debugWndReady = 1;
    }
}

void ZSysCom::SendDebugMsg(WPARAM p_wParam, LPARAM p_lParam, boolean p_sendMessage)
{
    HWND l_mainHwnd = m_mainHwnd;

    if (p_sendMessage)
    {
        if (l_mainHwnd)
            SendMessageA(0, m_msgID, p_wParam + (m_unkInt1 << 8), p_lParam);
    }
    else
    {
        if (!l_mainHwnd)
            l_mainHwnd = HWND_BROADCAST;

        PostMessageA(l_mainHwnd, m_msgID, p_wParam + (m_unkInt1 << 8), p_lParam);
    }
}

i32 ZSysCom::ForwardWndProc(u32 p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
    char l_buffer[256];
    i32 l_unused;

    if ((p_wParam & 0xFFFFFF00) == 0 || p_wParam >> 8 == m_unkInt1)
    {
        switch (p_wParam)
        {
        case 2:
            if (!m_mainHwnd)
                m_mainHwnd = (HWND)p_lParam;

            break;
        case 3:
            if (!m_mainHwnd)
            {
                m_mainHwnd = (HWND)p_lParam;

                SendDebugMsg(8, (LPARAM)m_debugHwnd, 0);
            }

            break;
        case 4:
            if (m_mainHwnd == (HWND)p_lParam)
                m_mainHwnd = 0;

            goto CASE_7;
        case 5:
            if (p_lParam)
            {
                if (m_debugWndReady)
                {
                    GlobalGetAtomNameA(p_lParam, l_buffer, 255);
                    GlobalDeleteAtom(p_lParam);
                    g_pSysInterface->UnkFunc5(l_buffer, 11);
                }
                else
                {
                    GlobalDeleteAtom(p_lParam);
                }
            }
            else
            {
                g_pSysCom->SendZMessage("ZSM_DOSETTING: Didn't get any string\n");
            }

            break;
        case 6:
            if (!m_linkRefTab)
            {
                m_linkRefTab = new LinkRefTab(128, 5);

                ++g_pSysInterface->m_unkInt99;
            }

            m_unkBool1 = 1;

            break;
        case 7:
        CASE_7:
            if (m_linkRefTab)
            {
                UnkFunc9();
                --g_pSysInterface->m_unkInt99;
            }

            m_unkBool1 = 0;

            break;
        default:
            return 0;
        }
    }
    return 0;
}

ZSysCom *ZSysCom::SetPathAndLine(char *p_filePath, i32 p_lineNum)
{
    ZSysCom *l_result = this;

    m_filePath = p_filePath;
    m_lineNum = p_lineNum;

    return l_result;
}

void ZSysCom::ThrowFatal(char *p_format, ...)
{
    char l_logMsg[8];
    char l_buffer[1024];

    va_list l_argList;
    va_start(l_argList, p_format);
    vsprintf(l_buffer, p_format, l_argList);

    g_pSysInterface->UnkFunc8();

    ZSysCom *l_pSysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysComWintel.cpp", 93);
    l_pSysCom->LogMessage("ERROR: %s", l_logMsg);

    MessageBoxA(0, l_logMsg, "Fatal error", MB_TOPMOST | MB_ICONSTOP);
    exit(-1);
}

void ZSysCom::DataToDebug(char *p_format, ...)
{
    char l_buffer[1024];

    va_list l_argList;
    va_start(l_argList, p_format);

    if (m_mainHwnd)
    {
        vsprintf(l_buffer, p_format, l_argList);

        if (l_buffer[0])
        {
            ATOM l_atom = GlobalAddAtomA(l_buffer);

            if (l_atom)
            {
                SendDebugMsg(12, l_atom, 0);
            }
            else
            {
                ZSysCom *l_this = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysComWintel.cpp", 216);
                l_this->LogMessage("ZSysComWintel::DataToDebug: Couldn't send \"%s\"", l_buffer);
            }
        }
        else
        {
            ZSysCom *l_this2 = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysComWintel.cpp", 209);
            l_this2->LogMessage("ZSysComWintel::DataToDebug: Tried to send empty string");
        }
    }
}

void ZSysCom::LogMessage(char *p_format, ...)
{
    COPYDATASTRUCT l_cds;
    char l_buffer[4096];

    va_list l_argList;
    va_start(l_argList, p_format);

    if ((!g_pSysInterface || g_pSysInterface->m_isMessagingEnabled || g_pSysInterface->m_enableDebugOptions != 0.0) && m_debugWndReady)
    {
        while (m_sendingZMessage)
            ;

        m_sendingZMessage = 1;

        FormatString(l_buffer, "(%s:%d)", m_filePath, m_lineNum);

        char *l_bufferPtr = &l_buffer[strlen(l_buffer)];

        vsprintf(l_bufferPtr, p_format, l_argList);

        if (*l_bufferPtr == '(')
            strcpy(l_buffer, l_bufferPtr);

        if (!l_buffer[0] || *((char *)&l_cds.lpData + strlen(l_buffer) + 3) != '\n')
            strcat(l_buffer, "\n");

        if (g_pSysMem->unkByte1 && g_pSysInterface && g_pSysInterface->GetConsole())
        {
            ZConsole *l_console = g_pSysInterface->GetConsole();
            l_console->AddCmdText(l_buffer);
        }

        if (m_mainHwnd)
        {
            l_cds.cbData = strlen(l_buffer) + 1;
            l_cds.dwData = 0;
            l_cds.lpData = l_buffer;

            SendMessageA(m_mainHwnd, 74u, 0, (LPARAM)&l_cds);
        }

        m_sendingZMessage = 0;
    }
}

void ZSysCom::UnkFunc5(i32 p_unused, char *p_format, ...)
{
    char l_buffer[4096];

    va_list l_argList;
    va_start(l_argList, p_format);

    if ((!g_pSysInterface || g_pSysInterface->m_isMessagingEnabled || g_pSysInterface->m_enableDebugOptions != 0.0) && m_debugWndReady)
    {
        if (!m_logsArray)
        {
            i32 l_unused = -1;

            m_logsArray = new StrRefTab(32, 0);
        }

        FormatString(l_buffer, "(%s:%d)", m_filePath, m_lineNum);

        char *l_ptr = &l_buffer[strlen(&l_buffer[4]) + 4];

        vsprintf(l_ptr, p_format, l_argList);

        if (*l_ptr == '(')
            strcpy(&l_buffer[4], l_ptr);

        if (m_logsArray->AddStr(&l_buffer[4]))
            LogMessage(l_buffer);
    }
}

void ZSysCom::SendZMessage(char *p_format, ...)
{
    COPYDATASTRUCT l_copyData;
    char l_buffer[1024];

    va_list l_argList;
    va_start(l_argList, p_format);

    if (!m_sendingZMessage)
    {
        m_sendingZMessage = 1;

        vsprintf(l_buffer, p_format, l_argList);

        if (!l_buffer[0] || *((char *)&l_copyData.lpData + strlen(l_buffer) + 3) != '\n')
            strcat(l_buffer, "\n");

        HWND l_unkHwnd = m_mainHwnd;

        if (l_unkHwnd)
        {
            l_copyData.cbData = strlen(l_buffer) + 1;
            l_copyData.dwData = 0;
            l_copyData.lpData = l_buffer;

            SendMessageA(l_unkHwnd, WM_COPYDATA, 0, (LPARAM)&l_copyData);
        }
        else
        {
            MessageBoxA(0, l_buffer, "ZSystem Default Output", MB_TOPMOST);
        }

        m_sendingZMessage = 0;
    }
}

void ZSysCom::AddCmdText(char *p_format, ...)
{
    char l_buffer[1024];

    va_list l_argList;
    va_start(l_argList, p_format);
    vsprintf(l_buffer, p_format, l_argList);

    if (!l_buffer[0] || l_buffer[strlen(l_buffer) + 1] != '\n')
        strcat(l_buffer, "\n");

    if (g_pSysInterface)
    {
        if (g_pSysInterface->GetConsole())
        {
            ZConsole *l_zConsole = g_pSysInterface->GetConsole();
            l_zConsole->AddCmdText(l_buffer);
        }
    }
}

void ZSysCom::UnkFunc8()
{
    printf("[ZSysCom::UnkFunc8] UNIMPLEMENTED\n");
}

void ZSysCom::UnkFunc9()
{
    printf("[ZSysCom::UnkFunc9] UNIMPLEMENTED\n");
}
