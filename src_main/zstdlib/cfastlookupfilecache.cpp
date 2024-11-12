#include "cfastlookupfilecache.h"

CFastLookupFileCache::~CFastLookupFileCache()
{
    Destroy();
}

void CFastLookupFileCache::Destroy()
{
    RefRun l_refRun;

    m_fastLookup.Reset();

    RefTab32 *l_refTab32 = &m_refTab32;

    if (l_refTab32)
    {
        l_refTab32->RunInitNxtRef(&l_refRun);

        for (u32 l_item = l_refTab32->RunNxtRef(&l_refRun); l_refRun.prev; l_item = l_refTab32->RunNxtRef(&l_refRun))
        {
            delete (u32 *)l_item;
        }
    }

    m_refTab32.Destroy();
    m_fastLookup.Destroy();
}

void CFastLookupFileCache::Add(const char *p_str, IOZip_LocalFileHeader *p_localFileHeader, u32 p_data)
{
    CFileCache *l_alloc = new CFileCache;

    u32 l_refNum = (u32)l_alloc;

    memcpy(l_alloc, p_localFileHeader, sizeof(IOZip_LocalFileHeader)); // File header is 26 bytes.

    l_alloc->data = p_data;

    m_fastLookup.SetLowerCase(p_str, (u32)l_alloc);
    m_refTab32.Add(l_refNum);
}

boolean CFastLookupFileCache::Lookup(const char *p_str, IOZip_LocalFileHeader *p_localFileHeader, u32 *p_result)
{
    CFileCache *l_header = (CFileCache *)m_fastLookup.GetLowerCase(p_str);

    if (!l_header)
        return FALSE;

    *p_result = l_header->data;

    memcpy(p_localFileHeader, l_header, sizeof(IOZip_LocalFileHeader));

    return TRUE;
}

void CFastLookupFileCache::Invalidate(const char *p_str)
{
    m_fastLookup.RemoveLowerCase(p_str, 0);
}

void CFastLookupFileCache::Flush()
{
    RefRun l_refRun;

    m_fastLookup.Reset();

    RefTab32 *l_refTab32 = &m_refTab32;

    if (l_refTab32)
    {
        l_refTab32->RunInitNxtRef(&l_refRun);

        for (u32 l_item = l_refTab32->RunNxtRef(&l_refRun); l_refRun.prev; l_item = l_refTab32->RunNxtRef(&l_refRun))
        {
            delete (u32 *)l_item;
        }
    }
}
