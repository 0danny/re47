#include <windows.h>
#include <csignal>

#include "System/ZSysInterface.h"

extern __declspec(dllimport) ZSysInterface *g_pSysInterface;

// Function prototypes
void SignalHandler(int signal);
BOOL DoAlwaysNoConv(HINSTANCE instance);

static const DWORD dword_40703C = 0xE4E46A01;
static const DWORD dword_407044 = 0x8BEEAD10;

int WINAPI WinMain(HINSTANCE p_hInstance, HINSTANCE p_hPrev, LPSTR p_lpCmdLine, int p_nShowCmd)
{
    char l_iniBuffer[4];        // [esp+4h] [ebp-210h] BYREF
    char l_filePath[260];       // [esp+8h] [ebp-20Ch] BYREF
    char l_filePathBuffer[263]; // [esp+10Dh] [ebp-107h] BYREF

    CreateMutexA(0, 0, "Hitboy");

    if (GetLastError() != ERROR_ALREADY_EXISTS)
    {
        if (!DoAlwaysNoConv(p_hInstance))
            return 1;

        signal(SIGFPE, SignalHandler);
        signal(SIGABRT, SignalHandler);
        signal(SIGILL, SignalHandler);
        signal(SIGSEGV, SignalHandler);

        HMODULE l_systemLib = LoadLibraryA("System.dll");

        if (!l_systemLib)
            return 1;

        LPSTR l_cmdLinePtr = p_lpCmdLine;

        if (strlen(p_lpCmdLine) < 2)
        {
            GetModuleFileNameA(p_hInstance, l_filePath, 260u);

            strcpy(&l_iniBuffer[strlen(l_filePath)], ".ini");

            HANDLE l_iniFile = CreateFileA(l_filePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

            if (l_iniFile == (HANDLE)-1)
            {
                l_cmdLinePtr = "@main.ini";
            }
            else
            {
                CloseHandle(l_iniFile);

                char l_atSymbol = '@';

                strcpy(l_filePathBuffer, l_filePath);

                l_cmdLinePtr = &l_atSymbol;
            }
        }

        g_pSysInterface->Func40(l_cmdLinePtr);

        //(*(void(__thiscall **)(struct ZSysInterface *, LPSTR))(*(_DWORD *)g_pSysInterface + 160))(g_pSysInterface, l_cmdLinePtr);

        FreeLibrary(l_systemLib);

        // Ive got next to no idea, what this doing.
        // In the original code these two variables are never written to/or passed into anything.
        // When putting a breakpoint on them, it never gets hit.

        if (!dword_40703C)
        {
        }
        // dword_407044();
    }

    return 0;
}

BOOL DoAlwaysNoConv(HINSTANCE instance)
{
    return 1;
}

// TODO: Handle these correctly.
void SignalHandler(int signal)
{
    // Show a messagebox.
    MessageBoxA(0, "Unhandled exception -> " + signal, "Error", 0);

    exit(-1);
}