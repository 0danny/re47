#include "zrbtree.h"

ZRBTree::ZRBTree()
{
    SBinTreeNode **l_dummy = &m_dummyNode;

    // Unused atm
    m_unkByte = 0;
    m_unkInt5 = 0;
    m_unkInt3 = 0;
    m_unkInt4 = 0;

    // Compilers very smart way of setting all of the nodes to 0 lmao
    *l_dummy = 0;

    m_nullNode = (SBinTreeNode *)l_dummy;
    m_rootNode = (SBinTreeNode *)l_dummy;
}

// MATCHED
void ZRBTree::Insert(SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_tempNode;
    SBinTreeNode *l_tempNode2;
    SBinTreeNode *l_tempNode3;
    SBinTreeNode *l_tempNode4;
    SBinTreeNode *l_tempNode5;
    SBinTreeNode *l_tempNode6;
    SBinTreeNode *l_tempNode7;
    SBinTreeNode *l_tempNode8;
    SBinTreeNode *l_tempNode9;
    SBinTreeNode *l_tempNode10;
    SBinTreeNode *l_tempNode11;
    SBinTreeNode *l_rootNode;

    SBinTreeNode *l_innerRightNode;
    SBinTreeNode *l_parentNode;
    SBinTreeNode *l_parentLeftNode;
    SBinTreeNode *l_currentNode = p_binNode;

    BaseInsert(p_binNode);

    p_binNode->color = 1;

    if (p_binNode == m_rootNode)
    {
        m_rootNode->color = 0;
        return;
    }

    while (1)
    {
        l_parentNode = l_currentNode->parent;

        if (l_currentNode->parent->color != 1)
            break;

        l_parentLeftNode = l_parentNode->parent->left;

        if (l_parentNode == l_parentLeftNode)
        {
            l_parentLeftNode = l_parentNode->parent->right;

            if (l_parentLeftNode->color != 1)
            {
                if (l_currentNode == l_parentNode->right)
                {
                    l_currentNode = l_currentNode->parent;
                    l_innerRightNode = l_parentNode->right;
                    l_currentNode->right = l_innerRightNode->left;

                    l_tempNode = l_innerRightNode->left;

                    if (l_tempNode != m_nullNode)
                        l_tempNode->parent = l_currentNode;

                    l_innerRightNode->parent = l_currentNode->parent;
                    l_tempNode2 = l_currentNode->parent;

                    if (l_currentNode->parent == m_nullNode)
                    {
                        m_rootNode = l_innerRightNode;
                    }
                    else if (l_currentNode == l_tempNode2->left)
                    {
                        l_tempNode2->left = l_innerRightNode;
                    }
                    else
                    {
                        l_tempNode2->right = l_innerRightNode;
                    }

                    l_innerRightNode->left = l_currentNode;
                    l_currentNode->parent = l_innerRightNode;
                }

                l_currentNode->parent->color = 0;
                l_currentNode->parent->parent->color = 1;

                l_tempNode3 = l_currentNode->parent->parent;

                l_tempNode4 = l_tempNode3->left;
                l_tempNode3->left = l_tempNode4->right;
                l_tempNode5 = l_tempNode4->right;

                if (l_tempNode5 != m_nullNode)
                    l_tempNode5->parent = l_tempNode3;

                l_tempNode4->parent = l_tempNode3->parent;
                l_tempNode6 = l_tempNode3->parent;

                if (l_tempNode3->parent == m_nullNode)
                {
                    m_rootNode = l_tempNode4;
                    l_tempNode4->right = l_tempNode3;
                }
                else
                {
                    if (l_tempNode3 == l_tempNode6->right)
                        l_tempNode6->right = l_tempNode4;
                    else
                        l_tempNode6->left = l_tempNode4;

                    l_tempNode4->right = l_tempNode3;
                }

            ADJUST_PARENT:
                l_tempNode3->parent = l_tempNode4;
                goto CHECK_ROOT;
            }
        }
        else if (l_parentLeftNode->color != 1)
        {
            if (l_currentNode == l_parentNode->left)
            {
                l_currentNode = l_currentNode->parent;
                l_tempNode7 = l_parentNode->left;
                l_currentNode->left = l_tempNode7->right;
                l_tempNode8 = l_tempNode7->right;

                if (l_tempNode8 != m_nullNode)
                    l_tempNode8->parent = l_currentNode;

                l_tempNode7->parent = l_currentNode->parent;
                l_tempNode9 = l_currentNode->parent;

                if (l_currentNode->parent == m_nullNode)
                {
                    m_rootNode = l_tempNode7;
                }
                else if (l_currentNode == l_tempNode9->right)
                {
                    l_tempNode9->right = l_tempNode7;
                }
                else
                {
                    l_tempNode9->left = l_tempNode7;
                }

                l_tempNode7->right = l_currentNode;
                l_currentNode->parent = l_tempNode7;
            }

            l_currentNode->parent->color = 0;
            l_currentNode->parent->parent->color = 1;

            l_tempNode3 = l_currentNode->parent->parent;
            l_tempNode4 = l_tempNode3->right;
            l_tempNode3->right = l_tempNode4->left;
            l_tempNode10 = l_tempNode4->left;

            if (l_tempNode10 != m_nullNode)
                l_tempNode10->parent = l_tempNode3;

            l_tempNode4->parent = l_tempNode3->parent;
            l_tempNode11 = l_tempNode3->parent;

            if (l_tempNode3->parent == m_nullNode)
            {
                m_rootNode = l_tempNode4;
            }
            else if (l_tempNode3 == l_tempNode11->left)
            {
                l_tempNode11->left = l_tempNode4;
            }
            else
            {
                l_tempNode11->right = l_tempNode4;
            }
            l_tempNode4->left = l_tempNode3;
            goto ADJUST_PARENT;
        }

        l_parentNode->color = 0;
        l_parentLeftNode->color = 0;
        l_currentNode->parent->parent->color = 1;

        l_currentNode = l_currentNode->parent->parent;

    CHECK_ROOT:
        l_rootNode = m_rootNode;

        if (l_currentNode == l_rootNode)
        {
            l_rootNode->color = 0;
            return;
        }
    }

    m_rootNode->color = 0;
}

// MATCHED
SBinTreeNode *ZRBTree::Delete(SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_curNode;

    if (p_binNode->left == m_nullNode || p_binNode->right == m_nullNode)
        l_curNode = p_binNode;
    else
        l_curNode = Successor(p_binNode);

    SBinTreeNode *l_curLeft = l_curNode->left;

    if (l_curLeft == m_nullNode)
        l_curLeft = l_curNode->right;

    l_curLeft->parent = l_curNode->parent;

    SBinTreeNode *l_curParent = l_curNode->parent;

    if (l_curNode->parent == m_nullNode)
    {
        m_rootNode = l_curLeft;
    }
    else if (l_curNode == l_curParent->left)
    {
        l_curParent->left = l_curLeft;
    }
    else
    {
        l_curParent->right = l_curLeft;
    }

    if (l_curNode != p_binNode)
    {
        p_binNode->key = l_curNode->key;

        CopyData(p_binNode, l_curNode);
    }

    if (!l_curNode->color)
        RBDeleteFixup(l_curLeft);

    return l_curNode == m_nullNode ? 0 : l_curNode;
}

// MATCHED
SBinTreeNode *ZRBTree::Search(i32 p_key, SBinTreeNode *p_binNode)
{
    i32 l_curKey;
    SBinTreeNode *i;
    SBinTreeNode *l_binNode = p_binNode;

    if (!p_binNode)
        l_binNode = m_rootNode;

    for (i = m_nullNode; l_binNode != i; l_binNode = p_key >= l_curKey ? l_binNode->right : l_binNode->left)
    {
        l_curKey = l_binNode->key;

        if (p_key == l_curKey)
            break;
    }

    return l_binNode == i ? 0 : l_binNode;
}

// MATCHED
SBinTreeNode *ZRBTree::SearchLargerOrSame(i32 p_key, SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_result;
    SBinTreeNode *l_curNode = p_binNode;

    if (!p_binNode)
    {
        l_curNode = m_rootNode;

        if (!l_curNode)
            return 0;
    }

    if (l_curNode->key <= p_key)
    {
        if (l_curNode->key >= p_key)
            return l_curNode == m_nullNode ? 0 : l_curNode;

        SBinTreeNode *l_right = l_curNode->right;

        if (!l_right)
            return 0;

        l_result = SearchLargerOrSame(p_key, l_right);

        if (!l_result || p_key > l_result->key)
            return 0;
    }
    else
    {
        if (!l_curNode->left)
            return l_curNode == m_nullNode ? 0 : l_curNode;

        l_result = SearchLargerOrSame(p_key, l_curNode->left);

        if (!l_result || p_key > l_result->key)
            return l_curNode == m_nullNode ? 0 : l_curNode;
    }

    return l_result;
}

// MATCHED
SBinTreeNode *ZRBTree::Minimum(SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_result = p_binNode;
    SBinTreeNode *l_left = p_binNode->left;

    for (SBinTreeNode *i = m_nullNode; l_left != i; l_left = l_left->left)
        l_result = l_left;

    return l_result;
}

// MATCHED
SBinTreeNode *ZRBTree::Maximum(SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_result = p_binNode;
    SBinTreeNode *l_right = p_binNode->right;

    for (SBinTreeNode *i = m_nullNode; l_right != i; l_right = l_right->right)
        l_result = l_right;

    return l_result;
}

// MATCHED
SBinTreeNode *ZRBTree::Successor(SBinTreeNode *p_binNode)
{
    if (p_binNode->right != m_nullNode)
        return Minimum(p_binNode->right);

    SBinTreeNode *result;
    SBinTreeNode *l_curNode = p_binNode;

    for (result = p_binNode->parent; result != m_nullNode; result = result->parent)
    {
        if (l_curNode != result->right)
            break;

        l_curNode = result;
    }

    return result;
}

// MATCHED
i32 ZRBTree::Depth(SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_binNode = p_binNode;

    if (!p_binNode)
        l_binNode = m_rootNode;

    i32 l_depth1 = 1;
    i32 l_depth2 = 1;

    if (l_binNode->left != m_nullNode)
        l_depth1 = Depth(l_binNode->left) + 1;

    if (l_binNode->right != m_nullNode)
        l_depth2 = Depth(l_binNode->right) + 1;

    if (l_depth1 >= l_depth2)
        return l_depth1;

    return l_depth2;
}

// MATCHED
SBinTreeNode *ZRBTree::CheckLinks(SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_binNode = p_binNode;

    if (!p_binNode)
    {
        l_binNode = m_rootNode;
        if (!l_binNode)
        {
            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 175);
            l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 175);

            DebugBreak();
        }
    }

    SBinTreeNode *l_result = m_nullNode;

    if (l_binNode != l_result)
    {
        SBinTreeNode *l_left = l_binNode->left;

        if (l_left != l_result)
        {
            if (l_left->parent != l_binNode)
            {
                ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 179);
                l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 179);

                DebugBreak();
            }

            CheckLinks(l_binNode->left);
        }

        l_result = l_binNode->right;

        if (l_result != m_nullNode)
        {
            if (l_result->parent != l_binNode)
            {
                ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 184);
                l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\BinaryTrees.cpp", 184);

                DebugBreak();
            }

            return CheckLinks(l_binNode->right);
        }
    }

    return l_result;
}

// NULLSUB, matches with debug symbols and VT calls
void ZRBTree::CopyData(SBinTreeNode *p_binNode, SBinTreeNode *p_binNode2)
{
    return;
}

// MATCHED
void ZRBTree::BaseInsert(SBinTreeNode *p_binNode)
{
    p_binNode->left = m_nullNode;
    p_binNode->right = m_nullNode;
    p_binNode->parent = m_nullNode;

    SBinTreeNode *l_rootNode = m_rootNode;
    SBinTreeNode *l_curNode = m_nullNode;

    while (l_rootNode != m_nullNode)
    {
        l_curNode = l_rootNode;

        if (p_binNode->key >= l_rootNode->key)
            l_rootNode = l_rootNode->right;
        else
            l_rootNode = l_rootNode->left;
    }

    p_binNode->parent = l_curNode;

    if (l_curNode == m_nullNode)
    {
        m_rootNode = p_binNode;
    }
    else if (p_binNode->key >= l_curNode->key)
    {
        l_curNode->right = p_binNode;
    }
    else
    {
        l_curNode->left = p_binNode;
    }
}

// MATCHED
void ZRBTree::RBDeleteFixup(SBinTreeNode *p_binNode)
{
    SBinTreeNode *l_currentNode;
    SBinTreeNode *l_parentNode;
    SBinTreeNode *l_siblingNode;
    SBinTreeNode *l_tempNode;
    SBinTreeNode *l_leftChild;
    SBinTreeNode *l_rightChild;
    SBinTreeNode *l_siblingLeft;
    SBinTreeNode *l_siblingRight;
    SBinTreeNode *l_tempLeft;
    SBinTreeNode *l_tempRight;

    l_currentNode = p_binNode;
    if (p_binNode != m_rootNode)
    {
        while (1)
        {
            if (l_currentNode->color)
            {
            FIXUP_DONE:
                l_currentNode->color = 0;
                return;
            }

            l_parentNode = l_currentNode->parent;
            l_siblingNode = l_currentNode->parent->left;

            if (l_currentNode == l_siblingNode)
            {
                l_siblingNode = l_parentNode->right;

                if (l_siblingNode->color == 1)
                {
                    l_siblingNode->color = 0;
                    l_currentNode->parent->color = 1;
                    l_tempNode = l_currentNode->parent;
                    l_rightChild = l_currentNode->parent->right;
                    l_currentNode->parent->right = l_rightChild->left;
                    l_siblingRight = l_rightChild->left;

                    if (l_siblingRight != m_nullNode)
                        l_siblingRight->parent = l_tempNode;

                    l_rightChild->parent = l_tempNode->parent;
                    l_siblingRight = l_tempNode->parent;

                    if (l_tempNode->parent == m_nullNode)
                    {
                        m_rootNode = l_rightChild;
                    }
                    else if (l_tempNode == l_siblingRight->left)
                    {
                        l_siblingRight->left = l_rightChild;
                    }
                    else
                    {
                        l_siblingRight->right = l_rightChild;
                    }

                    l_rightChild->left = l_tempNode;
                    l_tempNode->parent = l_rightChild;
                    l_parentNode = l_currentNode->parent;
                    l_siblingNode = l_currentNode->parent->right;
                }

                if (l_siblingNode->left->color || l_siblingNode->right->color)
                {
                    if (!l_siblingNode->right->color)
                    {
                        l_siblingNode->left->color = 0;
                        l_siblingLeft = l_siblingNode->left;
                        l_siblingNode->color = 1;
                        l_siblingNode->left = l_siblingLeft->right;
                        l_siblingRight = l_siblingLeft->right;

                        if (l_siblingRight != m_nullNode)
                            l_siblingRight->parent = l_siblingNode;

                        l_siblingLeft->parent = l_siblingNode->parent;
                        l_tempRight = l_siblingNode->parent;

                        if (l_siblingNode->parent == m_nullNode)
                        {
                            m_rootNode = l_siblingLeft;
                        }
                        else if (l_siblingNode == l_tempRight->right)
                        {
                            l_tempRight->right = l_siblingLeft;
                        }
                        else
                        {
                            l_tempRight->left = l_siblingLeft;
                        }

                        l_siblingLeft->right = l_siblingNode;
                        l_siblingNode->parent = l_siblingLeft;
                        l_parentNode = l_currentNode->parent;
                        l_siblingNode = l_currentNode->parent->right;
                    }

                    l_siblingNode->color = l_parentNode->color;
                    l_currentNode->parent->color = 0;
                    l_siblingNode->right->color = 0;
                    l_tempNode = l_currentNode->parent;
                    l_leftChild = l_tempNode->right;
                    l_tempNode->right = l_leftChild->left;
                    l_tempLeft = l_leftChild->left;

                    if (l_tempLeft != m_nullNode)
                        l_tempLeft->parent = l_tempNode;

                    l_leftChild->parent = l_tempNode->parent;
                    l_siblingRight = l_tempNode->parent;

                    if (l_tempNode->parent == m_nullNode)
                    {
                        m_rootNode = l_leftChild;
                        l_leftChild->left = l_tempNode;
                    }
                    else
                    {
                        if (l_tempNode == l_siblingRight->left)
                            l_siblingRight->left = l_leftChild;
                        else
                            l_siblingRight->right = l_leftChild;

                        l_leftChild->left = l_tempNode;
                    }

                ADJUST_PARENT_RIGHT:
                    l_tempNode->parent = l_leftChild;
                    l_currentNode = m_rootNode;
                    goto FIXUP_COMPLETE;
                }

                l_siblingNode->color = 1;
                l_currentNode = l_currentNode->parent;
            }
            else
            {
                if (l_siblingNode->color == 1)
                {
                    l_siblingNode->color = 0;
                    l_currentNode->parent->color = 1;
                    l_tempNode = l_currentNode->parent;
                    l_leftChild = l_currentNode->parent->left;
                    l_currentNode->parent->left = l_leftChild->right;
                    l_siblingLeft = l_leftChild->right;

                    if (l_siblingLeft != m_nullNode)
                        l_siblingLeft->parent = l_tempNode;

                    l_leftChild->parent = l_tempNode->parent;
                    l_tempLeft = l_tempNode->parent;

                    if (l_tempNode->parent == m_nullNode)
                    {
                        m_rootNode = l_leftChild;
                    }
                    else if (l_tempNode == l_tempLeft->right)
                    {
                        l_tempLeft->right = l_leftChild;
                    }
                    else
                    {
                        l_tempLeft->left = l_leftChild;
                    }

                    l_leftChild->right = l_tempNode;
                    l_tempNode->parent = l_leftChild;
                    l_parentNode = l_currentNode->parent;
                    l_siblingNode = l_currentNode->parent->left;
                }

                if (l_siblingNode->right->color || l_siblingNode->left->color)
                {
                    if (!l_siblingNode->left->color)
                    {
                        l_siblingNode->right->color = 0;
                        l_siblingRight = l_siblingNode->right;
                        l_siblingNode->color = 1;
                        l_siblingNode->right = l_siblingRight->left;
                        l_siblingLeft = l_siblingRight->left;

                        if (l_siblingLeft != m_nullNode)
                            l_siblingLeft->parent = l_siblingNode;

                        l_siblingRight->parent = l_siblingNode->parent;
                        l_tempRight = l_siblingNode->parent;

                        if (l_siblingNode->parent == m_nullNode)
                        {
                            m_rootNode = l_siblingRight;
                        }
                        else if (l_siblingNode == l_tempRight->left)
                        {
                            l_tempRight->left = l_siblingRight;
                        }
                        else
                        {
                            l_tempRight->right = l_siblingRight;
                        }

                        l_siblingRight->left = l_siblingNode;
                        l_siblingNode->parent = l_siblingRight;
                        l_parentNode = l_currentNode->parent;
                        l_siblingNode = l_currentNode->parent->left;
                    }

                    l_siblingNode->color = l_parentNode->color;
                    l_currentNode->parent->color = 0;
                    l_siblingNode->left->color = 0;
                    l_tempNode = l_currentNode->parent;
                    l_leftChild = l_tempNode->left;
                    l_tempNode->left = l_leftChild->right;
                    l_tempRight = l_leftChild->right;

                    if (l_tempRight != m_nullNode)
                        l_tempRight->parent = l_tempNode;

                    l_leftChild->parent = l_tempNode->parent;
                    l_tempLeft = l_tempNode->parent;

                    if (l_tempNode->parent == m_nullNode)
                    {
                        m_rootNode = l_leftChild;
                    }
                    else if (l_tempNode == l_tempLeft->right)
                    {
                        l_tempLeft->right = l_leftChild;
                    }
                    else
                    {
                        l_tempLeft->left = l_leftChild;
                    }

                    l_leftChild->right = l_tempNode;
                    goto ADJUST_PARENT_RIGHT;
                }

                l_siblingNode->color = 1;
                l_currentNode = l_currentNode->parent;
            }

        FIXUP_COMPLETE:
            if (l_currentNode == m_rootNode)
                goto FIXUP_DONE;
        }
    }

    p_binNode->color = 0;
}
