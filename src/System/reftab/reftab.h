#pragma once

#include <windows.h>

#include "system/zsyscom.h"
#include "system/zsysmem.h"
#include "system/system.h"

#include "typedefs.h"

struct RefRun
{
    RefRun *prev;
    RefRun *next;
    int usedUnits;
};

class RefTab
{
private:
    RefRun *m_head;
    RefRun *m_tail;
    int m_poolSize;
    int m_blockCapacity;
    int m_count;
    int m_size;

public:
    RefTab(int p_poolSize, int p_size);
    ~RefTab();

    virtual void *Add(UINT p_refNum);
    virtual void AddUnique(UINT p_refNum);

    virtual void Clear();
    virtual void ClearThis();

    virtual int GetCount();

    virtual void *DelRefPtr(UINT *p_refNum);
    virtual bBool Exists(UINT *p_refPtr);
    virtual void *Find(UINT p_id);

    virtual int GetRefNr(int refNum);
    virtual void *GetRefPtrNr(int refNum);

    virtual void PrintStatus();

    virtual void Remove(UINT p_refNumber);
    virtual bBool RemoveIfExists(UINT p_refNumber);

    virtual void RunDelRef(RefRun *refRun);
    virtual void RunInitNxtRef(RefRun *refRun);
    virtual void RunInitPrevRef(RefRun *refRun);

    virtual int RunNxtRef(RefRun *refRun);
    virtual int *RunNxtRefPtr(RefRun *refRun);

    virtual int RunPrevRef(RefRun *refRun);
    virtual int *RunPrevRefPtr(RefRun *refRun);

    virtual void *RunToRefPtr(RefRun *refRun);

    virtual void DeleteBlock(LPVOID p_lpMem);
    virtual RefRun *NewBlock();
};
