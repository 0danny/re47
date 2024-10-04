#include "zconsolecommand.h"

// Crude implementation to get it to work.

/* ------------ ZConsoleCommand ----------------*/

ZConsoleCommand::ZConsoleCommand()
{
    m_cmdClass = new ZCmdClass(this);
    m_cmdStruct = new ZCmdStruct();

    m_cmdStruct->cmdStruct1 = 0;
    m_cmdStruct->cmdStruct2 = 0;
    m_cmdStruct->cmdStruct3 = 0;
    m_cmdStruct->cmdStruct4 = 0;
    m_cmdStruct->cmdClass = m_cmdClass;
}

ZCmdStruct *ZConsoleCommand::UnkFunc1(ZCmdClass *p_cmdClass)
{
    ZCmdStruct *l_cmdStruct2; // edi
    ZCmdStruct *l_malloc;     // eax
    ZCmdStruct *l_cmdStruct;  // esi
    ZCmdStruct *l_result;     // eax
    ZCmdStruct *i;            // eax
    ZCmdStruct *l_cmdStruct3; // edi

    l_cmdStruct2 = this->m_cmdStruct;

    l_malloc = new ZCmdStruct();

    l_cmdStruct = l_malloc;
    l_malloc->cmdStruct1 = 0;
    l_malloc->cmdStruct2 = 0;
    l_malloc->cmdStruct4 = 0;
    l_malloc->cmdStruct3 = 0;
    l_malloc->cmdClass = p_cmdClass;

    int strcmpResult = _strcmpi(p_cmdClass->m_commandsStr, l_cmdStruct2->cmdClass->m_commandsStr);

    if (strcmpResult >= 0)
    {
        for (i = l_cmdStruct2->cmdStruct1; l_cmdStruct2->cmdStruct1; i = l_cmdStruct2->cmdStruct1)
        {
            if (_strcmpi(p_cmdClass->m_commandsStr, i->cmdClass->m_commandsStr) <= 0)
                break;

            l_cmdStruct2 = l_cmdStruct2->cmdStruct1;
        }

        if (!l_cmdStruct2->cmdStruct1 || _strcmpi(p_cmdClass->m_commandsStr, l_cmdStruct2->cmdStruct1->cmdClass->m_commandsStr))
        {
            l_cmdStruct->cmdStruct1 = l_cmdStruct2->cmdStruct1;
            l_cmdStruct->cmdStruct2 = l_cmdStruct2;

            l_result = l_cmdStruct2->cmdStruct1;
            if (l_cmdStruct2->cmdStruct1)
                l_result->cmdStruct2 = l_cmdStruct;

            l_cmdStruct2->cmdStruct1 = l_cmdStruct;
        }
        else
        {
            l_cmdStruct3 = l_cmdStruct2->cmdStruct1;
            l_result = l_cmdStruct3->cmdStruct4;

            if (l_result)
            {
                l_result->cmdStruct3 = l_cmdStruct;
                l_cmdStruct->cmdStruct4 = l_cmdStruct3->cmdStruct4;
            }

            l_cmdStruct->cmdStruct3 = l_cmdStruct3;
            l_cmdStruct3->cmdStruct4 = l_cmdStruct;
        }
    }
    else
    {
        l_cmdStruct->cmdStruct1 = this->m_cmdStruct;
        this->m_cmdStruct->cmdStruct2 = l_cmdStruct;
        this->m_cmdStruct = l_cmdStruct;
    }

    return l_result;
}

void ZConsoleCommand::UnkFunc2(ZCmdClass *p_cmdClass)
{
    ZCmdStruct *m_cmdStruct; // esi
    char *m_commandsStr;     // edi
    ZCmdStruct *cmdStruct2;  // eax
    ZCmdStruct *cmdStruct4;  // ecx
    ZCmdStruct *cmdStruct1;  // eax
    ZCmdStruct *v9;          // ecx
    ZCmdStruct *cmdStruct3;  // eax
    ZCmdStruct *v11;         // eax

    m_cmdStruct = this->m_cmdStruct;

    if (m_cmdStruct)
    {
        while (_strcmpi(m_cmdStruct->cmdClass->m_commandsStr, p_cmdClass->m_commandsStr))
        {
            m_cmdStruct = m_cmdStruct->cmdStruct1;

            if (!m_cmdStruct)
                return;
        }

        while (m_cmdStruct->cmdClass != p_cmdClass)
        {
            m_cmdStruct = m_cmdStruct->cmdStruct4;

            if (!m_cmdStruct)
            {
                m_commandsStr = p_cmdClass->m_commandsStr;

                ZSysCom *v5 = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 215);
                v5->UnkFunc4("Command %s not registered in list!", m_commandsStr);
                return;
            }
        }

        if (m_cmdStruct == this->m_cmdStruct)
            this->m_cmdStruct = m_cmdStruct->cmdStruct1;

        cmdStruct2 = m_cmdStruct->cmdStruct2;
        if (cmdStruct2)
        {

            cmdStruct4 = m_cmdStruct->cmdStruct4;

            if (cmdStruct4)
            {
                cmdStruct4->cmdStruct2 = cmdStruct2;
                m_cmdStruct->cmdStruct2->cmdStruct1 = m_cmdStruct->cmdStruct4;
            }
            else
            {
                cmdStruct2->cmdStruct1 = m_cmdStruct->cmdStruct1;
            }
        }

        cmdStruct1 = m_cmdStruct->cmdStruct1;
        if (m_cmdStruct->cmdStruct1)
        {
            v9 = m_cmdStruct->cmdStruct4;
            if (v9)
            {
                v9->cmdStruct1 = cmdStruct1;
                m_cmdStruct->cmdStruct1->cmdStruct2 = m_cmdStruct->cmdStruct4;
            }
            else
            {
                cmdStruct1->cmdStruct2 = m_cmdStruct->cmdStruct2;
            }
        }

        cmdStruct3 = m_cmdStruct->cmdStruct3;

        if (cmdStruct3)
            cmdStruct3->cmdStruct4 = m_cmdStruct->cmdStruct4;

        v11 = m_cmdStruct->cmdStruct4;

        if (v11)
            v11->cmdStruct3 = m_cmdStruct->cmdStruct3;

        delete m_cmdStruct;
    }
}

ZCmdStruct *ZConsoleCommand::UnkFunc3(char *p_str, bool p_flag, bool p_flag2, ZCmdStruct *p_cmdStruct)
{
    ZCmdStruct *l_cmdStruct; // esi
    unsigned int l_strLen;   // kr04_4

    if (p_cmdStruct)
    {
        if (p_flag)
            l_cmdStruct = p_cmdStruct->cmdStruct1;
        else
            l_cmdStruct = p_cmdStruct->cmdStruct2;
    }
    else
    {
        l_cmdStruct = this->m_cmdStruct;
    }

    l_strLen = strlen(p_str) + 1;

    if (!l_cmdStruct)
        return p_cmdStruct;

    while (p_flag2)
    {
        if (!_strcmpi(p_str, l_cmdStruct->cmdClass->m_commandsStr))
            return l_cmdStruct;
    LABEL_11:

        if (p_flag)
            l_cmdStruct = l_cmdStruct->cmdStruct1;
        else
            l_cmdStruct = l_cmdStruct->cmdStruct2;

        if (!l_cmdStruct)
            return p_cmdStruct;
    }

    if (_strnicmp(p_str, l_cmdStruct->cmdClass->m_commandsStr, l_strLen - 1))
        goto LABEL_11;

    return l_cmdStruct;
}

bBool ZConsoleCommand::UnkFunc4(char *p_cmd, char *p_cmdValue)
{
    ZCmdStruct *v3; // esi

    v3 = this->UnkFunc3(p_cmd, 1, 1, 0);

    if (!v3)
        return 0;

    do
    {
        v3->cmdClass->UnkFunc1(p_cmdValue);
        v3 = v3->cmdStruct4;
    } while (v3);

    return 1;
}

void ZConsoleCommand::PrintStatus(char *p_cmdValue)
{
    for (ZCmdStruct *i = m_cmdStruct; i; i = i->cmdStruct1)
    {
        char *l_commandsStr = i->cmdClass->m_commandsStr;

        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\ConsoleCommand.cpp", 284);
        l_sysCom->UnkFunc4("%s", l_commandsStr);
    }
}

ZCmdStruct *ZConsoleCommand::GetCmdStruct()
{
    return m_cmdStruct;
}

void ZConsoleCommand::Destroy()
{
    // TODO: Implement (0x0FFC8B20)
}

/* ------------ ZCmdClass ----------------*/

ZCmdClass::ZCmdClass(ZConsoleCommand *p_consoleCmd) : ZCmdClassBase()
{
    m_consoleCmd = p_consoleCmd;
}

ZCmdClass::~ZCmdClass()
{
    Destroy();
}

void ZCmdClass::UnkFunc1(char *p_cmdValue)
{
    m_consoleCmd->PrintStatus(p_cmdValue);
}

void ZCmdClass::Destroy()
{
    ZCmdClassBase::~ZCmdClassBase();
}

/* ------------ ZCmdClassBase ------------ */

ZCmdClassBase::ZCmdClassBase()
{
    m_commandsStr = new char[strlen("commands") + 1];
    strcpy(m_commandsStr, "commands");
}

ZCmdClassBase::~ZCmdClassBase()
{
    delete[] m_commandsStr;
}

// NULLSUB
void ZCmdClassBase::UnkFunc0()
{
    return;
}
