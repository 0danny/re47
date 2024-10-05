#pragma once

#include "common.h"
#include "zconsolecommand.h"

class ZConsoleUnk;

struct ZConsoleStruct
{
    ZConsoleStruct *m_consoleStruct1;
    ZConsoleStruct *unkInt1;
    int unkInt2;
    int unkInt3;
};

#pragma pack(push, 1)

class ZConsole
{
public:
    uint8_t unkByte1;
    uint8_t unkByte2;
    uint8_t unkByte3;
    float unkFloat1;
    float unkFloat2;
    char *m_outputBox[1000];
    int m_prevCmdIndex;
    int unkInt3;
    char *m_prevCmds[20];
    char m_textBox[200];
    int m_txtBoxLen;
    int m_cmdCount;
    int m_cmdCount2;
    ZConsoleCommand m_consoleCmd;
    int unkInt7;
    uint8_t unkByte5;
    ZConsoleStruct *m_consoleStruct;
    ZConsoleUnk *m_zConsoleUnk;

    ZConsole();

    virtual bBool UnkFunc0();
    virtual void AddCmdText(const char *p_format, ...);
    virtual char *UnkFunc2(int p_unkInt);
    virtual uint8_t UnkFunc3();
    virtual void UnkFunc4(int p_identifier, char *p_cmdName);
    virtual void UnkFunc5(int p_unkInt1, int p_unkInt2);
    virtual void UnkFunc6();
    virtual ZCmdNode *RegisterCommand(ZCmdHandler *p_handler);
    virtual void UnregisterCommand(ZCmdHandler *p_handler);
    virtual void UnkFunc9(const char *p_str);
    virtual void UnkFunc10();
    virtual void GoPrevCmd(bBool p_forwards);
    virtual void UnkFunc12(const char *p_str);
    virtual double UnkFunc13();
};

#pragma pack(pop)

class ZConsoleUnk
{
public:
    ZConsoleStruct *m_consoleStruct;
    int unkInt2;
    int unkInt3;

    ZConsoleUnk(ZConsoleStruct *p_consoleStruct);
    virtual ~ZConsoleUnk();
};