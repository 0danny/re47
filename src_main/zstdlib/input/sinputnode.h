#ifndef SINPUTNODE_H
#define SINPUTNODE_H

#include "common.h"
#include "zstdlib/reftab/reftab.h"

class ZInputActions;

#pragma pack(push, 1)

class SInputNode
{
public:
    SInputNode()
    {
        m_refTab = new RefTab(4, 1);
        m_isToggleNode = 0;
    }

    ~SInputNode()
    {
        if (m_refTab)
            m_refTab->~RefTab();
    }

    ZInputActions *m_parent; // 0
    RefTab *m_refTab;        // 4
    u8 m_isToggleNode;       // 8

}; // 9 in size.

#pragma pack(pop)

#endif