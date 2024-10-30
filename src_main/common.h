#ifndef COMMON_H
#define COMMON_H

#include <new>
#include <cstdlib>
#include <stdio.h>
#include <windows.h>

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

// Exceptions
class ZArrayRangeError
{
public:
    ZArrayRangeError(const char *msg) : message(msg) {}

    const char *message;
};

#endif