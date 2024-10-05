#pragma once

#include "common.h"
#include "zconsolecommand.h"

class ZConsoleUnk;

struct ZConsoleStruct
{
    ZConsoleStruct *consoleStruct;
    char **commandArray;
    int unkInt2;
    int unkInt3;
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

    int m_unkInt1;
    uint8_t m_isAutoCompleting;

    ZConsoleStruct *m_consoleStruct;
    ZConsoleUnk *m_consoleUnk;

    ZConsole();

    virtual bBool UpdateConsoleVisibility();

    virtual void AddCmdText(const char *p_format, ...);
    virtual char *GetOutputLine(int p_offset);

    virtual uint8_t IsAnimating();

    virtual void HandleInput(int p_identifier, int p_lparam);
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

class ZConsoleUnk
{
public:
    ZConsoleStruct *m_consoleStruct;
    int unkInt2;
    int unkInt3;

    ZConsoleUnk(ZConsoleStruct *p_consoleStruct);
    virtual ~ZConsoleUnk();
};