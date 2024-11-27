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

#pragma pack(push, 1)

class ZMalloc
{
public:
    ZMalloc();
    ~ZMalloc();

    i32 m_unkInt0;        // 0
    i32 m_unkInt1;        // 4
    i32 m_nodeArray[116]; // 8
    i32 m_unkInt2;        // 472
    i32 m_unkInt3;        // 476
    ZRBTree m_rbTree1;    // 480
    u8 m_unkByte1;        // 509
    ZRBTree m_rbTree2;    // 510
    u8 m_unkByte2;        // 539

    void RemoveFreeHeaderFromBins(SMallocFreeHeader *p_freeHeader, SBinTreeNode *p_mallocBin);
    inline void FreeMallocBin(SBinTreeNode *p_mallocBin);
    inline void MarkFree(void *p_marked);
}; // 540 in size

#pragma pack(pop)

#endif