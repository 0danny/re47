#ifndef ZCONSOLECOMMAND_H
#define ZCONSOLECOMMAND_H

#include "system/zsyscom.h"
#include "common.h"

class ZCmdHandler;
class ZHelpCommand;

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
    ZHelpCommand *m_helpCommand;

    ZConsoleCommand();

    virtual ZCmdNode *RegisterCommand(ZCmdHandler *p_handler);
    virtual void UnregisterCommand(ZCmdHandler *p_handler);
    virtual ZCmdNode *FindCommand(char *p_commandName, boolean p_searchForward, boolean p_exactMatch, ZCmdNode *p_startNode);
    virtual boolean ExecuteCommand(char *p_cmd, char *p_cmdValue);
    virtual void PrintStatus(char *p_cmdValue);
    virtual ZCmdNode *GetCmdRoot();

    void Destroy();
};

class ZCmdHandler
{
public:
    inline ZCmdHandler(char *p_cmdName)
    {
        m_cmdName = new char[strlen(p_cmdName) + 1];
        strcpy(m_cmdName, p_cmdName);
    }

    virtual ~ZCmdHandler();
    virtual void UnkFunc0();
    virtual void ExecuteCommand(char *p_cmdValue) = 0;

    char *m_cmdName;
};

class ZHelpCommand : public ZCmdHandler
{
public:
    inline ZHelpCommand(char *p_cmdName, ZConsoleCommand *p_consoleCommand) : ZCmdHandler(p_cmdName)
    {
        m_consoleCommand = p_consoleCommand;
    }

    virtual ~ZHelpCommand();
    void ExecuteCommand(char *p_cmdValue);

    void Destroy();

    ZConsoleCommand *m_consoleCommand;
};

#endif