#include "zconsolecommand.h"

/* ------------ ZConsoleCommand ----------------*/

ZConsoleCommand::ZConsoleCommand()
{
    m_cmdHandler = new ZCmdHandler(this);
    m_cmdNodeRoot = new ZCmdNode();

    m_cmdNodeRoot->next = 0;
    m_cmdNodeRoot->prev = 0;
    m_cmdNodeRoot->prevSameCommand = 0;
    m_cmdNodeRoot->nextSameCommand = 0;
    m_cmdNodeRoot->cmdHandler = m_cmdHandler;
}

ZCmdNode *ZConsoleCommand::RegisterCommand(ZCmdHandler *p_handler)
{
    ZCmdNode *l_result;

    ZCmdNode *l_cmdStruct = new ZCmdNode();

    l_cmdStruct->next = 0;
    l_cmdStruct->prev = 0;
    l_cmdStruct->nextSameCommand = 0;
    l_cmdStruct->prevSameCommand = 0;
    l_cmdStruct->cmdHandler = p_handler;

    int strcmpResult = _strcmpi(p_handler->m_cmdName, m_cmdNodeRoot->cmdHandler->m_cmdName);

    if (strcmpResult >= 0)
    {
        for (ZCmdNode *i = m_cmdNodeRoot->next; m_cmdNodeRoot->next; i = m_cmdNodeRoot->next)
        {
            if (_strcmpi(p_handler->m_cmdName, i->cmdHandler->m_cmdName) <= 0)
                break;

            m_cmdNodeRoot = m_cmdNodeRoot->next;
        }

        if (!m_cmdNodeRoot->next || _strcmpi(p_handler->m_cmdName, m_cmdNodeRoot->next->cmdHandler->m_cmdName))
        {
            l_cmdStruct->next = m_cmdNodeRoot->next;
            l_cmdStruct->prev = m_cmdNodeRoot;

            l_result = m_cmdNodeRoot->next;

            if (m_cmdNodeRoot->next)
                l_result->prev = l_cmdStruct;

            m_cmdNodeRoot->next = l_cmdStruct;
        }
        else
        {
            ZCmdNode *l_cmdStruct3 = m_cmdNodeRoot->next;
            l_result = l_cmdStruct3->nextSameCommand;

            if (l_result)
            {
                l_result->prevSameCommand = l_cmdStruct;
                l_cmdStruct->nextSameCommand = l_cmdStruct3->nextSameCommand;
            }

            l_cmdStruct->prevSameCommand = l_cmdStruct3;
            l_cmdStruct3->nextSameCommand = l_cmdStruct;
        }
    }
    else
    {
        l_cmdStruct->next = m_cmdNodeRoot;
        m_cmdNodeRoot->prev = l_cmdStruct;
        m_cmdNodeRoot = l_cmdStruct;
    }

    return l_result;
}

void ZConsoleCommand::UnregisterCommand(ZCmdHandler *p_handler)
{
    if (m_cmdNodeRoot)
    {
        while (_strcmpi(m_cmdNodeRoot->cmdHandler->m_cmdName, p_handler->m_cmdName))
        {
            m_cmdNodeRoot = m_cmdNodeRoot->next;

            if (!m_cmdNodeRoot)
                return;
        }

        while (m_cmdNodeRoot->cmdHandler != p_handler)
        {
            m_cmdNodeRoot = m_cmdNodeRoot->nextSameCommand;

            if (!m_cmdNodeRoot)
            {
                ZSysCom *l_com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 215);
                l_com->UnkFunc4("Command %s not registered in list!", p_handler->m_cmdName);

                return;
            }
        }

        if (m_cmdNodeRoot == m_cmdNodeRoot)
            m_cmdNodeRoot = m_cmdNodeRoot->next;

        ZCmdNode *cmdStruct2 = m_cmdNodeRoot->prev;

        if (cmdStruct2)
        {
            ZCmdNode *cmdStruct4 = m_cmdNodeRoot->nextSameCommand;

            if (cmdStruct4)
            {
                cmdStruct4->prev = cmdStruct2;
                m_cmdNodeRoot->prev->next = m_cmdNodeRoot->nextSameCommand;
            }
            else
            {
                cmdStruct2->next = m_cmdNodeRoot->next;
            }
        }

        ZCmdNode *cmdStruct1 = m_cmdNodeRoot->next;

        if (m_cmdNodeRoot->next)
        {
            ZCmdNode *cmdStruct4 = m_cmdNodeRoot->nextSameCommand;

            if (cmdStruct4)
            {
                cmdStruct4->next = cmdStruct1;
                m_cmdNodeRoot->next->prev = m_cmdNodeRoot->nextSameCommand;
            }
            else
            {
                cmdStruct1->prev = m_cmdNodeRoot->prev;
            }
        }

        ZCmdNode *cmdStruct3 = m_cmdNodeRoot->prevSameCommand;

        if (cmdStruct3)
            cmdStruct3->nextSameCommand = m_cmdNodeRoot->nextSameCommand;

        ZCmdNode *cmdStruct5 = m_cmdNodeRoot->nextSameCommand;

        if (cmdStruct5)
            cmdStruct5->prevSameCommand = m_cmdNodeRoot->prevSameCommand;

        delete m_cmdNodeRoot;
    }
}

ZCmdNode *ZConsoleCommand::FindCommand(char *p_commandName, bool p_searchForward, bool p_exactMatch, ZCmdNode *p_startNode)
{
    ZCmdNode *l_cmdStruct;

    if (p_startNode)
    {
        if (p_searchForward)
            l_cmdStruct = p_startNode->next;
        else
            l_cmdStruct = p_startNode->prev;
    }
    else
    {
        l_cmdStruct = m_cmdNodeRoot;
    }

    UINT l_cmdLength = strlen(p_commandName) + 1;

    if (!l_cmdStruct)
        return p_startNode;

    while (p_exactMatch)
    {
        if (!_strcmpi(p_commandName, l_cmdStruct->cmdHandler->m_cmdName))
            return l_cmdStruct;

    IN_LOOP:
        if (p_searchForward)
            l_cmdStruct = l_cmdStruct->next;
        else
            l_cmdStruct = l_cmdStruct->prev;

        if (!l_cmdStruct)
            return p_startNode;
    }

    if (_strnicmp(p_commandName, l_cmdStruct->cmdHandler->m_cmdName, l_cmdLength - 1))
        goto IN_LOOP; // thats interesting lol

    return l_cmdStruct;
}

bBool ZConsoleCommand::ExecuteCommand(char *p_cmd, char *p_cmdValue)
{
    ZCmdNode *l_cmdStruct = FindCommand(p_cmd, 1, 1, 0);

    if (!l_cmdStruct)
        return 0;

    do
    {
        l_cmdStruct->cmdHandler->PrintStatus(p_cmdValue);

        l_cmdStruct = l_cmdStruct->nextSameCommand;

    } while (l_cmdStruct);

    return 1;
}

void ZConsoleCommand::PrintStatus(char *p_cmdValue)
{
    for (ZCmdNode *i = m_cmdNodeRoot; i; i = i->next)
    {
        char *l_commandsStr = i->cmdHandler->m_cmdName;

        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 284);
        l_sysCom->UnkFunc4("%s", l_commandsStr);
    }
}

ZCmdNode *ZConsoleCommand::GetCmdRoot()
{
    return m_cmdNodeRoot;
}

void ZConsoleCommand::Destroy()
{
    // TODO: Implement (0x0FFC8B20)
}

/* ------------ ZCmdHandler ----------------*/

ZCmdHandler::ZCmdHandler(ZConsoleCommand *p_consoleCmd) : ZCmdHandlerBase()
{
    m_consoleCmd = p_consoleCmd;
}

ZCmdHandler::~ZCmdHandler()
{
    Destroy();
}

void ZCmdHandler::PrintStatus(char *p_cmdValue)
{
    m_consoleCmd->PrintStatus(p_cmdValue);
}

void ZCmdHandler::Destroy()
{
    ZCmdHandlerBase::~ZCmdHandlerBase();
}

/* ------------ ZCmdHandlerBase ------------ */

ZCmdHandlerBase::ZCmdHandlerBase()
{
    m_cmdName = new char[strlen("commands") + 1];
    strcpy(m_cmdName, "commands");
}

ZCmdHandlerBase::~ZCmdHandlerBase()
{
    delete[] m_cmdName;
}

// NULLSUB
void ZCmdHandlerBase::UnkFunc0()
{
    return;
}
