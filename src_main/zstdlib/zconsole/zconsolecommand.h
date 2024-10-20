#ifndef ZCONSOLECOMMAND_H
#define ZCONSOLECOMMAND_H

#include "system/zsyscom.h"
#include "common.h"

class ZCmdHandler;
class ZCmdHandlerBase;

struct ZCmdNode
{
    // Command linked list
    ZCmdNode *next;
    ZCmdNode *prev;

    // Same command linked list
    ZCmdNode *prevSameCommand;
    ZCmdNode *nextSameCommand;

    ZCmdHandler *cmdHandler;
};

class ZConsoleCommand
{
public:
    ZCmdNode *m_cmdNodeRoot;
    ZCmdHandler *m_cmdHandler;

    ZConsoleCommand();

    virtual ZCmdNode *RegisterCommand(ZCmdHandler *p_handler);
    virtual void UnregisterCommand(ZCmdHandler *p_handler);
    virtual ZCmdNode *FindCommand(char *p_commandName, boolean p_searchForward, boolean p_exactMatch, ZCmdNode *p_startNode);
    virtual boolean ExecuteCommand(char *p_cmd, char *p_cmdValue);
    virtual void PrintStatus(char *p_cmdValue);
    virtual ZCmdNode *GetCmdRoot();

    void Destroy();
};

class ZCmdHandlerBase
{
public:
    ZCmdHandlerBase();

    virtual ~ZCmdHandlerBase();
    virtual void UnkFunc0();
    virtual void PrintStatus(char *p_cmdValue) = 0;

    char *m_cmdName;
};

class ZCmdHandler : public ZCmdHandlerBase
{
public:
    ZCmdHandler(ZConsoleCommand *p_consoleCmd);

    virtual ~ZCmdHandler();
    void PrintStatus(char *p_cmdValue);

    void Destroy();

    ZConsoleCommand *m_consoleCmd;
};

#endif