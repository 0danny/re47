#pragma once

#include "reftab.h"

class StaticRefTab : public RefTab
{
public:
    RefTab *m_refTab;

    StaticRefTab(i32 p_poolSize, i32 p_size);
    virtual ~StaticRefTab();

    // Overriden
    u32 *Add(u32 p_refNum);
    void AddUnique(u32 p_refNum);

    void Clear();
    void ClearThis();

    void DelRefPtr(u32 *p_refNum);
    boolean Exists(u32 p_refNum);
    u32 *Find(u32 p_refNum);

    u32 GetRefNr(i32 p_refIndex);
    u32 *GetRefPtrNr(i32 p_refIndex);

    void PrintStatus();

    void Remove(u32 p_refNum);
    boolean RemoveIfExists(u32 p_refNum);

    void RunDelRef(RefRun *p_refRun);
    void RunInitNxtRef(RefRun *p_refRun);
    void RunInitPrevRef(RefRun *p_refRun);

    u32 RunNxtRef(RefRun *p_refRun);
    u32 *RunNxtRefPtr(RefRun *p_refRun);

    u32 RunPrevRef(RefRun *p_refRun);
    u32 *RunPrevRefPtr(RefRun *p_refRun);

    void Destroy();
};
