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
    virtual u32 *Add(u32 p_refNum) override;

    virtual void Clear() override;
    virtual void ClearThis() override;
    virtual i32 GetCount() override;
    virtual void DelRefPtr(u32 *p_refNum) override;

    virtual u32 GetRefNr(i32 p_refIndex) override;
    virtual u32 *GetRefPtrNr(i32 p_refIndex) override;
    virtual void PrintStatus() override;

    virtual void RunDelRef(RefRun *p_refRun) override;
    virtual void RunInitNxtRef(RefRun *p_refRun) override;
    virtual void RunInitPrevRef(RefRun *p_refRun) override;

    virtual u32 *RunNxtRefPtr(RefRun *p_refRun) override;
    virtual u32 *RunPrevRefPtr(RefRun *p_refRun) override;
    virtual u32 *RunToRefPtr(RefRun *p_refRun) override;

    // Added
    virtual u32 *AddStart(u32 p_refNum);
    virtual u32 *AddEnd(u32 p_refNum);
    virtual u32 *InsertBefore(u32 *p_uintPtr, u32 p_refNum);

    virtual u32 *GetPrevRefPtr(u32 *p_refPtr);
    virtual u32 *GetNextRefPtr(u32 *p_refPtr);

    void Destroy();
};

#endif