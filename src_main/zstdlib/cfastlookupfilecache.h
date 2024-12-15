#ifndef CFASTLOOKUPFILECACHE_H
#define CFASTLOOKUPFILECACHE_H

#include "common.h"
#include "cfastlookup2.h"
#include "reftab/reftab32.h"

#pragma pack(push, 1)

struct IOZip_LocalFileHeader
{
    u32 unkInt1;
    u16 unkInt2;
    u16 unkInt3;
    u16 unkInt4;
    u32 unkInt5;
    u32 unkInt6;
    u32 unkInt7;
    u16 unkInt8;
    u16 unkInt9;
};

struct CFileCache
{
    IOZip_LocalFileHeader fileHeader;
    u32 data;
};

class CFastLookupFileCache
{
public:
    CFastLookup2 m_fastLookup;
    RefTab32 m_refTab32;

    inline CFastLookupFileCache() : m_fastLookup(32), m_refTab32() {}

    virtual ~CFastLookupFileCache();
    virtual void Add(const char *p_str, IOZip_LocalFileHeader *p_localFileHeader, u32 p_data);
    virtual bool Lookup(const char *p_str, IOZip_LocalFileHeader *p_localFileHeader, u32 *p_result);
    virtual void Invalidate(const char *p_str);
    virtual void Flush();

    void Destroy();
};

#pragma pack(pop)

#endif