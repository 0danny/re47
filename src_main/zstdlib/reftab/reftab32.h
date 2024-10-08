#pragma once

#include "reftab.h"

class RefTab32 : public RefTab
{
public:
    RefRun m_refRun;

    RefTab32();
    virtual ~RefTab32();

    // Overriden
    void DeleteBlock(void *p_lpMem);
    RefRun *NewBlock();

    void Destroy();
};
