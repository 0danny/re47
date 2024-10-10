#pragma once

#include "reftab.h"

// 24 in size.
class AllocRefTab
{
public:
    u32 *m_array;        // 4
    RefTab *m_refTab;    // 8
    i32 m_nextFreeRef;   // 12
    i32 m_maxRefIndex;   // 16
    i32 m_refShitAmount; // 20

    AllocRefTab();
    virtual ~AllocRefTab();

    virtual u32 NewRef();
    virtual boolean FreeRef(u32 p_refNum);
    virtual boolean CheckRefActive(u32 p_refNum);
    virtual void PrintRef(u32 p_refNum);
};
