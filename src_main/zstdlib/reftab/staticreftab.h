#ifndef STATICREFTAB_H
#define STATICREFTAB_H

#include "reftab.h"

class StaticRefTab : public RefTab
{
public:
    RefTab *m_refTab;

    StaticRefTab(i32 p_poolSize, i32 p_size);
    virtual ~StaticRefTab();

    // Overriden
    virtual u32 *Add(u32 p_refNum) override;
    virtual u32 *AddUnique(u32 p_refNum) override;

    virtual void Clear() override;
    virtual void ClearThis() override;

    virtual void DelRefPtr(u32 *p_refNum) override;
    virtual bool Exists(u32 p_refNum) override;
    virtual u32 *Find(u32 p_refNum) override;

    virtual u32 GetRefNr(i32 p_refIndex) override;
    virtual u32 *GetRefPtrNr(i32 p_refIndex) override;

    virtual void PrintStatus() override;

    virtual void Remove(u32 p_refNum) override;
    virtual bool RemoveIfExists(u32 p_refNum) override;

    virtual void RunDelRef(RefRun *p_refRun) override;
    virtual void RunInitNxtRef(RefRun *p_refRun) override;
    virtual void RunInitPrevRef(RefRun *p_refRun) override;

    virtual u32 RunNxtRef(RefRun *p_refRun) override;
    virtual u32 *RunNxtRefPtr(RefRun *p_refRun) override;

    virtual u32 RunPrevRef(RefRun *p_refRun) override;
    virtual u32 *RunPrevRefPtr(RefRun *p_refRun) override;

    void Destroy();
};

#endif