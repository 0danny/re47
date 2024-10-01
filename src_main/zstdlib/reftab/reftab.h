#pragma once

#include <windows.h>
#include "system/zsyscom.h"

struct RefRun
{
    RefRun *prev;
    RefRun *next;
    int usedUnits;
    UINT *data;
};

class RefTab
{
public:
    RefRun *m_head;
    RefRun *m_tail;
    int m_poolSize;
    int m_blockCapacity;
    int m_count;
    int m_size;

    RefTab(int p_poolSize, int p_size);

    virtual ~RefTab();

    virtual UINT *Add(UINT p_refNum);
    virtual void AddUnique(UINT p_refNum);

    virtual void Clear();
    virtual void ClearThis();

    virtual int GetCount();

    virtual void DelRefPtr(UINT *p_refNum);
    virtual bBool Exists(UINT p_refNum);
    virtual UINT *Find(UINT p_refNum);

    virtual UINT GetRefNr(int p_refIndex);
    virtual UINT *GetRefPtrNr(int p_refIndex);

    virtual void PrintStatus();

    virtual void Remove(UINT p_refNum);
    virtual bBool RemoveIfExists(UINT p_refNum);

    virtual void RunDelRef(RefRun *p_refRun);
    virtual void RunInitNxtRef(RefRun *p_refRun);
    virtual void RunInitPrevRef(RefRun *p_refRun);

    virtual UINT RunNxtRef(RefRun *p_refRun);
    virtual UINT *RunNxtRefPtr(RefRun *p_refRun);

    virtual UINT RunPrevRef(RefRun *p_refRun);
    virtual UINT *RunPrevRefPtr(RefRun *p_refRun);

    virtual UINT *RunToRefPtr(RefRun *p_refRun);

    virtual void DeleteBlock(void *p_lpMem);
    virtual RefRun *NewBlock();
};
