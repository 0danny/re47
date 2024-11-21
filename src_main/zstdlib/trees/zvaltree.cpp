#include "zvaltree.h"

ZValTree::ZValTree(i32 p_poolSize) : ZRBTree()
{
    m_refTab = new StaticRefTab(p_poolSize, 5);
}

ZValTree::~ZValTree()
{
    SBinTreeNode *l_result;
    SBinTreeNode *l_nodeDeleted;

    while (1)
    {
        l_result = m_rootNode;

        if (l_result == m_nullNode || !l_result)
            break;

        l_nodeDeleted = ZRBTree::Delete(m_rootNode);

        m_refTab->DelRefPtr((u32 *)l_nodeDeleted);

        m_refTab->DelRefPtr(0);
    }

    if (m_refTab)
        m_refTab->~StaticRefTab();
}

SBinTreeNode *ZValTree::Delete(SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_deleteRes = ZRBTree::Delete(p_binNode);
    m_refTab->DelRefPtr((u32 *)l_deleteRes);

    return 0;
}

void ZValTree::CopyData(SBinTreeNode *p_binNode, SBinTreeNode *p_binNode2)
{
    p_binNode->data = p_binNode2->data;
}

void ZValTree::InsertKey(i32 p_key, i32 p_data)
{
    SBinTreeNode *l_dataPtr = (SBinTreeNode *)(m_refTab->Add(0) - 1);

    l_dataPtr->key = p_key;
    l_dataPtr->data = p_data;

    Insert(l_dataPtr);
}

void ZValTree::DeleteKey(i32 p_key)
{
    SBinTreeNode *l_searchRes = Search(p_key, 0);

    if (l_searchRes)
    {
        SBinTreeNode *l_ptr = Delete(l_searchRes);

        m_refTab->DelRefPtr((u32 *)l_ptr);
    }
}

i32 ZValTree::GetKeyVal(i32 p_key)
{
    SBinTreeNode *l_searchRes = Search(p_key, 0);

    if (l_searchRes)
        return l_searchRes->data;
    else
        return 0;
}
