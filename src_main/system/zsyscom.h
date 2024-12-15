#ifndef ZSYSCOM_H
#define ZSYSCOM_H

#include "common.h"

class LinkRefTab;
class StrRefTab;

// 0x27 in size
#pragma pack(push, 1)

class ZSysCom
{
public:
    ZSysCom();

    // Base class members
    u32 m_msgID;              // 0x4
    i32 m_unkInt1;            // 0x8
    LinkRefTab *m_linkRefTab; // 0xC
    bool m_unkBool1;          // 0x10

    bool m_sendingZMessage; // 0x11
    bool m_debugWndReady;   // 0x12
    char *m_filePath;       // 0x16
    i32 m_lineNum;          // 0x1A
    HWND m_debugHwnd;       // 0x1E
    HWND m_mainHwnd;        // 0x22
    StrRefTab *m_logsArray; // 0x26

    // Overrides
    virtual i32 FormatString(char *p_resultBuffer, char *p_format, ...);

    // Destroy
    virtual ~ZSysCom();
    virtual void DestroyArrays();
    void Destroy();

    // Getters
    virtual i32 GetUnkInt1();
    virtual u32 GetMsgID();

    // Debug window stuff
    virtual void ProcessDebugWnd(HWND p_hWnd);
    virtual void SendDebugMsg(WPARAM p_wParam, LPARAM p_lParam, bool p_sendMessage);
    virtual i32 ForwardWndProc(u32 p_msg, WPARAM p_wParam, LPARAM p_lParam);

    // Logging
    virtual ZSysCom *SetPathAndLine(char *p_filePath, i32 p_lineNum);
    virtual void ThrowFatal(char *p_format, ...);
    virtual void DataToDebug(char *p_format, ...);
    virtual void LogMessage(char *p_format, ...);

    virtual void UnkFunc5(i32 p_unused, char *p_format, ...);
    virtual void SendZMessage(char *p_format, ...);
    virtual void AddCmdText(char *p_format, ...);
    virtual void UnkFunc8();
    virtual void UnkFunc9();
};

#pragma pack(pop)

extern __declspec(dllimport) ZSysCom *g_pSysCom;

#endif