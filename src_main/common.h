#pragma once

#include <new>
#include <cstdlib>
#include "system/zsysmem.h"

// Typedefs
typedef byte bBool;

// return malloc(size & 0x1FFFFFFF);

inline static void *operator new(size_t size)
{
    if (g_pSysMem)
        // TODO: Implement this when g_pSysMem is available
        // return g_pSysMem->vtable->Alloc(size);
        return malloc(size);
    else
        return malloc(size);
}

inline static void operator delete(void *ptr)
{
    if (g_pSysMem)
    {
        // TODO: Implement this when g_pSysMem is available
        // g_pSysMem->vtable->Free(ptr);
        free(ptr);
        return;
    }
    free(ptr);
}