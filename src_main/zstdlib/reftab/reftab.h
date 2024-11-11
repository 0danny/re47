#ifndef REFTAB_H
#define REFTAB_H

#include "system/zsyscom.h"

struct RefRun
{
    RefRun *prev;
    RefRun *next;
    i32 usedUnits;
    u32 *data;
};

class RefTab
{
public:
    RefRun *m_head;
    RefRun *m_tail;
    i32 m_poolSize;
    i32 m_blockCapacity;
    i32 m_count;
    i32 m_size;

    RefTab(i32 p_poolSize, i32 p_size);

    virtual ~RefTab();

    virtual u32 *Add(u32 p_refNum);
    virtual u32 *AddUnique(u32 p_refNum);

    virtual void Clear();
    virtual void ClearThis();

    virtual i32 GetCount();

    virtual void DelRefPtr(u32 *p_refNum);
    virtual boolean Exists(u32 p_refNum);
    virtual u32 *Find(u32 p_refNum);

    virtual u32 GetRefNr(i32 p_refIndex);
    virtual u32 *GetRefPtrNr(i32 p_refIndex);

    virtual void PrintStatus();

    virtual void Remove(u32 p_refNum);
    virtual boolean RemoveIfExists(u32 p_refNum);

    virtual void RunDelRef(RefRun *p_refRun);
    virtual void RunInitNxtRef(RefRun *p_refRun);
    virtual void RunInitPrevRef(RefRun *p_refRun);

    virtual u32 RunNxtRef(RefRun *p_refRun);
    virtual u32 *RunNxtRefPtr(RefRun *p_refRun);

    virtual u32 RunPrevRef(RefRun *p_refRun);
    virtual u32 *RunPrevRefPtr(RefRun *p_refRun);

    virtual u32 *RunToRefPtr(RefRun *p_refRun);

    virtual void DeleteBlock(void *p_lpMem);
    virtual RefRun *NewBlock();
};

#endif