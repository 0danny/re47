#ifndef REFTAB32_H
#define REFTAB32_H

#include "reftab.h"

class RefTab32 : public RefTab
{
public:
    RefRun m_refRun;

    RefTab32();
    virtual ~RefTab32();

    // Overriden
    virtual void DeleteBlock(void *p_lpMem) override;
    virtual RefRun *NewBlock() override;

    void Destroy();
};
// TODO: 168 in size, as confirmed by enginedata.dll (0x0FF6DEFF)

#endif