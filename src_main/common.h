#pragma once

#include <new>
#include <cstdlib>
#include "system/zsysmem.h"

// Typedefs
typedef byte bBool;
typedef unsigned char uint8_t;
typedef long int64_t;
typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;

static char *g_emptyArray = new char[264];

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