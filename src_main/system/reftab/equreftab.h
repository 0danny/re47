#pragma once

#include "reftab.h"

class EquRefTab : public RefTab
{
public:
    EquRefTab(int p_poolSize, int p_size);

    virtual ~EquRefTab();

    // Overriden
    void PrintStatus();

    // Added
    virtual UINT *FindEqu(int p_value);
    virtual UINT *AddEqu(UINT p_refNum, int p_value);

    void Destroy();
};