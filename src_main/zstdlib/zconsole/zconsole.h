#ifndef ZCONSOLE_H
#define ZCONSOLE_H

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
    i32 arraySize;
    i32 commandCount;
};

class ZAutoCompleteHandler
{
public:
    ZAutoComplete *m_consoleStruct;
    char *m_currentMatch;
    i32 m_matchIndex;

    ZAutoCompleteHandler(ZAutoComplete *p_consoleStruct);
    virtual ~ZAutoCompleteHandler();

    const char *GetMatch(char *p_cmd);
};

#pragma pack(push, 1)

class ZConsole
{
public:
    // Visibility stuff
    u8 m_isVisible;
    u8 m_shiftPressed;
    u8 m_isAnimating;
    float m_visiblityProgress;
    float m_visibilitySpeed;

    // Output box
    char *m_outputBox[1000];
    i32 m_outputIndex;
    i32 m_outputScrollOffset;

    // Command history
    char *m_commandHistory[20];
    char m_inputBuffer[200];
    i32 m_inputLength;

    i32 m_historyIndex;
    i32 m_historyCount;

    ZConsoleCommand m_consoleCmd;

    i32 m_autoCompleteIndex;
    u8 m_isAutoCompleting;

    ZAutoComplete *m_autoComplete;
    ZAutoCompleteHandler *m_autoCompleteHandler;

    ZConsole();

    virtual boolean UpdateConsoleVisibility();

    virtual void AddCmdText(const char *p_format, ...);
    virtual char *GetOutputLine(i32 p_offset);

    virtual u8 IsAnimating();

    virtual void HandleInput(i32 p_keyCode, char *p_cmdName);
    virtual void HandleKeyRelease(i32 p_keyCode, i32 p_unused);

    virtual void ExecuteCommand();
    virtual ZCmdNode *RegisterCommand(ZCmdHandler *p_handler);
    virtual void UnregisterCommand(ZCmdHandler *p_handler);

    virtual void ExecuteCommandString(const char *p_cmd);
    virtual void ToggleVisibility();

    virtual void GoPrevCmd(boolean p_forwards);
    virtual void AddToHistory(const char *p_command);
    virtual double GetVisibilityProgress();

    void Destroy();
};

#pragma pack(pop)

#endif