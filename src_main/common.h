#ifndef COMMON_H
#define COMMON_H

#define WINVER 0x0500
#include <windows.h>

#include <new>
#include <cstdlib>
#include <stdio.h>

// Typedefs
typedef byte boolean;

typedef char i8;
typedef short i16;
typedef int i32;
typedef __int64 i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned __int64 u64;

static char *g_emptyArray = new char[264];
static char *g_emptyDelimiter = new char[4];

// Static class size assert macro
#define DECOMP_SIZE(type, size) static_assert(sizeof(type) == size, "Size does not match.");

// Static Helper Functions
static FILE *LockedFileOpen(const char *p_fileName, const char *p_mode)
{
    return _fsopen(p_fileName, p_mode, 64);
}

// We don't have x86intrin.h, so we have to inline the rdtsc ASM syscall manually.
static inline u64 GetRDTSC(void)
{
    u64 cycles;
    __asm {
            _emit 0x0F // RDTSC instruction (0F 31)
            _emit 0x31
            mov DWORD PTR cycles, eax // Store low 32 bits
            mov DWORD PTR cycles+4, edx // Store high 32 bits
    }
    return cycles;
}

// Lets use compile using override keyword with VS6 (used for intellisense)
#if __cplusplus < 201103L
#define override
#endif

// Exceptions
class ZArrayRangeError
{
public:
    ZArrayRangeError(const char *msg) : message(msg) {}

    const char *message;
};

#endif