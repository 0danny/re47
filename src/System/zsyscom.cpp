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
    this->m_unkBool2 = 0;
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

HWND ZSysCom::ProcessDebugWnd(HWND p_hWnd)
{
    return HWND();
}

void ZSysCom::SendDebugMsg(int p_wParam, LPARAM p_lParam, bBool p_sendMessage)
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

int ZSysCom::UnkFunc2(int p_unkInt, unsigned int p_unkUint, HWND p_atom)
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
    char l_buffer[1024]; // [esp+8h] [ebp-400h] BYREF
    va_list l_argList;   // [esp+414h] [ebp+Ch] BYREF

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

void ZSysCom::UnkFunc6(char *p_format, ...)
{
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
