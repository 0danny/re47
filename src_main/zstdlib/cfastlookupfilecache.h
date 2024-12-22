#ifndef CFASTLOOKUPFILECACHE_H
#define CFASTLOOKUPFILECACHE_H

#include "common.h"
#include "cfastlookup2.h"
#include "reftab/reftab32.h"

#pragma pack(push, 1)

struct IOZip_LocalFileHeader
{
    u16 versionNeeded;     // 0
    u16 generalPurpose;    // 2
    u16 compressionMethod; // 4
    u16 lastModTime;       // 6
    u16 lastModDate;       // 8
    u32 crc32;             // 10
    u32 compressedSize;    // 14
    u32 uncompressedSize;  // 18
    u16 filenameLength;    // 22
    u16 extraFieldLength;  // 24
}; // 26 in size.

struct CFileCache
{
    IOZip_LocalFileHeader fileHeader;
    u32 data;
};

class CFastLookupFileCache
{
public:
    CFastLookup2 m_fastLookup; // 4
    RefTab32 m_refTab32;       // 8

    inline CFastLookupFileCache() : m_fastLookup(32), m_refTab32() {}

    virtual ~CFastLookupFileCache();
    virtual void Add(const char *p_str, IOZip_LocalFileHeader *p_localFileHeader, i32 p_data);
    virtual bool Lookup(const char *p_str, IOZip_LocalFileHeader *p_localFileHeader, i32 *p_result);
    virtual void Invalidate(const char *p_str);
    virtual void Flush();

    void Destroy();

}; // 176 in size.

#pragma pack(pop)

#endif