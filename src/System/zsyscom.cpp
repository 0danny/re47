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
}

int ZSysCom::FormatString(char *p_resultBuffer, char *p_format, ...)
{
    return 0;
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
}

int ZSysCom::UnkFunc2(int p_unkInt, unsigned int p_unkUint, HWND p_atom)
{
    return 0;
}

ZSysCom *ZSysCom::UnkFunc3(BOOL p_unkBool, int p_unkInt)
{
    return this;
}

void ZSysCom::ThrowFatal(char *p_format, ...)
{
}

void ZSysCom::DataToDebug(char *p_format, ...)
{
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
