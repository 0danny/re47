#include <windows.h>
#include <csignal>

#include "system/zsysinterface.h"

void SignalHandler(i32 signal);
i32 DoAlwaysNoConv(HINSTANCE instance);

i32 WINAPI WinMain(HINSTANCE p_hInstance, HINSTANCE p_hPrev, LPSTR p_lpCmdLine, i32 p_nShowCmd)
{
    char l_filePath[264];
    char l_fileBuffer[264];

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
            // Overwrite the .exe extension with .ini
            GetModuleFileNameA(p_hInstance, l_filePath, sizeof(l_filePath));
            strcpy(&l_filePath[strlen(l_filePath) - 4], ".ini");

            HANDLE l_iniFile = CreateFileA(l_filePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

            if (l_iniFile == (HANDLE)-1)
            {
                // Fallback to @main.ini
                l_cmdLinePtr = "@main.ini";
            }
            else
            {
                CloseHandle(l_iniFile);

                // Adds a '@' to the beginning of the string.
                l_fileBuffer[0] = '@';
                strcpy(&l_fileBuffer[1], l_filePath);

                l_cmdLinePtr = l_fileBuffer;
            }
        }

        g_pSysInterface->Init(l_cmdLinePtr);
        FreeLibrary(l_systemLib);
    }

    return 0;
}

i32 DoAlwaysNoConv(HINSTANCE instance)
{
    return 1;
}

void SignalHandler(i32 signal)
{
    ZDynamicLoader *l_soundDLLBase = g_pSysInterface->m_soundDLLBase;

    if (l_soundDLLBase)
        l_soundDLLBase->CrashFree();

    ZDynamicLoader *l_drawDLLBase = g_pSysInterface->m_drawDLLBase;

    if (l_drawDLLBase)
        l_drawDLLBase->CrashFree();
}
