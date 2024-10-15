#include "zvaltree.h"

ZValTree::ZValTree(int p_poolSize) : ZRBTree()
{
    m_refTab = new StaticRefTab(p_poolSize, 5);
}

ZValTree::~ZValTree()
{
    SBinTreeNode *l_result;
    SBinTreeNode *l_nodeDeleted;

    while (1)
    {
        l_result = this->m_rootNode;

        if (l_result == this->m_nullNode || !l_result)
            break;

        l_nodeDeleted = ZRBTree::Delete(this->m_rootNode);

        m_refTab->DelRefPtr((UINT *)l_nodeDeleted);

        this->m_refTab->DelRefPtr(0);
    }

    if (m_refTab)
        m_refTab->~StaticRefTab();
}

SBinTreeNode *ZValTree::Delete(SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_deleteRes = ZRBTree::Delete(p_binNode);
    m_refTab->DelRefPtr((UINT *)l_deleteRes);

    return 0;
}

void ZValTree::CopyData(SBinTreeNode *p_binNode, SBinTreeNode *p_binNode2)
{
    p_binNode[1].parent = p_binNode2[1].parent;
}

void ZValTree::InsertKey(int p_key, int p_data)
{
    SBinTreeNode *l_dataPtr = (SBinTreeNode *)(this->m_refTab->Add(0) - 1);

    l_dataPtr->key = p_key;

    l_dataPtr[1].parent = (SBinTreeNode *)p_data;

    this->Insert(l_dataPtr);
}

void ZValTree::DeleteKey(int p_key)
{
    SBinTreeNode *l_searchRes = this->Search(p_key, 0);

    if (l_searchRes)
    {
        SBinTreeNode *l_ptr = this->Delete(l_searchRes);

        m_refTab->DelRefPtr((UINT *)l_ptr);
    }
}

SBinTreeNode *ZValTree::GetKeyVal(int p_key)
{
    SBinTreeNode *l_searchRes = this->Search(p_key, 0);

    if (l_searchRes)
        return l_searchRes[1].parent;
    else
        return 0;
}
