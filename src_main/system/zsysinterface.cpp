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

void ZSysInterface::ChangeDisplaySettings() {}

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

bBool ZSysInterface::ProcessWindowMessages(HWND hwnd)
{
    return false;
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

bBool ZSysInterface::IsKeyPressed(int vKey)
{
    return false;
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

WORD ZSysInterface::UnkFunc20(UINT param)
{
    return 0;
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

void ZSysInterface::Init(char *p_cmdLineArgs) {}

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

int ZSysInterface::GetConsole()
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
