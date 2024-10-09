#pragma once

#include "reftab.h"

class StaticRefTab : public RefTab
{
public:
    RefTab *m_refTab;

    StaticRefTab(int p_poolSize, int p_size);
    virtual ~StaticRefTab();

    // Overriden
    UINT *Add(UINT p_refNum);
    void AddUnique(UINT p_refNum);

    void Clear();
    void ClearThis();

    void DelRefPtr(UINT *p_refNum);
    bBool Exists(UINT p_refNum);
    UINT *Find(UINT p_refNum);

    UINT GetRefNr(int p_refIndex);
    UINT *GetRefPtrNr(int p_refIndex);

    void PrintStatus();

    void Remove(UINT p_refNum);
    bBool RemoveIfExists(UINT p_refNum);

    void RunDelRef(RefRun *p_refRun);
    void RunInitNxtRef(RefRun *p_refRun);
    void RunInitPrevRef(RefRun *p_refRun);

    UINT RunNxtRef(RefRun *p_refRun);
    UINT *RunNxtRefPtr(RefRun *p_refRun);

    UINT RunPrevRef(RefRun *p_refRun);
    UINT *RunPrevRefPtr(RefRun *p_refRun);

    void Destroy();
};
