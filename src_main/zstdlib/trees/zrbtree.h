#ifndef ZRBTREE_H
#define ZRBTREE_H

#include "common.h"
#include "system/zsyscom.h"

#pragma pack(push, 1)

struct SBinTreeNode
{
    SBinTreeNode *parent; // 0
    SBinTreeNode *left;   // 4
    SBinTreeNode *right;  // 8
    i32 key;              // 12
    u8 color;             // 16
    i32 data[];           // 17
}; // 17 in size

class ZRBTree
{
public:
    SBinTreeNode *m_rootNode;  // 4
    SBinTreeNode *m_nullNode;  // 8
    SBinTreeNode *m_dummyNode; // 12

    i32 m_unkInt3; // 16
    i32 m_unkInt4; // 20
    i32 m_unkInt5; // 24
    u8 m_unkByte;  // 28

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
}; // 29 in size

#pragma pack(pop)

#endif