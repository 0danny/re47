#pragma once

#include <new>
#include <cstdlib>
#include <windows.h>

// Avoid circular dependencies
class ZSysMem;
extern __declspec(dllimport) ZSysMem *g_pSysMem;

// Typedefs
typedef byte boolean;

typedef char i8;
typedef short i16;
typedef int i32;
typedef long i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

static char *g_emptyArray = new char[264];
static char *g_emptyDelimiter = new char[4];

// Mem alloc overloads
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

// Exceptions
class ZArrayRangeError
{
public:
    ZArrayRangeError(const char *msg) : message(msg) {}

    const char *message;
};