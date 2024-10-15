#include "zrbtree.h"
#include "zstdlib/reftab/staticreftab.h"

#pragma pack(push, 1)

// 33 in size
class ZValTree : public ZRBTree
{
public:
    ZValTree(int p_poolSize);
    ~ZValTree();

    StaticRefTab *m_refTab;

    // Overriden
    SBinTreeNode *Delete(SBinTreeNode *p_binNode);
    void CopyData(SBinTreeNode *p_binNode, SBinTreeNode *p_binNode2);

    // Added
    virtual void InsertKey(int p_key, int p_data);
    virtual void DeleteKey(int p_key);
    virtual SBinTreeNode *GetKeyVal(int p_key);
};

#pragma pack(pop)
