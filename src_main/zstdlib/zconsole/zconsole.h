#pragma once

#include "common.h"
#include "zconsolecommand.h"

struct ZConsoleArray
{
    ZConsoleArray *nextArray;
    char **commandList;
};

struct ZAutoComplete
{
    ZConsoleArray *commandArray;
    ZConsoleArray *lastCommandArray;
    int arraySize;
    int commandCount;
};

class ZAutoCompleteHandler
{
public:
    ZAutoComplete *m_consoleStruct;
    char *m_currentMatch;
    int m_matchIndex;

    ZAutoCompleteHandler(ZAutoComplete *p_consoleStruct);
    virtual ~ZAutoCompleteHandler();

    const char *GetMatch(char *p_cmd);
};

#pragma pack(push, 1)

class ZConsole
{
public:
    // Visibility stuff
    uint8_t m_isVisible;
    uint8_t m_shiftPressed;
    uint8_t m_isAnimating;
    float m_visiblityProgress;
    float m_visibilitySpeed;

    // Output box
    char *m_outputBox[1000];
    int m_outputIndex;
    int m_outputScrollOffset;

    // Command history
    char *m_commandHistory[20];
    char m_inputBuffer[200];
    int m_inputLength;

    int m_historyIndex;
    int m_historyCount;

    ZConsoleCommand m_consoleCmd;

    int m_autoCompleteIndex;
    uint8_t m_isAutoCompleting;

    ZAutoComplete *m_autoComplete;
    ZAutoCompleteHandler *m_autoCompleteHandler;

    ZConsole();

    virtual bBool UpdateConsoleVisibility();

    virtual void AddCmdText(const char *p_format, ...);
    virtual char *GetOutputLine(int p_offset);

    virtual uint8_t IsAnimating();

    virtual void HandleInput(int p_identifier, char *p_cmdName);
    virtual void HandleKeyRelease(int p_keyCode, int p_unused);

    virtual void ExecuteCommand();
    virtual ZCmdNode *RegisterCommand(ZCmdHandler *p_handler);
    virtual void UnregisterCommand(ZCmdHandler *p_handler);

    virtual void ExecuteCommandString(const char *p_cmd);
    virtual void ToggleVisibility();

    virtual void GoPrevCmd(bBool p_forwards);
    virtual void AddToHistory(const char *p_command);
    virtual double GetVisibilityProgress();
};

#pragma pack(pop)