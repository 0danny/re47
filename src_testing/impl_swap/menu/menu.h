#pragma once

#include <windows.h>
#include "impl_swap/utilities.h"
#include <gl/GL.h>

namespace Menu
{
    struct MenuItem
    {
        char *name;
        byte selected;
    };

    // glFinish Hook Stuff
    typedef void(WINAPI *glFinish_t)();
    static glFinish_t originalGlFinish = 0;
    void WINAPI hookedGlFinish();

    static byte isMenuOpen = FALSE;
    static byte fontBuilt = FALSE;

    static GLuint base;
    static int selectedIndex = 0;

    static MenuItem menuItems[4] = {
        {"General", FALSE},
        {"Visuals", FALSE},
        {"Misc", FALSE},
        {"Config", FALSE}};

    void CreateHooks();
    void DrawMenu();
    void HandleInput();

    void BuildFont(HDC hDC);
    void KillFont();
}