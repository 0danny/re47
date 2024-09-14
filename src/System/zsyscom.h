#pragma once

#include <windows.h>
#include "typedefs.h"

// 0x27 in size
#pragma pack(push, 1)

class ZSysCom;

// Virtual base class for the ZSysCom
class ZSysComBase
{
public:
    ZSysComBase();

    UINT m_msgID;     // 0x4
    int m_unkInt1;    // 0x8
    void *m_unkPtr;   // 0xC
    bBool m_unkBool1; // 0x10

    virtual int FormatString(char *p_resultBuffer, char *p_format, ...) = 0;

    ~ZSysComBase();
    void UnkFunc1();

    int GetUnkInt1();
    UINT GetMsgID();

    virtual HWND ProcessDebugWnd(HWND p_hWnd) = 0;
    virtual void SendDebugMsg(int p_wParam, LPARAM p_lParam, bBool p_sendMessage) = 0;
    virtual int UnkFunc2(int p_unkInt, unsigned int p_unkUint, HWND p_atom) = 0;
    virtual ZSysCom *UnkFunc3(BOOL p_unkBool, int p_unkInt) = 0;
    virtual void ThrowFatal(char *p_format, ...) = 0;
    virtual void DataToDebug(char *p_format, ...) = 0;
    virtual void UnkFunc4(char *p_format, ...) = 0;
    virtual void UnkFunc5(int p_unkInt, char *p_format, ...) = 0;
    virtual void UnkFunc6(char *p_format, ...) = 0;
    virtual void UnkFunc7(char *p_format, ...) = 0;
    virtual void *UnkFunc8(char *p_unkChar) = 0;
    virtual void UnkFunc9() = 0;
};

class ZSysCom : public ZSysComBase
{
public:
    ZSysCom();

    bBool m_unkBool2; // 0x11
    bBool m_unkBool3; // 0x12
    char *m_filePath; // 0x16
    int m_lineNum;    // 0x1A
    HWND m_debugHwnd; // 0x1E
    HWND m_unkHwnd;   // 0x22
    void *m_unkPtr2;  // 0x26

    int FormatString(char *p_resultBuffer, char *p_format, ...);

    ~ZSysCom();

    HWND ProcessDebugWnd(HWND p_hWnd);
    void SendDebugMsg(int p_wParam, LPARAM p_lParam, bBool p_sendMessage);
    int UnkFunc2(int p_unkInt, unsigned int p_unkUint, HWND p_atom);
    ZSysCom *UnkFunc3(BOOL p_unkBool, int p_unkInt);
    void ThrowFatal(char *p_format, ...);
    void DataToDebug(char *p_format, ...);
    void UnkFunc4(char *p_format, ...);
    void UnkFunc5(int p_unkInt, char *p_format, ...);
    void UnkFunc6(char *p_format, ...);
    void UnkFunc7(char *p_format, ...);
    void *UnkFunc8(char *p_unkChar);
    void UnkFunc9();
};

#pragma pack(pop)

extern __declspec(dllimport) ZSysCom *g_pSysCom;