#pragma once

#include "reftab.h"
#include "common.h"

class LinkRefTab : public RefTab
{
public:
    RefRun *m_next;
    RefRun *m_prev;
    RefTab *m_refTab;

    LinkRefTab(int p_poolSize, int p_size);
    virtual ~LinkRefTab();

    // Overriden
    UINT *Add(UINT p_refNum);

    void Clear();
    void ClearThis();
    int GetCount();
    void DelRefPtr(UINT *p_refNum);

    UINT GetRefNr(int p_refIndex);
    UINT *GetRefPtrNr(int p_refIndex);
    void PrintStatus();

    void RunDelRef(RefRun *p_refRun);
    void RunInitNxtRef(RefRun *p_refRun);
    void RunInitPrevRef(RefRun *p_refRun);

    UINT *RunNxtRefPtr(RefRun *p_refRun);
    UINT *RunPrevRefPtr(RefRun *p_refRun);
    UINT *RunToRefPtr(RefRun *p_refRun);

    // Added
    virtual UINT *AddStart(UINT p_refNum);
    virtual void AddEnd(UINT p_refNum);
    virtual UINT *InsertBefore(UINT *p_uintPtr, UINT p_refNum);

    virtual UINT *GetPrevRefPtr(UINT *p_refPtr);
    virtual UINT *GetNextRefPtr(UINT *p_refPtr);

    void Destroy();
};
