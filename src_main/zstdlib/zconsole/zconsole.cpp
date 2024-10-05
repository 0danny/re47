#include "zconsole.h"

// TODO: This implementation is incomplete
ZConsole::ZConsole()
{
    m_consoleCmd = *new ZConsoleCommand();

    unkFloat1 = 0.0;
    unkFloat2 = 0.2;
    unkByte1 = 0;
    unkByte3 = 0;

    strcpy(m_textBox, ">");

    m_txtBoxLen = 1;
    unkInt7 = 0;
    unkByte5 = 0;
    m_prevCmdIndex = 0;
    unkInt3 = 0;
    unkByte2 = 0;

    for (int i = 0; i < 1000; i++)
    {
        m_outputBox[i] = new char[150];
    }

    memset(m_prevCmds, 0, sizeof(m_prevCmds));

    m_cmdCount2 = 0;
    m_cmdCount = 0;

    ZConsoleStruct *l_consoleStruct = new ZConsoleStruct();

    if (l_consoleStruct)
    {
        l_consoleStruct->m_consoleStruct1 = 0;
        l_consoleStruct->unkInt1 = 0;
        l_consoleStruct->unkInt2 = 32;
        l_consoleStruct->unkInt3 = 0;
    }

    m_consoleStruct = l_consoleStruct;

    m_zConsoleUnk = new ZConsoleUnk(m_consoleStruct);
}

bBool ZConsole::UnkFunc0()
{
    if (unkByte1)
    {
        double l_unkFloat2 = unkFloat2;

        unkByte3 = 1;

        double l_add = l_unkFloat2 + unkFloat1;

        unkFloat1 = l_add;

        if (l_add >= 1.0)
        {
            unkFloat1 = 1.0;
            return 1;
        }
    }
    else
    {
        double l_sub = unkFloat1 - unkFloat2;
        unkFloat1 = l_sub;

        if (l_sub <= 0.0)
        {
            unkFloat1 = 0.0;
            unkByte3 = 0;
            return 1;
        }
    }

    return 0;
}

void ZConsole::AddCmdText(const char *p_format, ...)
{
    char l_buffer[1025];
    va_list l_argList;

    va_start(l_argList, p_format);
    vsprintf(&l_buffer[1], p_format, l_argList);

    char *l_bracketOccurence = strchr(&l_buffer[1], ')');

    if (!l_bracketOccurence)
        l_bracketOccurence = l_buffer;

    strncpy(m_outputBox[m_prevCmdIndex], l_bracketOccurence + 1, 149u);

    m_outputBox[m_prevCmdIndex][149] = '\0';

    int l_newIndex = m_prevCmdIndex + 1;

    m_prevCmdIndex = l_newIndex;

    if (l_newIndex == 1000)
        m_prevCmdIndex = 0;
}

char *ZConsole::UnkFunc2(int p_unkInt)
{
    if (!p_unkInt)
        return m_textBox;

    int l_unkInt3 = unkInt3;

    if (l_unkInt3 + p_unkInt > 1000)
        return g_emptyArray;

    if (l_unkInt3 + p_unkInt < -1000)
        return g_emptyArray;

    int l_index = p_unkInt + l_unkInt3 + m_prevCmdIndex;

    if (l_index < 0)
        l_index += 1000 * ((999 - l_index) / 1000u);

    if (l_index > 999)
        l_index %= 1000u;

    return m_outputBox[l_index];
}

uint8_t ZConsole::UnkFunc3()
{
    return unkByte3;
}

void ZConsole::UnkFunc4(int p_identifier, char *p_cmdName)
{
    uint16_t l_cmdName;               // eax
    int l_identifier;                 // ebx
    int v6;                           // ecx
    int v7;                           // eax
    int v8;                           // ecx
    int m_txtBoxLen;                  // eax
    int v10;                          // eax
    int v11;                          // eax
    int v12;                          // eax
    ZConsoleStruct *l_zConsoleStruct; // esi
    ZConsoleStruct *i;                // edi
    ZCmdNode *j;                      // ebx
    unsigned int unkInt3;             // ecx
    int v17;                          // edx
    ZConsoleStruct *v18;              // eax
    ZConsoleStruct *v19;              // edi
    ZConsoleStruct *v20;              // eax
    ZConsoleStruct *v21;              // edi
    ZConsoleStruct *v22;              // eax
    const char *v23;                  // eax
    ZConsoleUnk *m_zConsoleUnk;       // ebx
    ZConsoleStruct *m_consoleStruct;  // edx
    unsigned int v26;                 // ecx
    ZConsoleStruct *m_consoleStruct1; // eax
    unsigned int unkInt2;             // edx
    const char *v29;                  // esi
    ZConsoleUnk *v30;                 // esi
    int v31;                          // eax
    unsigned int v32;                 // ecx
    ZConsoleStruct *v33;              // edx
    ZConsoleStruct *v34;              // eax
    unsigned int v35;                 // edx
    const char *v36;                  // ebx
    int v37;                          // ecx
    int v38;                          // eax
    int v39;                          // esi
    int v40;                          // eax
    int p_unkInt1a;                   // [esp+14h] [ebp+4h]

    l_cmdName = (uint16_t)(p_cmdName);

    if (!p_cmdName)
        l_cmdName = 1;

    l_identifier = p_identifier;

    if (p_identifier != 9 && p_identifier != 16)
    {
        unkByte5 = 0;
        unkInt7 = 0;
    }

    switch (p_identifier)
    {
    case VK_BACK:
    case VK_DELETE:

        v6 = l_cmdName;
        v7 = l_cmdName - 1;

        if (v6)
        {
            v8 = v7 + 1;

            do
            {
                m_txtBoxLen = m_txtBoxLen;

                if (m_txtBoxLen > 1)
                {
                    v10 = m_txtBoxLen - 1;
                    m_txtBoxLen = v10;
                    m_textBox[v10] = '\0';
                }

                --v8;
            } while (v8);
        }
        return;
    case VK_TAB:
        // TODO: Implement logic.
        ++v30->unkInt3;
        unkByte5 = 1;
        return;
    case VK_RETURN:
        UnkFunc6();
        return;
    case VK_SHIFT:
        unkByte2 = 1;
        return;
    case VK_ESCAPE:
        m_txtBoxLen = 1;
        m_textBox[1] = '\0';
        return;
    case VK_PRIOR:
        v12 = unkInt3;

        if (v12 > -1000)
            unkInt3 = v12 - 1;

        return;
    case VK_NEXT:
        v11 = unkInt3;

        if (v11)
            unkInt3 = v11 + 1;

        return;
    case VK_UP:
        GoPrevCmd(0);
        return;
    case VK_DOWN:
        GoPrevCmd(1);
        return;
    default:
        v37 = l_cmdName;
        v38 = l_cmdName - 1;

        if (v37)
        {
            v39 = v38 + 1;

            do
            {
                switch (l_identifier)
                {
                case 189:
                    l_identifier = '_';
                    break;
                case 190:
                    l_identifier = '.';
                    break;
                case 191:
                    l_identifier = '*';
                    break;
                }

                if (isalnum(l_identifier) || l_identifier == '_' || l_identifier == ' ' || l_identifier == '.' || l_identifier == '*')
                {
                    m_textBox[m_txtBoxLen] = l_identifier;
                    v40 = m_txtBoxLen + 1;
                    m_txtBoxLen = v40;
                    m_textBox[v40] = 0;
                }

                --v39;

            } while (v39);
        }
        return;
    }
}

void ZConsole::UnkFunc5(int p_unkInt1, int p_unkInt2)
{
    if (p_unkInt1 == 16)
        unkByte2 = 0;
}

void ZConsole::UnkFunc6()
{
    char *l_txtBox = &m_textBox[1];

    if (m_textBox[1])
        UnkFunc12(&m_textBox[1]);

    unkInt3 = 0;

    char *l_subStr = strtok(l_txtBox, " ");

    char *l_cmd = l_subStr;

    if (l_subStr && *l_subStr)
    {
        char *l_cmdValue = strtok(0, 0); // Uses the same string as the previous strtok call

        if (m_consoleCmd.FindCommandAlt(l_cmd, l_cmdValue))
        {
            AddCmdText("%s %s", l_cmd, l_cmdValue);
            m_txtBoxLen = 1;
            *l_txtBox = 0;
        }
        else
        {
            AddCmdText("%s - Unknown command - use <commands> to display list", l_cmd);
            *l_txtBox = 0;
            m_txtBoxLen = 1;
        }
    }
}

ZCmdNode *ZConsole::RegisterCommand(ZCmdHandler *p_handler)
{
    return m_consoleCmd.RegisterCommand(p_handler);
}

void ZConsole::UnregisterCommand(ZCmdHandler *p_handler)
{
    m_consoleCmd.UnregisterCommand(p_handler);
}

void ZConsole::UnkFunc9(const char *p_str)
{
    sprintf(m_textBox, ">%s", p_str);

    UnkFunc6();
}

void ZConsole::UnkFunc10()
{
    unkByte1 = unkByte1 == 0;
}

void ZConsole::GoPrevCmd(bBool p_forwards)
{
    if (m_prevCmds[0])
    {
        if (p_forwards)
        {
            do
            {
                int l_cmdCount = m_cmdCount2 + 1;
                m_cmdCount2 = l_cmdCount;

                if (l_cmdCount == 20)
                    m_cmdCount2 = 0;

            } while (!m_prevCmds[m_cmdCount2]);
        }
        else
        {
            do
            {
                if (m_cmdCount2-- == 0)
                    m_cmdCount2 = 19;

            } while (!m_prevCmds[m_cmdCount2]);
        }

        char *l_newCmd = m_prevCmds[m_cmdCount2];

        if (l_newCmd)
        {
            strcpy(&m_textBox[1], l_newCmd);
            m_txtBoxLen = strlen(m_textBox);
        }
    }
}

void ZConsole::UnkFunc12(const char *p_str)
{
    char *l_prevCmd = m_prevCmds[m_cmdCount];

    if (l_prevCmd)
    {
        delete[] l_prevCmd;
    }

    m_prevCmds[m_cmdCount] = new char[strlen(p_str) + 1];
    strcpy(m_prevCmds[m_cmdCount], p_str);

    int l_newCmdCount = m_cmdCount + 1;
    bool l_check = m_cmdCount - 19 < 0;

    m_cmdCount = l_newCmdCount;

    if (l_check == l_newCmdCount > 20)
        m_cmdCount = 0;

    m_cmdCount2 = m_cmdCount;
}

double ZConsole::UnkFunc13()
{
    return unkFloat1;
}

/* ----------------- ZConsoleUnk ----------------- */

ZConsoleUnk::ZConsoleUnk(ZConsoleStruct *p_consoleStruct)
{
    m_consoleStruct = p_consoleStruct;
    unkInt2 = 0;
    unkInt3 = 0;
}

ZConsoleUnk::~ZConsoleUnk()
{
    delete (int *)unkInt2;
}
