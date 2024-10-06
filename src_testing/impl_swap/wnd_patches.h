#pragma once

#include <windows.h>
#include <minhook.h>
#include <iostream>

namespace WndPatches
{
    typedef LONG(WINAPI *SetWindowLongA_t)(HWND hWnd, int nIndex, LONG dwNewLong);
    typedef BOOL(WINAPI *AdjustWindowRectEx_t)(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);
    typedef HWND(WINAPI *CreateWindowExA_t)(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
                                            DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                                            HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
    typedef LRESULT(WINAPI *SendMessageA_t)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

    static CreateWindowExA_t originalCreateWindowExA = 0;

    LONG WINAPI HookedSetWindowLongA(HWND hWnd, int nIndex, LONG dwNewLong);
    BOOL WINAPI HookedAdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);
    HWND WINAPI HookedCreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName,
                                      DWORD dwStyle, int x, int y, int nWidth, int nHeight,
                                      HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);

    void CreateHooks();
}
