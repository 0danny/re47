#ifndef ZMALLOC_H
#define ZMALLOC_H

#include "zstdlib/trees/zrbtree.h"

struct SMallocFreeHeader
{
    i32 size;
    i32 unkInt1;
    SBinTreeNode *right;
    SBinTreeNode *left;
};

struct SMallocBlockHeader
{
    u32 checksumComponents[4];
    u32 checksum;
    u32 unkInt;
    u32 flag;
    u32 referenceNumber;
};

#pragma pack(push, 2)

// 33 in size
class ZMalloc
{
public:
    ZMalloc();
    ~ZMalloc();

    i32 m_unkInt0;
    i32 m_unkInt1;
    i32 m_nodeArray[116];
    i32 m_unkInt2;
    i32 m_unkInt3;
    ZRBTree m_rbTree1;
    ZRBTree m_rbTree2;

    void RemoveFreeHeaderFromBins(SMallocFreeHeader *p_freeHeader, SBinTreeNode *p_mallocBin);
    inline void FreeMallocBin(SBinTreeNode *p_mallocBin);
    inline void MarkFree(void *p_marked);
};

#pragma pack(pop)

#endif