#ifndef CFASTLOOKUPFILECACHE_H
#define CFASTLOOKUPFILECACHE_H

#include "common.h"
#include "cfastlookup2.h"
#include "reftab/reftab32.h"

#pragma pack(push, 1)

struct IOZip_LocalFileHeader
{
    u32 signature;
    u16 compressionMethod;
    u16 fatTime;
    u16 fatDate;
    u32 lastModTime;
    u32 crc32;
    u32 compressedSize;
    u16 uncompressedSize;
    u16 fileNameLength;
};

struct CFileCache
{
    IOZip_LocalFileHeader fileHeader;
    u32 data;
};

// TODO: This class is actually 176 bytes in size, but even with the lookup and reftab it only comes to 48.
// It either means that RefTab32 members are wrong, and theres more. Which I haven't seen anywhere, or
// there is more to this class.
class CFastLookupFileCache
{
public:
    CFastLookup2 m_fastLookup;
    RefTab32 m_refTab32;

    inline CFastLookupFileCache() : m_fastLookup(32), m_refTab32() {}

    virtual ~CFastLookupFileCache();
    virtual void Add(const char *p_str, IOZip_LocalFileHeader *p_localFileHeader, u32 p_data);
    virtual boolean Lookup(const char *p_str, IOZip_LocalFileHeader *p_localFileHeader, u32 *p_result);
    virtual void Invalidate(const char *p_str);
    virtual void Flush();

    void Destroy();
};

#pragma pack(pop)

#endif