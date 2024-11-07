#include "freecam.h"

namespace FreeCam
{
    void HandleInput()
    {
        if (GetAsyncKeyState(VK_NUMPAD9) & 1)
        {
            printf("[Re47 Menu] Enabling Free Cam Hook.\n");

            // ZGLDrawUnkFunc24
            if (MH_CreateHook(zglDrawFunc42Address, (LPVOID)&hookedZGLDrawUnkFunc24, (LPVOID *)&originalZGLDrawUnkFunc24) != MH_OK)
            {
                printf("[Re47 Menu] Failed to create ZGLDrawUnkFunc24 hook\n");
            }

            MH_EnableHook(MH_ALL_HOOKS);
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            inFreeCam = !inFreeCam;

            printf("[Re47 Menu] Free Cam -> %s\n", inFreeCam ? "Enabled" : "Disabled");
        }
    }

    void __stdcall hookedZGLDrawUnkFunc24(float p_left, float p_right, float p_bottom, float p_top, float p_near, float p_far, bool p_projectionType)
    {
        float modified_p_far = p_far * 100.0f;

        originalZGLDrawUnkFunc24(p_left, p_right, p_bottom, p_top, p_near, modified_p_far, p_projectionType);

        if (inFreeCam)
        {
            float moveSpeed = 5.0f;

            // Handle movement
            if (GetAsyncKeyState(VK_Y) & 0x8000) // Forward
            {
                camZ += moveSpeed;
                printf("Moving forward: camZ = %f\n", camZ);
            }

            if (GetAsyncKeyState(VK_H) & 0x8000) // Backward
            {
                camZ -= moveSpeed;
                printf("Moving backward: camZ = %f\n", camZ);
            }

            if (GetAsyncKeyState(VK_G) & 0x8000) // Left
            {
                camX += moveSpeed;
                printf("Moving left: camX = %f\n", camX);
            }

            if (GetAsyncKeyState(VK_J) & 0x8000) // Right
            {
                camX -= moveSpeed;
                printf("Moving right: camX = %f\n", camX);
            }

            // Add up/down movement
            if (GetAsyncKeyState(VK_R) & 0x8000) // Up
            {
                camY += moveSpeed;
            }

            if (GetAsyncKeyState(VK_F) & 0x8000) // Down
            {
                camY -= moveSpeed;
            }

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glDisable(GL_CULL_FACE);
            glTranslatef(camX, camY, camZ);
        }
    }

}
