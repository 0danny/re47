#include "zsysinterface.h"

ZSysInterface::ZSysInterface()
{
}

ZSysInterface::~ZSysInterface()
{
}

bBool ZSysInterface::UnkFunc0()
{
    return false;
}

int ZSysInterface::InitializeResources()
{
    return 0;
}

void ZSysInterface::UnkFunc3() {}

void ZSysInterface::ChangeDisplaySettings()
{
    ::ChangeDisplaySettingsA(0, 0);
    exit(1);
}

int ZSysInterface::UnkFunc8()
{
    return 0;
}

void ZSysInterface::UnkFunc9() {}

void *ZSysInterface::InitActionMap()
{
    return 0;
}

void ZSysInterface::FreeActionMap() {}

bBool ZSysInterface::ProcessWindowMessages(HWND p_hWnd)
{
    struct tagMSG message;
    ZSysInterface *self = this;

    if (this->m_messageFlag)
        return 1;

    this->m_messageFlag = 1;

    if (!PeekMessageA(&message, p_hWnd, 0, 0, 0))
    {
    RETURN_LBL:
        self->m_messageFlag = 0;
        return 1;
    }

    while (GetMessageA(&message, p_hWnd, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);

        if (!PeekMessageA(&message, p_hWnd, 0, 0, 0))
        {
            self = this;
            goto RETURN_LBL;
        }
    }

    return 0;
}

LRESULT ZSysInterface::DispatchAndAllocateMessage(WPARAM wParam, const void *message, int param)
{
    return 0;
}

char ZSysInterface::UnkFunc5(const char *str, int param)
{
    return 0;
}

char ZSysInterface::PrintStatus(char status)
{
    return 0;
}

bBool ZSysInterface::IsKeyPressed(int p_vKey)
{
    return (GetAsyncKeyState(p_vKey) & 0x8000) != 0;
}

char ZSysInterface::UnkFunc14(int param)
{
    return 0;
}

char ZSysInterface::UnkFunc15(int param)
{
    return 0;
}

char ZSysInterface::UnkFunc16(int param)
{
    return 0;
}

char ZSysInterface::UnkFunc18(int param)
{
    return 0;
}

char ZSysInterface::UnkFunc19(int param)
{
    return 0;
}

WORD ZSysInterface::ConvertVirtualKeyToAscii(UINT p_keyCode)
{
    WORD l_asciiChar[2];
    byte l_keyboardState[256];

    GetKeyboardState(l_keyboardState);

    l_asciiChar[0] = 0;

    UINT l_scanCode = MapVirtualKeyA(p_keyCode, 0);
    int l_numCharsConverted = ToAscii(p_keyCode, l_scanCode, l_keyboardState, l_asciiChar, 0);

    return l_numCharsConverted < 1 ? 0 : l_asciiChar[0];
}

char **ZSysInterface::UnkFunc21(char **str, int param)
{
    return 0;
}

BOOL ZSysInterface::SetWindowTitle(const char *title)
{
    return FALSE;
}

int ZSysInterface::UnkFunc23(int param)
{
    return 0;
}

int ZSysInterface::UnkFunc24()
{
    return 0;
}

void ZSysInterface::UnkFunc25(const char *msg)
{
}

void ZSysInterface::UnkFunc26(const char *msg)
{
}

void ZSysInterface::UnkFunc27(const char *msg)
{
}

void ZSysInterface::UnkFunc28(const char *msg)
{
}

void ZSysInterface::UnkFunc29(const char *msg)
{
}

char **ZSysInterface::UnkFunc30(char **str, int param)
{
    return 0;
}

int ZSysInterface::UnkFunc31()
{
    return 0;
}

void ZSysInterface::GenerateUniqueFilename() {}

void ZSysInterface::UnkFunc33() {}

double ZSysInterface::UnkFunc34()
{
    return 0.0;
}

int ZSysInterface::UnkFunc35()
{
    return 0;
}

int64_t ZSysInterface::UnkFunc36()
{
    return 0;
}

void ZSysInterface::UnkFunc37() {}

void ZSysInterface::Sleep(DWORD milliseconds)
{
    ::Sleep(milliseconds);
}

void ZSysInterface::Init(char *p_cmdLineArgs)
{
}

char ZSysInterface::Restart(int param)
{
    return 0;
}

void ZSysInterface::UnkFunc40(int param1, int param2, int param3) {}

void ZSysInterface::UnkFunc41(int param1, int param2) {}

int ZSysInterface::UnkFunc42(char *msg)
{
    return 0;
}

char ZSysInterface::UnkFunc43(int param)
{
    return 0;
}

void ZSysInterface::UnkFunc44() {}

int ZSysInterface::UnkFunc45(int param1, int param2)
{
    return 0;
}

void ZSysInterface::UnkFunc46(int param1, int param2, int param3, int param4, char *format, ...)
{
    va_list args;
    va_start(args, format);
    va_end(args);
}

void ZSysInterface::UnkFunc47(int param1, int param2, int param3, char *format, ...)
{
    va_list args;
    va_start(args, format);
    va_end(args);
}

void ZSysInterface::UnkFunc48(int param1, char *format, ...)
{
    va_list args;
    va_start(args, format);
    va_end(args);
}

int ZSysInterface::UnkFunc49(int param)
{
    return 0;
}

int ZSysInterface::UnkFunc50(int param)
{
    return 0;
}

int ZSysInterface::UnkFunc51(int param)
{
    return 0;
}

ZConsole *ZSysInterface::GetConsole()
{
    return m_consoleInstance;
}

int ZSysInterface::UnkFunc53(int param1, int param2, int param3)
{
    return 0;
}

int ZSysInterface::UnkFunc54(int param1, int param2)
{
    return 0;
}

double ZSysInterface::UnkFunc55(int param1, int param2)
{
    return 0.0;
}

double ZSysInterface::UnkFunc56(int param1, int param2)
{
    return 0.0;
}

int ZSysInterface::UnkFunc57(const void *ptr, unsigned int param1, int param2, const char *str, int param3)
{
    return 0;
}

char ZSysInterface::UnkFunc58(char *str, unsigned int param1, int param2, int param3, int param4)
{
    return 0;
}

int ZSysInterface::UnkFunc59(int param1, int param2, int param3, int param4, int param5)
{
    return 0;
}

int ZSysInterface::UnkFunc60(int param1, int param2)
{
    return 0;
}

int ZSysInterface::UnkFunc61()
{
    return 0;
}

void ZSysInterface::UnkFunc62(const char *msg) {}
void ZSysInterface::UnkFunc63(const char *msg) {}
void ZSysInterface::UnkFunc64() {}
void ZSysInterface::UnkFunc65() {}

ZSysInterface *ZSysInterface::UnkFunc66(const void *ptr, signed int param1, int param2, int param3)
{
    return 0;
}

uint64_t ZSysInterface::UnkFunc67(int param1, int param2)
{
    return 0;
}

FARPROC ZSysInterface::UnkFunc68(int param)
{
    return 0;
}

LRESULT ZSysInterface::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}