#pragma once

#include <new>
#include <cstdlib>
#include "system/zsysmem.h"

// Typedefs
typedef byte bBool;

namespace Common
{
    inline void *Alloc(size_t size)
    {
        if (g_pSysMem)
            // TODO: Implement this when g_pSysMem is available
            // return g_pSysMem->vtable->Alloc(size);
            return malloc(size);
        else
            return malloc(size);
    }

    inline void Free(void *ptr)
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

    inline void *AllocArr(size_t size)
    {
        if (g_pSysMem)
            // TODO: Implement this when g_pSysMem is available
            // return g_pSysMem->vtable->Alloc(size);
            return malloc(size & 0x1FFFFFFF);
        else
            return malloc(size & 0x1FFFFFFF);
    }

    inline void FreeArr(void *ptr)
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
}
