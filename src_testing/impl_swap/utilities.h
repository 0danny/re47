#include <windows.h>

namespace Utilities
{
    static char *GetFileName(LPCSTR filePath)
    {
        const char *lastSlash = strrchr(filePath, '\\');

        if (!lastSlash)
        {
            lastSlash = strrchr(filePath, '/');
        }

        if (lastSlash)
        {
            return (char *)(lastSlash + 1);
        }
        else
        {
            return (char *)filePath;
        }
    }
}