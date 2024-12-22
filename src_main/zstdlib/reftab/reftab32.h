#ifndef REFTAB32_H
#define REFTAB32_H

#include "reftab.h"
#include "common.h"

struct RefRun32
{
    RefRun32 *prev; // 0
    RefRun32 *next; // 4
    i32 usedUnits;  // 8
    u32 data[38];   // 12
}; // 164 in size.

class RefTab32 : public RefTab
{
public:
    RefRun32 m_refRun; // 4

    RefTab32();
    virtual ~RefTab32();

    // Overriden
    virtual void DeleteBlock(void *p_lpMem) override;
    virtual RefRun *NewBlock() override;

    void Destroy();
}; // 168 in size.

// TODO: 168 in size, as confirmed by enginedata.dll (0x0FF6DEFF)

#endif