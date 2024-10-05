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
    ZCmdNode *l_cmdNodeRoot = m_cmdNodeRoot;

    ZCmdNode *l_newNode = new ZCmdNode();

    l_newNode->next = 0;
    l_newNode->prev = 0;
    l_newNode->nextSameCommand = 0;
    l_newNode->prevSameCommand = 0;
    l_newNode->cmdHandler = p_handler;

    int strcmpResult = _strcmpi(p_handler->m_cmdName, l_cmdNodeRoot->cmdHandler->m_cmdName);

    if (strcmpResult >= 0)
    {
        for (ZCmdNode *i = l_cmdNodeRoot->next; l_cmdNodeRoot->next; i = l_cmdNodeRoot->next)
        {
            if (_strcmpi(p_handler->m_cmdName, i->cmdHandler->m_cmdName) <= 0)
                break;

            l_cmdNodeRoot = l_cmdNodeRoot->next;
        }

        if (!l_cmdNodeRoot->next || _strcmpi(p_handler->m_cmdName, l_cmdNodeRoot->next->cmdHandler->m_cmdName))
        {
            l_newNode->next = l_cmdNodeRoot->next;
            l_newNode->prev = l_cmdNodeRoot;

            l_result = l_cmdNodeRoot->next;

            if (l_cmdNodeRoot->next)
                l_result->prev = l_newNode;

            l_cmdNodeRoot->next = l_newNode;
        }
        else
        {
            ZCmdNode *l_rootNext = l_cmdNodeRoot->next;

            l_result = l_rootNext->nextSameCommand;

            if (l_result)
            {
                l_result->prevSameCommand = l_newNode;
                l_newNode->nextSameCommand = l_rootNext->nextSameCommand;
            }

            l_newNode->prevSameCommand = l_rootNext;
            l_rootNext->nextSameCommand = l_newNode;
        }
    }
    else
    {
        l_newNode->next = m_cmdNodeRoot;
        m_cmdNodeRoot->prev = l_newNode;
        m_cmdNodeRoot = l_newNode;
    }

    return l_result;
}

void ZConsoleCommand::UnregisterCommand(ZCmdHandler *p_handler)
{
    ZCmdNode *l_cmdNodeRoot = m_cmdNodeRoot;

    if (l_cmdNodeRoot)
    {
        while (_strcmpi(l_cmdNodeRoot->cmdHandler->m_cmdName, p_handler->m_cmdName))
        {
            l_cmdNodeRoot = l_cmdNodeRoot->next;

            if (!l_cmdNodeRoot)
                return;
        }

        while (l_cmdNodeRoot->cmdHandler != p_handler)
        {
            l_cmdNodeRoot = l_cmdNodeRoot->nextSameCommand;

            if (!l_cmdNodeRoot)
            {
                char *m_cmdName = p_handler->m_cmdName;

                ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 215);
                l_sysCom->UnkFunc4("Command %s not registered in list!", m_cmdName);

                return;
            }
        }

        if (l_cmdNodeRoot == m_cmdNodeRoot)
            m_cmdNodeRoot = l_cmdNodeRoot->next;

        ZCmdNode *rootPrev = l_cmdNodeRoot->prev;

        if (rootPrev)
        {
            ZCmdNode *rootNextSame_2 = l_cmdNodeRoot->nextSameCommand;

            if (rootNextSame_2)
            {
                rootNextSame_2->prev = rootPrev;
                l_cmdNodeRoot->prev->next = l_cmdNodeRoot->nextSameCommand;
            }
            else
            {
                rootPrev->next = l_cmdNodeRoot->next;
            }
        }

        ZCmdNode *rootNext = l_cmdNodeRoot->next;

        if (l_cmdNodeRoot->next)
        {
            ZCmdNode *rootNextSame = l_cmdNodeRoot->nextSameCommand;
            if (rootNextSame)
            {
                rootNextSame->next = rootNext;
                l_cmdNodeRoot->next->prev = l_cmdNodeRoot->nextSameCommand;
            }
            else
            {
                rootNext->prev = l_cmdNodeRoot->prev;
            }
        }

        ZCmdNode *rootPrevSame = l_cmdNodeRoot->prevSameCommand;

        if (rootPrevSame)
            rootPrevSame->nextSameCommand = l_cmdNodeRoot->nextSameCommand;

        ZCmdNode *rootNextSame_1 = l_cmdNodeRoot->nextSameCommand;

        if (rootNextSame_1)
            rootNextSame_1->prevSameCommand = l_cmdNodeRoot->prevSameCommand;

        delete l_cmdNodeRoot;
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

bBool ZConsoleCommand::PrintCommandStatus(char *p_cmd, char *p_cmdValue)
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
    ZCmdNode *l_nextSame;
    ZCmdNode *l_rootNode;
    ZCmdHandler *l_cmdHandler = this->m_cmdHandler;

    char *l_cmdName;

    UnregisterCommand(l_cmdHandler);

    if (l_cmdHandler)
        l_cmdHandler->~ZCmdHandler();

    for (ZCmdNode *l_curNode = this->m_cmdNodeRoot; l_curNode; l_curNode = this->m_cmdNodeRoot)
    {
        l_nextSame = l_curNode->nextSameCommand;

        for (ZCmdNode **l_curSameNode = &l_curNode->nextSameCommand; l_nextSame; l_curSameNode = &l_rootNode->nextSameCommand)
        {
            *l_curSameNode = l_nextSame->nextSameCommand;

            l_cmdName = l_nextSame->cmdHandler->m_cmdName;

            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 94);
            l_sysCom->UnkFunc4("Removing unremoved command '%s'", l_cmdName);

            delete l_nextSame;

            l_rootNode = this->m_cmdNodeRoot;
            l_nextSame = l_rootNode->nextSameCommand;
        }

        ZCmdNode *l_rootNode2 = this->m_cmdNodeRoot;
        this->m_cmdNodeRoot = l_rootNode2->next;

        char *l_cmdName2 = l_rootNode2->cmdHandler->m_cmdName;

        ZSysCom *l_sysCom2 = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 99);
        l_sysCom2->UnkFunc4("Removing unremoved command '%s'", l_cmdName2);

        delete l_rootNode2;
    }
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
