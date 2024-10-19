#pragma once

#include "system/zsyscom.h"

#pragma pack(push, 1)

// 17 in size (not 17 because of data, but 17 because of the struct size)
struct SBinTreeNode
{
    SBinTreeNode *parent;
    SBinTreeNode *left;
    SBinTreeNode *right;
    i32 key;
    u8 color;
    i32 data;
};

#pragma pack(pop)

#pragma pack(push, 2) // m_unkByte is apparently a u8, so we need to align it to 2 bytes

// 30 in size
class ZRBTree
{
public:
    SBinTreeNode *m_rootNode;
    SBinTreeNode *m_nullNode;
    SBinTreeNode *m_dummyNode;

    // TODO: Fix names
    i32 m_unkInt3;
    i32 m_unkInt4;
    i32 m_unkInt5;
    u8 m_unkByte;

    ZRBTree();

    virtual void Insert(SBinTreeNode *p_binNode);
    virtual SBinTreeNode *Delete(SBinTreeNode *p_binNode);

    virtual SBinTreeNode *Search(i32 p_key, SBinTreeNode *p_binNode);
    virtual SBinTreeNode *SearchLargerOrSame(i32 p_key, SBinTreeNode *p_binNode);

    virtual SBinTreeNode *Minimum(SBinTreeNode *p_binNode);
    virtual SBinTreeNode *Maximum(SBinTreeNode *p_binNode);
    virtual SBinTreeNode *Successor(SBinTreeNode *p_binNode);

    virtual i32 Depth(SBinTreeNode *p_binNode);
    virtual SBinTreeNode *CheckLinks(SBinTreeNode *p_binNode);
    virtual void CopyData(SBinTreeNode *p_binNode, SBinTreeNode *p_binNode2);

    void BaseInsert(SBinTreeNode *p_binNode);
    void RBDeleteFixup(SBinTreeNode *p_binNode);
};

#pragma pack(pop)
