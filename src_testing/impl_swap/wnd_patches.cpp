#include "wnd_patches.h"

namespace WndPatches
{
    void CreateHooks()
    {
        printf("[WND_PATCH]: Creating window patch hooks...\n");

        if (MH_CreateHookApi(L"user32", "SetWindowLongA", &WndPatches::HookedSetWindowLongA, NULL))
        {
            printf("[WND_PATCH]: Failed to hook SetWindowLongA!\n");
        }

        if (MH_CreateHookApi(L"user32", "AdjustWindowRectEx", &WndPatches::HookedAdjustWindowRectEx, NULL))
        {
            printf("[WND_PATCH]: Failed to hook AdjustWindowRectEx!\n");
        }

        if (MH_CreateHookApi(L"user32", "CreateWindowExA", &WndPatches::HookedCreateWindowExA, reinterpret_cast<LPVOID *>(&originalCreateWindowExA)) != MH_OK)
        {
            printf("[WND_PATCH]: Failed to hook CreateWindowExA!\n");
        }
    }

    LONG WINAPI HookedSetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong)
    {
        printf("[WND_PATCH]: Diverting SetWindowLongA!\n");

        return 0;
    }

    BOOL WINAPI HookedAdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle)
    {
        printf("[WND_PATCH]: Diverting AdjustWindowEx!\n");

        return 0;
    }

    HWND WINAPI HookedCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
                                      DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                                      HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
    {
        printf("[WND_PATCH]: Patching CreateWindowExA!\n");
        printf("[WND PATCH] lpWindowName: %s, X: %d, Y: %d, Width: %d, Height: %d\n", lpWindowName, x, y, nWidth, nHeight);

        dwStyle = WS_OVERLAPPEDWINDOW;

        return originalCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    }
}
