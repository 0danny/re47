#include "zsyscom.h"

ZSysComBase::ZSysComBase()
{
    this->m_unkInt1 = 1234;
    this->m_unkPtr = 0;
    this->m_unkBool1 = 0;
}

ZSysComBase::~ZSysComBase()
{
}

void ZSysComBase::UnkFunc1()
{
}

int ZSysComBase::GetUnkInt1()
{
    return this->m_unkInt1;
}

UINT ZSysComBase::GetMsgID()
{
    return this->m_msgID;
}

ZSysCom::ZSysCom()
{
    this->m_unkPtr2 = 0;
    this->m_sendingZMessage = 0;
    this->m_filePath = 0;
    this->m_lineNum = 0;
    this->m_unkBool3 = 0;
    this->m_unkHwnd = 0;
    this->m_msgID = RegisterWindowMessageA("ZSystemMessage");

    g_pSysCom = this;
}

int ZSysCom::FormatString(char *p_resultBuffer, char *p_format, ...)
{
    va_list l_argList;
    va_start(l_argList, p_format);
    return vsprintf(p_resultBuffer, p_format, l_argList);
}

ZSysCom::~ZSysCom()
{
}

void ZSysCom::ProcessDebugWnd(HWND p_hWnd)
{
    struct tagMSG l_msg;

    this->m_debugHwnd = p_hWnd;
    this->SendDebugMsg(9, (WPARAM)p_hWnd, 0);

    HWND l_findWnd = FindWindowA(0, "ZDebug");

    if (l_findWnd)
    {
        int l_count = 0;

        while (1)
        {
            while (PeekMessageA(&l_msg, p_hWnd, 0, 0, 0))
            {
                if (!GetMessageA(&l_msg, p_hWnd, 0, 0))
                    break;

                TranslateMessage(&l_msg);
                DispatchMessageA(&l_msg);
            }

            if (this->m_unkHwnd)
                break;

            Sleep(5);

            if (++l_count >= 200)
            {
                break;
            }
        }

        this->m_unkBool3 = 1;
    }
    else
    {
        this->m_unkBool3 = 1;
    }
}

void ZSysCom::SendDebugMsg(WPARAM p_wParam, LPARAM p_lParam, bBool p_sendMessage)
{
    HWND l_unkHwnd = this->m_unkHwnd;

    if (p_sendMessage)
    {
        if (l_unkHwnd)
            SendMessageA(0, this->m_msgID, p_wParam + (this->m_unkInt1 << 8), p_lParam);
    }
    else
    {
        if (!l_unkHwnd)
            l_unkHwnd = HWND_BROADCAST;

        PostMessageA(l_unkHwnd, this->m_msgID, p_wParam + (this->m_unkInt1 << 8), p_lParam);
    }
}

int ZSysCom::ForwardWndProc(UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
    return 0;
}

ZSysCom *ZSysCom::SetPathAndLine(char *p_filePath, int p_lineNum)
{
    ZSysCom *l_result = this;

    this->m_filePath = p_filePath;
    this->m_lineNum = p_lineNum;

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

    if (this->m_unkHwnd)
    {
        vsprintf(l_buffer, p_format, l_argList);

        if (l_buffer[0])
        {
            ATOM l_atom = GlobalAddAtomA(l_buffer);

            if (l_atom)
            {
                this->SendDebugMsg(12, l_atom, 0);
            }
            else
            {
                ZSysCom *l_this = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysComWintel.cpp", 216);

                l_this->UnkFunc4("ZSysComWintel::DataToDebug: Couldn't send \"%s\"", l_buffer);
            }
        }
        else
        {
            ZSysCom *l_this2 = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\_Wintel\\Source\\SysComWintel.cpp", 209);

            l_this2->UnkFunc4("ZSysComWintel::DataToDebug: Tried to send empty string");
        }
    }
}

void ZSysCom::UnkFunc4(char *p_format, ...)
{
}

void ZSysCom::UnkFunc5(int p_unkInt, char *p_format, ...)
{
}

void ZSysCom::SendZMessage(char *p_format, ...)
{
    COPYDATASTRUCT l_copyData;
    char l_buffer[1024];
    va_list l_argList;

    va_start(l_argList, p_format);

    if (!this->m_sendingZMessage)
    {
        this->m_sendingZMessage = 1;

        vsprintf(l_buffer, p_format, l_argList);

        if (!l_buffer[0] || *((byte *)&l_copyData.lpData + strlen(l_buffer) + 3) != '\n')
            strcat(l_buffer, "\n");

        HWND l_unkHwnd = this->m_unkHwnd;

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

        this->m_sendingZMessage = 0;
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
