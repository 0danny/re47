#ifndef ZVALTREE_H
#define ZVALTREE_H

#include "zrbtree.h"
#include "zstdlib/reftab/staticreftab.h"

#pragma pack(push, 1)

// 33 in size
class ZValTree : public ZRBTree
{
public:
    ZValTree(i32 p_poolSize);
    ~ZValTree();

    StaticRefTab *m_refTab;

    // Overriden
    SBinTreeNode *Delete(SBinTreeNode *p_binNode) override;
    void CopyData(SBinTreeNode *p_binNode, SBinTreeNode *p_binNode2) override;

    // Added
    virtual void InsertKey(i32 p_key, i32 p_data);
    virtual void DeleteKey(i32 p_key);
    virtual i32 GetKeyVal(i32 p_key);
};

#pragma pack(pop)

#endif