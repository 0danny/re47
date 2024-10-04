#pragma once

#include "system/zsyscom.h"

// TODO: Clean this whole class up

class ZCmdClass;
class ZCmdClassBase;

struct ZCmdStruct
{
    ZCmdStruct *cmdStruct1;
    ZCmdStruct *cmdStruct2;
    ZCmdStruct *cmdStruct3;
    ZCmdStruct *cmdStruct4;
    ZCmdClass *cmdClass;
};

class ZConsoleCommand
{
public:
    ZCmdStruct *m_cmdStruct;
    ZCmdClass *m_cmdClass;

    ZConsoleCommand();

    virtual ZCmdStruct *UnkFunc1(ZCmdClass *p_cmdClass);
    virtual void UnkFunc2(ZCmdClass *p_cmdClass);
    virtual ZCmdStruct *UnkFunc3(char *p_str, bool p_flag, bool p_flag2, ZCmdStruct *p_cmdStruct);
    virtual bBool UnkFunc4(char *p_cmd, char *p_cmdValue);
    virtual void PrintStatus(char *p_cmdValue);
    virtual ZCmdStruct *GetCmdStruct();

    void Destroy();
};

class ZCmdClassBase
{
public:
    ZCmdClassBase();

    virtual ~ZCmdClassBase();
    virtual void UnkFunc0();
    virtual void UnkFunc1(char *p_cmdValue) = 0;

    char *m_commandsStr;
};

class ZCmdClass : public ZCmdClassBase
{
public:
    ZCmdClass(ZConsoleCommand *p_consoleCmd);

    virtual ~ZCmdClass();
    void UnkFunc1(char *p_cmdValue);

    void Destroy();

    ZConsoleCommand *m_consoleCmd;
};
