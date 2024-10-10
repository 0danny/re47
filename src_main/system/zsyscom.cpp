#include "zsyscom.h"

ZSysCom::ZSysCom()
{
    m_unkInt1 = 1234;
    m_unkPtr = 0;
    m_unkBool1 = 0;

    m_unkPtr2 = 0;
    m_sendingZMessage = 0;
    m_filePath = 0;
    m_lineNum = 0;
    m_unkBool3 = 0;
    m_unkHwnd = 0;
    m_msgID = RegisterWindowMessageA("ZSystemMessage");

    g_pSysCom = this;
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

ZSysCom::~ZSysCom()
{
}

void ZSysCom::DestroyArrays()
{
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

            if (m_unkHwnd)
                break;

            Sleep(5);

            if (++l_count >= 200)
            {
                break;
            }
        }

        m_unkBool3 = 1;
    }
    else
    {
        m_unkBool3 = 1;
    }
}

void ZSysCom::SendDebugMsg(WPARAM p_wParam, LPARAM p_lParam, boolean p_sendMessage)
{
    HWND l_unkHwnd = m_unkHwnd;

    if (p_sendMessage)
    {
        if (l_unkHwnd)
            SendMessageA(0, m_msgID, p_wParam + (m_unkInt1 << 8), p_lParam);
    }
    else
    {
        if (!l_unkHwnd)
            l_unkHwnd = HWND_BROADCAST;

        PostMessageA(l_unkHwnd, m_msgID, p_wParam + (m_unkInt1 << 8), p_lParam);
    }
}

i32 ZSysCom::ForwardWndProc(u32 p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
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
}

void ZSysCom::DataToDebug(char *p_format, ...)
{
    char l_buffer[1024];
    va_list l_argList;

    va_start(l_argList, p_format);

    if (m_unkHwnd)
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
}

void ZSysCom::UnkFunc5(i32 p_unkInt, char *p_format, ...)
{
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

        if (!l_buffer[0] || *((byte *)&l_copyData.lpData + strlen(l_buffer) + 3) != '\n')
            strcat(l_buffer, "\n");

        HWND l_unkHwnd = m_unkHwnd;

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

void ZSysCom::UnkFunc7(char *p_format, ...)
{
}

void *ZSysCom::UnkFunc8(char *p_unkChar)
{
    return 0;
}

void ZSysCom::UnkFunc9()
{
}
