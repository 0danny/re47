#pragma once

#include <windows.h>

namespace Utilities
{
    static const char *TITLE = "Implementation Swap - 0.0.1";

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
