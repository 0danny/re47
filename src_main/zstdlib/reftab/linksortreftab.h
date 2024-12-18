#ifndef LINKSORTREFTAB_H
#define LINKSORTREFTAB_H

#include "common.h"
#include "linkreftab.h"

class LinkSortRefTab : public LinkRefTab
{
public:
    u32 *m_curElement;

    LinkSortRefTab(i32 p_poolSize, i32 p_size);
    virtual ~LinkSortRefTab();

    // Overriden
    virtual void Clear() override;
    virtual void ClearThis() override;

    virtual void DelRefPtr(u32 *p_refPtr) override;
    virtual void PrintStatus() override;
    virtual void RunDelRef(RefRun *p_refRun) override;

    // Added
    virtual float *AddSort(u32 p_refNum, float p_sortKey, i32 p_insertMode);
    virtual float GetSort(u32 *p_refPtr);
    virtual void SetSortNr(u32 *p_refPtr, float p_sortKey);
};

#endif