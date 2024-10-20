#ifndef EQUREFTAB_H
#define EQUREFTAB_H

#include "reftab.h"

class EquRefTab : public RefTab
{
public:
    EquRefTab(i32 p_poolSize, i32 p_size);

    virtual ~EquRefTab();

    // Overriden
    void PrintStatus();

    // Added
    virtual u32 *FindEqu(i32 p_value);
    virtual u32 *AddEqu(u32 p_refNum, i32 p_value);

    void Destroy();
};

#endif