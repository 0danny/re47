#include "menu.h"

namespace Menu
{
    void BuildFont(HDC hDC)
    {
        HFONT font;

        base = glGenLists(96);

        font = CreateFontA(-32,
                           0,
                           0,
                           0,
                           FW_BOLD,
                           FALSE,
                           FALSE,
                           FALSE,
                           ANSI_CHARSET,
                           OUT_TT_PRECIS,
                           CLIP_DEFAULT_PRECIS,
                           ANTIALIASED_QUALITY,
                           FF_DONTCARE | DEFAULT_PITCH,
                           "Arial");

        SelectObject(hDC, font);
        wglUseFontBitmaps(hDC, 32, 96, base);
        DeleteObject(font);
    }

    void KillFont()
    {
        glDeleteLists(base, 96);
    }

    void CreateHooks()
    {
        printf("[Re47 Menu] Creating hooks...\n");

        // glFinish
        if (MH_CreateHookApi(L"opengl32", "glFinish", (LPVOID)&hookedGlFinish, (LPVOID *)&originalGlFinish) != MH_OK)
        {
            printf("[Re47 Menu] Failed to create glFinish hook\n");
        }
    }

    void DrawMenu()
    {
        // Background
        glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
        glBegin(GL_QUADS);
        glVertex2i(50, 50);
        glVertex2i(350, 50);
        glVertex2i(350, 600);
        glVertex2i(50, 600);
        glEnd();

        // Title bar
        glColor4f(0.4f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
        glVertex2i(50, 50);
        glVertex2i(350, 50);
        glVertex2i(350, 100);
        glVertex2i(50, 100);
        glEnd();

        // Draw Title text
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2i(120, 85);

        Utilities::glPrint(base, "Re47 Menu");

        int yPos = 150;
        for (int i = 0; i < 4; i++)
        {
            if (i == selectedIndex)
            {
                glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
                glBegin(GL_QUADS);
                glVertex2i(50, yPos - 30);
                glVertex2i(350, yPos - 30);
                glVertex2i(350, yPos + 10);
                glVertex2i(50, yPos + 10);
                glEnd();
            }

            glColor3f(1.0f, 1.0f, 1.0f);

            glRasterPos2i(70, yPos);

            Utilities::glPrint(base, "%s", menuItems[i].name);

            yPos += 60;
        }
    }

    void HandleInput()
    {
        if (GetAsyncKeyState(VK_UP) & 1)
        {
            if (selectedIndex > 0)
                selectedIndex--;
        }

        if (GetAsyncKeyState(VK_DOWN) & 1)
        {
            if (selectedIndex < 3)
                selectedIndex++;
        }

        if (GetAsyncKeyState(VK_RETURN) & 1)
        {
            menuItems[selectedIndex].selected = !menuItems[selectedIndex].selected;
        }
    }

    void WINAPI hookedGlFinish()
    {
        if (!fontBuilt)
        {
            BuildFont(wglGetCurrentDC());
            fontBuilt = true;
        }

        // Open the menu
        if (GetAsyncKeyState(VK_F7) & 1)
        {
            printf("[Re47 Menu] Toggling menu\n");
            isMenuOpen = !isMenuOpen;
        }

        if (isMenuOpen)
        {
            glPushAttrib(GL_ALL_ATTRIB_BITS);

            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_CULL_FACE);

            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);

            glMatrixMode(GL_PROJECTION);
            glPushMatrix();
            glLoadIdentity();
            glOrtho(0, viewport[2], viewport[3], 0, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadIdentity();

            HandleInput();
            DrawMenu();

            glMatrixMode(GL_PROJECTION);
            glPopMatrix();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();

            glPopAttrib();
        }

        originalGlFinish();
    }
}
