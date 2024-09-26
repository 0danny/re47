#include "reftab.h"

// MATCHED
RefTab::RefTab(int p_poolSize, int p_size)
{
    if (p_poolSize < 1)
        p_poolSize = 1;

    this->m_blockCapacity = p_poolSize * (p_size + 1);
    this->m_count = 0;
    this->m_head = 0;
    this->m_tail = 0;
    this->m_size = p_size + 1;
    this->m_poolSize = p_poolSize;
}

// MATCHED
RefTab::~RefTab()
{
    RefRun *current = m_head;
    m_poolSize |= 0x80000000;

    while (current)
    {
        current = current->next;

        DeleteBlock(current);
    }

    m_count = 0;
    m_head = 0;
}

// MATCHED
UINT *RefTab::Add(UINT p_refNum)
{
    if (!m_head || m_tail->usedUnits == m_blockCapacity)
    {
        RefRun *newBlock = NewBlock();
        newBlock->next = 0;

        if (m_head)
        {
            m_tail->next = newBlock;
            newBlock->prev = m_tail;
        }
        else
        {
            m_head = newBlock;

            newBlock->prev = 0;
            newBlock = m_head;
        }

        m_tail = newBlock;

        newBlock->usedUnits = 0;
    }

    UINT *data = reinterpret_cast<UINT *>(&m_tail->data + m_tail->usedUnits);

    *data = p_refNum;

    m_tail->usedUnits += m_size;

    ++m_count;

    // Return the ptr after the last data.
    return data + 1;
}

// MATCHED
void RefTab::AddUnique(UINT p_refNum)
{
    if (!Find(p_refNum))
        Add(p_refNum);
}

// MATCHED
void RefTab::Clear()
{
    ClearThis();
}

// MATCHED
void RefTab::ClearThis()
{
    RefRun *current = m_head;
    m_poolSize |= 0x80000000;

    while (current)
    {
        RefRun *next = current;
        current = current->next;

        DeleteBlock(next);
    }

    m_count = 0;
    m_head = nullptr;
}

// MATCHED
int RefTab::GetCount()
{
    return m_count;
}

// MATCHED
void RefTab::DelRefPtr(UINT *p_refNum)
{
    RefRun l_refRun;

    if (this)
    {
        RunInitNxtRef(&l_refRun);
        UINT *l_nextRef = (UINT *)RunNxtRefPtr(&l_refRun);

        if (l_nextRef)
        {
            while (1)
            {
                if (l_nextRef == p_refNum)
                    break;

                l_nextRef = (UINT *)RunNxtRefPtr(&l_refRun);

                if (!l_nextRef)
                    return;
            }

            RunDelRef(&l_refRun);

            this->m_poolSize |= 0x80000000;
        }
    }
}

// MATCHED
bBool RefTab::Exists(UINT p_refNum)
{
    if (!this)
        return 0;

    RefRun l_refRun;

    RunInitNxtRef(&l_refRun);
    UINT l_nextRef = RunNxtRef(&l_refRun);

    if (!l_refRun.prev)
        return 0;

    while (l_nextRef != p_refNum)
    {
        l_nextRef = RunNxtRef(&l_refRun);

        if (!l_refRun.prev)
            return 0;
    }

    return 1;
}

// MATCHED
UINT *RefTab::Find(UINT p_refNum)
{
    RefRun l_refRun;

    if (!this)
        return 0;

    RunInitNxtRef(&l_refRun);
    UINT *result = RunNxtRefPtr(&l_refRun);

    if (!result)
        return 0;

    while (*result != p_refNum)
    {
        result = RunNxtRefPtr(&l_refRun);

        if (!result)
            return 0;
    }

    return result;
}

// MATCHED
UINT RefTab::GetRefNr(int p_refIndex)
{
    // Gets the reference by index
    if (p_refIndex >= m_count)
        return 0;

    RefRun *i;

    int poolRes = m_poolSize & 0x7FFFFFFF;

    for (i = m_head; p_refIndex >= poolRes; p_refIndex -= poolRes)
        i = i->next;

    return *((UINT *)&i->data + p_refIndex * this->m_size);
}

// MATCHED
UINT *RefTab::GetRefPtrNr(int p_refIndex)
{
    // Gets the reference by index
    if (p_refIndex >= m_count)
        return 0;

    RefRun *i;

    int poolRes = m_poolSize & 0x7FFFFFFF;

    for (i = m_head; p_refIndex >= poolRes; p_refIndex -= poolRes)
        i = i->next;

    return ((UINT *)&i->data + p_refIndex * this->m_size);
}

// MATCHED
void RefTab::PrintStatus()
{
    RefRun *currentBlock;
    UINT *data;
    int usedUnits = 0;
    int l_count;

    char *l_malloc = (char *)System::Alloc(256);
    char *l_mallocPtr = l_malloc;

    RefRun *l_currentBlock = this->m_head;

    int i;

    for (i = 0; l_currentBlock; ++i)
        l_currentBlock = l_currentBlock->next;

    ZSysCom *com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 518);
    com->UnkFunc4("REFTAB::PrintStatus Nr %d\n", i);

    for (currentBlock = m_head; currentBlock; currentBlock = currentBlock->next)
    {
        data = (UINT *)&currentBlock->data;
        l_count = 0;

        if (currentBlock->usedUnits)
        {
            for (int j = 0; j < currentBlock->usedUnits; j += m_size)
            {
                _ltoa(*data, l_mallocPtr, 10);

                l_mallocPtr += strlen(l_mallocPtr);

                data += m_size;
                usedUnits = currentBlock->usedUnits;

                l_count++;
            }

            l_mallocPtr = l_malloc;
        }

        com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 534);
        com->UnkFunc4("RefBlk %d -> %s\n", currentBlock->usedUnits, l_mallocPtr);
    }

    // Free the allocated buffer
    System::FreeArr(l_mallocPtr);
}

// MATCHED
void RefTab::Remove(UINT p_refNum)
{
    if (!RemoveIfExists(p_refNum))
    {
        ZSysCom *com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 290);
        com->UnkFunc4("WARNING: unable to remove REF %d\n", p_refNum);
    }
}

// MATCHED
bBool RefTab::RemoveIfExists(UINT p_refNum)
{
    RefRun l_refRun;

    if (!this)
        return 0;

    RunInitNxtRef(&l_refRun);
    UINT l_nextRef = RunNxtRef(&l_refRun);

    if (!l_refRun.prev)
        return 0;

    while (1)
    {
        if (l_nextRef == p_refNum)
            break;

        l_nextRef = RunNxtRef(&l_refRun);

        if (!l_refRun.prev)
            return 0;
    }

    RunDelRef(&l_refRun);

    m_poolSize |= 0x80000000;

    return 1;
}

// MATCHED
void RefTab::RunDelRef(RefRun *p_refRun)
{
    RefRun *l_tail = m_tail;
    int l_size = m_size;

    --m_count;

    l_tail->usedUnits -= l_size;

    if (p_refRun->usedUnits > 0)
    {
        l_size = m_size;

        bBool l_less0 = (int)p_refRun->next - l_size < 0;
        p_refRun->next = (RefRun *)((char *)p_refRun->next - l_size);

        if (l_less0)
        {
            p_refRun->next = (RefRun *)(m_blockCapacity - m_size);
            p_refRun->prev = p_refRun->prev->prev;
        }
    }

    if (m_count)
    {
        UINT *l_flag = (UINT *)(&m_tail->data + m_tail->usedUnits);
        UINT *l_flag2 = (UINT *)(&p_refRun->prev->data + (int)p_refRun->next);

        if (l_flag != l_flag2)
            memcpy(l_flag2, l_flag, 4 * m_size);
    }

    RefRun *l_tail2 = m_tail;

    if (!l_tail2->usedUnits)
    {
        RefRun *l_prev = l_tail2->prev;
        m_tail = l_tail2->prev;

        if (l_tail2->prev == 0)
        {
            m_head = 0;
            p_refRun->prev = 0;

            if (m_count)
                g_pSysCom->ThrowFatal("RefPik\n");

            DeleteBlock(l_tail2);
        }
        else
        {
            if (l_tail2 == p_refRun->prev)
            {
                p_refRun->prev = l_prev;
                p_refRun->next = (RefRun *)m_blockCapacity;
            }

            m_tail->next = 0;

            DeleteBlock(l_tail2);
        }
    }
}

// MATCHED
void RefTab::RunInitNxtRef(RefRun *p_refRun)
{
    p_refRun->next = 0;
    p_refRun->usedUnits = 1;

    p_refRun->prev = m_head;
    m_poolSize &= ~0x80000000;
}

// MATCHED
void RefTab::RunInitPrevRef(RefRun *p_refRun)
{
    p_refRun->usedUnits = -1;

    p_refRun->prev = m_tail;
    RefRun *l_tail = m_tail;

    if (l_tail)
        p_refRun->next = (RefRun *)l_tail->usedUnits;

    m_poolSize &= ~0x80000000;
}

// MATCHED
UINT RefTab::RunNxtRef(RefRun *p_refRun)
{
    UINT *result = RunNxtRefPtr(p_refRun);

    return result ? *result : 0;
}

// MATCHED
UINT *RefTab::RunNxtRefPtr(RefRun *p_refRun)
{
    if (m_poolSize < 0)
    {
        ZSysCom *com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 373);
        com->UnkFunc4("ERROR: Illegal operation inside REFTAB loop");

        com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 374);
        com->UnkFunc4("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 374);

        DebugBreak();
    }

    if (p_refRun->next == (RefRun *)m_blockCapacity)
    {
        RefRun *prevTmp = p_refRun->prev;

        p_refRun->next = 0;
        p_refRun->prev = prevTmp->next;
    }

    RefRun *prev = p_refRun->prev;

    if (p_refRun->prev)
    {
        int next = (int)p_refRun->next;

        if (next != m_tail->usedUnits || prev->next)
        {
            p_refRun->next = (RefRun *)(next + m_size);

            return (UINT *)((char *)prev + sizeof(UINT) * next + 12);
        }
        else
        {
            p_refRun->prev = 0;
            return 0;
        }
    }

    return (UINT *)prev;
}

// MATCHED
UINT RefTab::RunPrevRef(RefRun *p_refRun)
{
    UINT *result = RunPrevRefPtr(p_refRun);
    return result ? *result : 0;
}

// MATCHED
UINT *RefTab::RunPrevRefPtr(RefRun *p_refRun)
{
    if (m_poolSize < 0)
    {
        ZSysCom *com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 424);
        com->UnkFunc4("ERROR: Illegal operation inside REFTAB loop");

        com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 425);
        com->UnkFunc4("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 425);

        DebugBreak();
    }

    RefRun *prev;

    if (!p_refRun->prev)
        return 0;

    int l_size = m_size;

    bBool l_flag = (int)p_refRun->next - l_size < 0;
    p_refRun->next = (RefRun *)((char *)p_refRun->next - l_size);

    if (l_flag)
    {
        prev = p_refRun->prev->prev;
        p_refRun->prev = prev;

        if (!prev)
            return 0;

        p_refRun->next = (RefRun *)(m_blockCapacity - m_size);
    }

    return (UINT *)(&p_refRun->prev->data + (int)p_refRun->next);
}

// MATCHED
UINT *RefTab::RunToRefPtr(RefRun *p_refRun)
{
    return (UINT *)(&p_refRun->prev->data + (int)p_refRun->next);
}

// MATCHED
void RefTab::DeleteBlock(void *p_lpMem)
{
    System::Free(p_lpMem);
}

// MATCHED
RefRun *RefTab::NewBlock()
{
    int newSize = sizeof(UINT) * m_blockCapacity + 12;

    return reinterpret_cast<RefRun *>(System::Alloc(newSize));
}