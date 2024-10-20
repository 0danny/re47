#ifndef LINKREFTAB_H
#define LINKREFTAB_H

#include "reftab.h"

class LinkRefTab : public RefTab
{
public:
    RefRun *m_next;
    RefRun *m_prev;
    RefTab *m_refTab;

    LinkRefTab(i32 p_poolSize, i32 p_size);
    virtual ~LinkRefTab();

    // Overriden
    u32 *Add(u32 p_refNum);

    void Clear();
    void ClearThis();
    i32 GetCount();
    void DelRefPtr(u32 *p_refNum);

    u32 GetRefNr(i32 p_refIndex);
    u32 *GetRefPtrNr(i32 p_refIndex);
    void PrintStatus();

    void RunDelRef(RefRun *p_refRun);
    void RunInitNxtRef(RefRun *p_refRun);
    void RunInitPrevRef(RefRun *p_refRun);

    u32 *RunNxtRefPtr(RefRun *p_refRun);
    u32 *RunPrevRefPtr(RefRun *p_refRun);
    u32 *RunToRefPtr(RefRun *p_refRun);

    // Added
    virtual u32 *AddStart(u32 p_refNum);
    virtual void AddEnd(u32 p_refNum);
    virtual u32 *InsertBefore(u32 *p_uintPtr, u32 p_refNum);

    virtual u32 *GetPrevRefPtr(u32 *p_refPtr);
    virtual u32 *GetNextRefPtr(u32 *p_refPtr);

    void Destroy();
};

#endif