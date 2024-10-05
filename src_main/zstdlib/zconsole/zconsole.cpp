#include "zconsole.h"

// TODO: This implementation is incomplete
ZConsole::ZConsole()
{
    m_consoleCmd = *new ZConsoleCommand();

    m_visiblityProgress = 0.0;
    m_visibilitySpeed = 0.2;
    m_isVisible = 0;
    m_isAnimating = 0;

    strcpy(m_inputBuffer, ">");

    m_inputLength = 1;
    m_unkInt1 = 0;
    m_isAutoCompleting = 0;
    m_outputIndex = 0;
    m_outputScrollOffset = 0;
    m_shiftPressed = 0;

    for (int i = 0; i < 1000; i++)
    {
        m_outputBox[i] = new char[150];
    }

    memset(m_commandHistory, 0, sizeof(m_commandHistory));

    m_historyCount = 0;
    m_historyIndex = 0;

    ZConsoleStruct *l_consoleStruct = new ZConsoleStruct();

    if (l_consoleStruct)
    {
        l_consoleStruct->consoleStruct = 0;
        l_consoleStruct->commandArray = 0;

        l_consoleStruct->unkInt2 = 32;
        l_consoleStruct->unkInt3 = 0;
    }

    m_consoleStruct = l_consoleStruct;

    m_consoleUnk = new ZConsoleUnk(m_consoleStruct);
}

bBool ZConsole::UpdateConsoleVisibility()
{
    if (m_isVisible)
    {
        double l_m_visibilitySpeed = m_visibilitySpeed;

        m_isAnimating = 1;

        double l_add = l_m_visibilitySpeed + m_visiblityProgress;

        m_visiblityProgress = l_add;

        if (l_add >= 1.0)
        {
            m_visiblityProgress = 1.0;
            return 1;
        }
    }
    else
    {
        double l_sub = m_visiblityProgress - m_visibilitySpeed;

        m_visiblityProgress = l_sub;

        if (l_sub <= 0.0)
        {
            m_visiblityProgress = 0.0;
            m_isAnimating = 0;
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

    strncpy(m_outputBox[m_outputIndex], l_bracketOccurence + 1, 149u);

    m_outputBox[m_outputIndex][149] = '\0';

    int l_newIndex = m_outputIndex + 1;

    m_outputIndex = l_newIndex;

    if (l_newIndex == 1000)
        m_outputIndex = 0;
}

// Im not a million percent sure about the name
char *ZConsole::GetOutputLine(int p_offset)
{
    if (!p_offset)
        return m_inputBuffer;

    int l_offset = m_outputScrollOffset;

    if (l_offset + p_offset > 1000)
    {
        return g_emptyArray;
    }

    if (l_offset + p_offset < -1000)
    {
        return g_emptyArray;
    }

    int l_index = p_offset + l_offset + m_outputIndex;

    if (l_index < 0)
        l_index += 1000 * ((999 - l_index) / 1000u);

    if (l_index > 999)
        l_index %= 1000u;

    return m_outputBox[l_index];
}

uint8_t ZConsole::IsAnimating()
{
    return m_isAnimating;
}

// I've got no idea what this second parameter is - danny
void ZConsole::HandleInput(int p_keyCode, int p_lparam)
{
    uint16_t l_lparam = (uint16_t)(p_lparam);

    if (!p_lparam)
        l_lparam = 1;

    if (p_keyCode != VK_TAB && p_keyCode != VK_SHIFT)
    {
        m_isAutoCompleting = 0;
        m_unkInt1 = 0;
    }

    switch (p_keyCode)
    {
    case VK_BACK:
    case VK_DELETE:

        if (l_lparam)
        {
            int l_deleteCount = (l_lparam - 1) + 1;

            do
            {
                if (m_inputLength > 1)
                {
                    int newLength = (m_inputLength - 1);

                    m_inputLength = newLength;
                    m_inputBuffer[newLength] = '\0';
                }

                --l_deleteCount;
            } while (l_deleteCount);
        }
        return;
        return;

    case VK_TAB:
        // TODO: Implement auto-completion (0x0FFC5AE0).
        m_isAutoCompleting = 1;
        return;

    case VK_RETURN:
        ExecuteCommand();
        return;

    case VK_SHIFT:
        m_shiftPressed = 1;
        return;

    case VK_ESCAPE:
        m_inputLength = 1;
        m_inputBuffer[1] = '\0';
        return;

    case VK_PRIOR:
        if (m_outputScrollOffset > -1000)
            m_outputScrollOffset = m_outputScrollOffset - 1;

        return;

    case VK_NEXT:
        if (m_outputScrollOffset)
            m_outputScrollOffset = m_outputScrollOffset + 1;

        return;

    case VK_UP:
        GoPrevCmd(0);
        return;

    case VK_DOWN:
        GoPrevCmd(1);
        return;

    default: // If any other keys are pressed
        if (l_lparam)
        {
            int l_defaultCount = (l_lparam - 1) + 1;

            do
            {
                switch (p_keyCode)
                {
                case 189:
                    p_keyCode = '_';
                    break;
                case 190:
                    p_keyCode = '.';
                    break;
                case 191:
                    p_keyCode = '*';
                    break;
                }

                if (isalnum(p_keyCode) || p_keyCode == '_' || p_keyCode == ' ' || p_keyCode == '.' || p_keyCode == '*')
                {
                    m_inputBuffer[m_inputLength] = p_keyCode;

                    int newLen = m_inputLength + 1;

                    m_inputLength = newLen;
                    m_inputBuffer[newLen] = 0;
                }

                --l_defaultCount;

            } while (l_defaultCount);
        }
        return;
    }
}

void ZConsole::HandleKeyRelease(int p_keyCode, int p_unused)
{
    if (p_keyCode == VK_SHIFT)
        m_shiftPressed = 0;
}

void ZConsole::ExecuteCommand()
{
    char *l_txtBox = &m_inputBuffer[1];

    if (m_inputBuffer[1])
        AddToHistory(&m_inputBuffer[1]);

    m_outputScrollOffset = 0;

    char *l_subStr = strtok(l_txtBox, " ");

    char *l_cmd = l_subStr;

    if (l_subStr && *l_subStr)
    {
        char *l_cmdValue = strtok(0, g_emptyDelimiter); // Uses the same string as the previous strtok call

        if (m_consoleCmd.ExecuteCommand(l_cmd, l_cmdValue))
        {
            AddCmdText("%s %s", l_cmd, l_cmdValue);
            m_inputLength = 1;
            *l_txtBox = 0;
        }
        else
        {
            AddCmdText("%s - Unknown command - use <commands> to display list", l_cmd);
            *l_txtBox = 0;
            m_inputLength = 1;
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

// Probably debug, because it doesn't update the input buffer length
void ZConsole::ExecuteCommandString(const char *p_cmd)
{
    sprintf(m_inputBuffer, ">%s", p_cmd);

    ExecuteCommand();
}

void ZConsole::ToggleVisibility()
{
    m_isVisible = m_isVisible == 0;
}

void ZConsole::GoPrevCmd(bBool p_forwards)
{
    if (m_commandHistory[0])
    {
        if (p_forwards)
        {
            do
            {
                int l_cmdCount = m_historyCount + 1;
                m_historyCount = l_cmdCount;

                if (l_cmdCount == 20)
                    m_historyCount = 0;

            } while (!m_commandHistory[m_historyCount]);
        }
        else
        {
            do
            {
                if (m_historyCount-- == 0)
                    m_historyCount = 19;

            } while (!m_commandHistory[m_historyCount]);
        }

        char *l_newCmd = m_commandHistory[m_historyCount];

        if (l_newCmd)
        {
            strcpy(&m_inputBuffer[1], l_newCmd);

            m_inputLength = strlen(m_inputBuffer);
        }
    }
}

void ZConsole::AddToHistory(const char *p_command)
{
    char *l_prevCmd = m_commandHistory[m_historyIndex];

    if (l_prevCmd)
    {
        delete[] l_prevCmd;
    }

    m_commandHistory[m_historyIndex] = new char[strlen(p_command) + 1];
    strcpy(m_commandHistory[m_historyIndex], p_command);

    int l_newCmdCount = m_historyIndex + 1;
    bool l_check = m_historyIndex - 19 < 0;

    m_historyIndex = l_newCmdCount;

    if (l_check == l_newCmdCount > 20)
        m_historyIndex = 0;

    m_historyCount = m_historyIndex;
}

double ZConsole::GetVisibilityProgress()
{
    return m_visiblityProgress;
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
