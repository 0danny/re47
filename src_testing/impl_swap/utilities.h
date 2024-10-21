#ifndef UTILITIES_H
#define UTILITIES_H

#include <windows.h>
#include <stdio.h>
#include <iostream>

#include <minhook.h>
#include <gl/GL.h>

namespace Utilities
{
    static const char *TITLE = "Implementation Swap - 0.0.1";

    static char *GetFileName(LPCSTR filePath)
    {
        const char *lastSlash = strrchr(filePath, '\\');

        if (!lastSlash)
            lastSlash = strrchr(filePath, '/');

        if (lastSlash)
            return (char *)(lastSlash + 1);
        else
            return (char *)filePath;
    }

    static void glPrint(GLuint base, const char *fmt, ...)
    {
        char text[256];
        va_list ap;

        if (fmt == NULL)
            return;

        va_start(ap, fmt);
        vsprintf(text, fmt, ap);
        va_end(ap);

        glPushAttrib(GL_LIST_BIT);
        glListBase(base - 32);
        glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
        glPopAttrib();
    }
}

#endif