#pragma once

#include "reftab.h"

// not working, see (0FFB1C9D) something to do with virtual dispatch not being correct.
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
