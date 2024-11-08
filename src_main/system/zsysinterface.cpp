#include "zsysinterface.h"

ZSysInterface::ZSysInterface(HINSTANCE p_dllHinst)
{
}

ZSysInterface::~ZSysInterface()
{
}

boolean ZSysInterface::UnkFunc0()
{
    return false;
}

i32 ZSysInterface::InitializeResources()
{
    return 0;
}

void ZSysInterface::UnkFunc3()
{
}

void ZSysInterface::ChangeDisplaySettings()
{
    ::ChangeDisplaySettingsA(0, 0);
    exit(1);
}

i32 ZSysInterface::UnkFunc8()
{
    return 0;
}

void ZSysInterface::UnkFunc9()
{
}

void *ZSysInterface::InitActionMap()
{
    return 0;
}

void ZSysInterface::FreeActionMap()
{
}

u8 *ZSysInterface::UnkFunc12()
{
    return 0;
}

void ZSysInterface::UnkFunc13()
{
}

boolean ZSysInterface::ProcessWindowMessages(HWND p_hWnd)
{
    struct tagMSG message;

    ZSysInterface *l_self = this;

    if (m_messageFlag)
        return 1;

    m_messageFlag = 1;

    if (!PeekMessageA(&message, p_hWnd, 0, 0, 0))
    {
    RETURN_LBL:
        l_self->m_messageFlag = 0;
        return 1;
    }

    while (GetMessageA(&message, p_hWnd, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);

        if (!PeekMessageA(&message, p_hWnd, 0, 0, 0))
        {
            l_self = this;
            goto RETURN_LBL;
        }
    }

    return 0;
}

LRESULT ZSysInterface::DispatchAndAllocateMessage(WPARAM wParam, const void *message, i32 param)
{
    return 0;
}

char ZSysInterface::UnkFunc5(const char *str, i32 param)
{
    return 0;
}

char ZSysInterface::PrintStatus(char status)
{
    return 0;
}

boolean ZSysInterface::IsKeyPressed(i32 p_vKey)
{
    return (GetAsyncKeyState(p_vKey) & 0x8000) != 0;
}

char ZSysInterface::UnkFunc14(i32 param)
{
    return 0;
}

char ZSysInterface::UnkFunc15(i32 param)
{
    return 0;
}

char ZSysInterface::UnkFunc16(i32 param)
{
    return 0;
}

char ZSysInterface::UnkFunc18(i32 param)
{
    return 0;
}

char ZSysInterface::UnkFunc19(i32 param)
{
    return 0;
}

WORD ZSysInterface::ConvertVirtualKeyToAscii(u32 p_keyCode)
{
    WORD l_asciiChar[2];
    u8 l_keyboardState[256];

    GetKeyboardState(l_keyboardState);

    l_asciiChar[0] = 0;

    u32 l_scanCode = MapVirtualKeyA(p_keyCode, 0);
    i32 l_numCharsConverted = ToAscii(p_keyCode, l_scanCode, l_keyboardState, l_asciiChar, 0);

    return l_numCharsConverted < 1 ? 0 : l_asciiChar[0];
}

char **ZSysInterface::UnkFunc21(char **str, i32 param)
{
    return 0;
}

BOOL ZSysInterface::SetWindowTitle(const char *title)
{
    return FALSE;
}

i32 ZSysInterface::UnkFunc23(i32 param)
{
    return 0;
}

i32 ZSysInterface::UnkFunc24()
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

char **ZSysInterface::UnkFunc30(char **str, i32 param)
{
    return 0;
}

i32 ZSysInterface::UnkFunc31()
{
    return 0;
}

void ZSysInterface::GenerateUniqueFilename()
{
}

void ZSysInterface::UnkFunc33()
{
}

double ZSysInterface::UnkFunc34()
{
    return 0.0;
}

i32 ZSysInterface::UnkFunc35()
{
    return 0;
}

i32 ZSysInterface::UnkFunc36()
{
    return 0;
}

void ZSysInterface::UnkFunc37()
{
}

void ZSysInterface::Sleep(u32 milliseconds)
{
    ::Sleep(milliseconds);
}

void ZSysInterface::Init(char *p_cmdLineArgs)
{
}

char ZSysInterface::Restart(i32 param)
{
    return 0;
}

void ZSysInterface::UnkFunc40(i32 param1, i32 param2, i32 param3)
{
}

void ZSysInterface::UnkFunc41(i32 param1, i32 param2)
{
}

i32 ZSysInterface::UnkFunc42(char *msg)
{
    return 0;
}

char ZSysInterface::UnkFunc43(i32 param)
{
    return 0;
}

void ZSysInterface::UnkFunc44()
{
}

i32 ZSysInterface::UnkFunc45(i32 param1, i32 param2)
{
    return 0;
}

void ZSysInterface::UnkFunc46(i32 param1, i32 param2, i32 param3, i32 param4, char *format, ...)
{
    va_list args;
    va_start(args, format);
    va_end(args);
}

void ZSysInterface::UnkFunc47(i32 param1, i32 param2, i32 param3, char *format, ...)
{
    va_list args;
    va_start(args, format);
    va_end(args);
}

void ZSysInterface::UnkFunc48(i32 param1, char *format, ...)
{
    va_list args;
    va_start(args, format);
    va_end(args);
}

i32 ZSysInterface::UnkFunc49(i32 param)
{
    return 0;
}

i32 ZSysInterface::UnkFunc50(i32 param)
{
    return 0;
}

i32 ZSysInterface::UnkFunc51(i32 param)
{
    return 0;
}

ZConsole *ZSysInterface::GetConsole()
{
    return m_console;
}

i32 ZSysInterface::UnkFunc53(i32 param1, i32 param2, i32 param3)
{
    return 0;
}

i32 ZSysInterface::UnkFunc54(i32 param1, i32 param2)
{
    return 0;
}

double ZSysInterface::UnkFunc55(i32 param1, i32 param2)
{
    return 0.0;
}

double ZSysInterface::UnkFunc56(i32 param1, i32 param2)
{
    return 0.0;
}

i32 ZSysInterface::UnkFunc57(const void *ptr, u32 param1, i32 param2, const char *str, i32 param3)
{
    return 0;
}

char ZSysInterface::UnkFunc58(char *str, u32 param1, i32 param2, i32 param3, i32 param4)
{
    return 0;
}

i32 ZSysInterface::UnkFunc59(i32 param1, i32 param2, i32 param3, i32 param4, i32 param5)
{
    return 0;
}

i32 ZSysInterface::UnkFunc60(i32 param1, i32 param2)
{
    return 0;
}

i32 ZSysInterface::UnkFunc61()
{
    return 0;
}

void ZSysInterface::UnkFunc62(const char *msg)
{
}

void ZSysInterface::UnkFunc63(const char *msg)
{
}

void ZSysInterface::UnkFunc64()
{
}

void ZSysInterface::UnkFunc65()
{
}

ZSysInterface *ZSysInterface::UnkFunc66(const void *ptr, i32 param1, i32 param2, i32 param3)
{
    return 0;
}

u32 ZSysInterface::UnkFunc67(i32 param1, i32 param2)
{
    return 0;
}

FARPROC ZSysInterface::UnkFunc68(i32 param)
{
    return 0;
}

LRESULT ZSysInterface::WndProc(HWND hwnd, u32 uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}