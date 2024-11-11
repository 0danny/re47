#ifndef CFASTLOOKUP2_H
#define CFASTLOOKUP2_H

#include "common.h"
#include "zstdlib/trees/zvaltree.h"
#include "zstdlib/reftab/linkreftab.h"
#include "system/zsyscom.h"
#include "zstdlib/mystr.h"

struct CFastLink
{
    char *stringPtr;
    i32 stringLength;
    u32 unkSize;
    u8 isAllocated;
};

class CFastLookup2
{
public:
    ZValTree *m_valTree;

    CFastLookup2(i32 p_poolSize);
    ~CFastLookup2();

    void Clear();
    void Reset();
    void Remove(const char *p_str, i32 p_size);
    CFastLink *FindFastLink(LinkRefTab *p_linkTab, const char *p_str, i32 p_size);
    inline void Set(const char *p_str, i32 p_size);

    void RemoveLowerCase(const char *p_str, u32 p_size);
    void SetLowerCase(const char *p_str, u32 p_size);
};

#endif