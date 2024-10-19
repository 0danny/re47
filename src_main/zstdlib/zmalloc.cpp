#include "zmalloc.h"

ZMalloc::ZMalloc()
{
}

ZMalloc::~ZMalloc()
{
}

void ZMalloc::RemoveFreeHeaderFromBins(SMallocFreeHeader *p_freeHeader, SBinTreeNode *p_mallocBin)
{
    if (p_freeHeader->size < 0)
    {
        ZSysCom *l_com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 274);
        l_com->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 274);
        DebugBreak();
    }

    SBinTreeNode *l_searchNode = p_mallocBin;

    if (!p_mallocBin)
        l_searchNode = m_rbTree1.Search(p_freeHeader->size, 0);

    SBinTreeNode *l_rightNode = p_freeHeader->right;

    if (l_rightNode)
        l_rightNode->key = (int)p_freeHeader->left;
    else
        l_searchNode->data = (int)p_freeHeader->left;

    SBinTreeNode *l_leftNode = p_freeHeader->left;

    if (l_leftNode)
        l_leftNode->right = p_freeHeader->right;

    if (!l_searchNode->data)
    {
        SBinTreeNode *l_deletedNode = m_rbTree1.Delete(l_searchNode);

        FreeMallocBin(l_deletedNode);
    }
}

inline void ZMalloc::FreeMallocBin(SBinTreeNode *p_mallocBin)
{
    if (m_unkInt2 == 100)
    {
        MarkFree((void *)p_mallocBin);
    }
    else
    {
        m_nodeArray[m_unkInt2 + 16] = (int)p_mallocBin;
        ++m_unkInt2;
    }
}

inline void ZMalloc::MarkFree(void *p_marked)
{
    free(p_marked);

    SBinTreeNode *l_searchRes = m_rbTree2.Search((int)p_marked, 0);

    if (!l_searchRes)
    {
        ZSysCom *l_com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 56);
        l_com->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\ZMalloc.cpp", 56);
        DebugBreak();
    }

    SBinTreeNode *deleted = m_rbTree1.Delete(l_searchRes);

    free(deleted);
}
