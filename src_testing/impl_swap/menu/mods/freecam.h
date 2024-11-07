#ifndef FREECAM_H
#define FREECAM_H

#include <windows.h>
#include <minhook.h>
#include <gl/GL.h>
#include <iostream>

#include <cmath>

#define VK_Y 0x59
#define VK_G 0x47
#define VK_H 0x48
#define VK_J 0x4A
#define VK_R 0x52
#define VK_F 0x46

namespace FreeCam
{
    // free cam hook
    static LPVOID zglDrawFunc42Address = (LPVOID)0x0FB73400;

    static float camX = 0.0f;
    static float camY = 0.0f;
    static float camZ = 0.0f;

    static byte inFreeCam = FALSE;

    void HandleInput();

    typedef void(__stdcall *ZGLDrawUnkFunc24_t)(float p_left, float p_right, float p_bottom, float p_top, float p_near, float p_far, bool p_projectionType);
    static ZGLDrawUnkFunc24_t originalZGLDrawUnkFunc24 = 0;
    void __stdcall hookedZGLDrawUnkFunc24(float p_left, float p_right, float p_bottom, float p_top, float p_near, float p_far, bool p_projectionType);
}

#endif