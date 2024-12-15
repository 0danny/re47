#include "reftab.h"

// MATCHED
RefTab::RefTab(i32 p_poolSize, i32 p_size)
{
    if (p_poolSize < 1)
        p_poolSize = 1;

    m_blockCapacity = p_poolSize * (p_size + 1);
    m_count = 0;
    m_head = 0;
    m_tail = 0;
    m_size = p_size + 1;
    m_poolSize = p_poolSize;
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
u32 *RefTab::Add(u32 p_refNum)
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

    u32 *data = &m_tail->data[m_tail->usedUnits];

    *data = p_refNum;

    m_tail->usedUnits += m_size;

    ++m_count;

    // Return the ptr after the last data.
    return data + 1;
}

// MATCHED
u32 *RefTab::AddUnique(u32 p_refNum)
{
    u32 *l_result = Find(p_refNum);

    if (!l_result)
        return Add(p_refNum);

    return l_result;
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
    m_head = 0;
}

// MATCHED
i32 RefTab::GetCount()
{
    return m_count;
}

// MATCHED
void RefTab::DelRefPtr(u32 *p_refNum)
{
    RefRun l_refRun;

    if (this)
    {
        RunInitNxtRef(&l_refRun);
        u32 *l_nextRef = (u32 *)RunNxtRefPtr(&l_refRun);

        if (l_nextRef)
        {
            while (1)
            {
                if (l_nextRef == p_refNum)
                    break;

                l_nextRef = (u32 *)RunNxtRefPtr(&l_refRun);

                if (!l_nextRef)
                    return;
            }

            RunDelRef(&l_refRun);

            m_poolSize |= 0x80000000;
        }
    }
}

// MATCHED
bool RefTab::Exists(u32 p_refNum)
{
    if (!this)
        return 0;

    RefRun l_refRun;

    RunInitNxtRef(&l_refRun);
    u32 l_nextRef = RunNxtRef(&l_refRun);

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
u32 *RefTab::Find(u32 p_refNum)
{
    RefRun l_refRun;

    if (!this)
        return 0;

    RunInitNxtRef(&l_refRun);
    u32 *result = RunNxtRefPtr(&l_refRun);

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
u32 RefTab::GetRefNr(i32 p_refIndex)
{
    // Gets the reference by index
    if (p_refIndex >= m_count)
        return 0;

    RefRun *i;

    i32 poolRes = m_poolSize & 0x7FFFFFFF;

    for (i = m_head; p_refIndex >= poolRes; p_refIndex -= poolRes)
        i = i->next;

    return i->data[p_refIndex * m_size];
}

// MATCHED
u32 *RefTab::GetRefPtrNr(i32 p_refIndex)
{
    // Gets the reference ptr by index
    if (p_refIndex >= m_count)
        return 0;

    RefRun *i;

    i32 poolRes = m_poolSize & 0x7FFFFFFF;

    for (i = m_head; p_refIndex >= poolRes; p_refIndex -= poolRes)
        i = i->next;

    return &i->data[p_refIndex * m_size];
}

// MATCHED
void RefTab::PrintStatus()
{
    RefRun *currentBlock;
    u32 *data;
    i32 usedUnits = 0;
    i32 l_count;

    char *l_malloc = new char[256];
    char *l_mallocPtr = l_malloc;

    RefRun *l_currentBlock = m_head;

    i32 i;

    for (i = 0; l_currentBlock; ++i)
        l_currentBlock = l_currentBlock->next;

    ZSysCom *com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 518);
    com->LogMessage("REFTAB::PrintStatus Nr %d\n", i);

    for (currentBlock = m_head; currentBlock; currentBlock = currentBlock->next)
    {
        data = currentBlock->data;
        l_count = 0;

        if (currentBlock->usedUnits)
        {
            for (i32 j = 0; j < currentBlock->usedUnits; j += m_size)
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
        com->LogMessage("RefBlk %d -> %s\n", currentBlock->usedUnits, l_mallocPtr);
    }

    // Free the allocated buffer
    delete[] l_mallocPtr;
}

// MATCHED
void RefTab::Remove(u32 p_refNum)
{
    if (!RemoveIfExists(p_refNum))
    {
        ZSysCom *com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 290);
        com->LogMessage("WARNING: unable to remove REF %d\n", p_refNum);
    }
}

// MATCHED
bool RefTab::RemoveIfExists(u32 p_refNum)
{
    RefRun l_refRun;

    if (!this)
        return 0;

    RunInitNxtRef(&l_refRun);
    u32 l_nextRef = RunNxtRef(&l_refRun);

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
    i32 l_size = m_size;

    --m_count;

    l_tail->usedUnits -= l_size;

    if (p_refRun->usedUnits > 0)
    {
        l_size = m_size;

        bool l_less0 = (i32)p_refRun->next - l_size < 0;
        p_refRun->next = (RefRun *)((char *)p_refRun->next - l_size);

        if (l_less0)
        {
            p_refRun->next = (RefRun *)(m_blockCapacity - m_size);
            p_refRun->prev = p_refRun->prev->prev;
        }
    }

    if (m_count)
    {
        u32 *l_flag = &m_tail->data[m_tail->usedUnits];
        u32 *l_flag2 = &p_refRun->prev->data[(i32)p_refRun->next];

        if (l_flag != l_flag2)
            memcpy(l_flag2, l_flag, 4 * m_size);
    }

    RefRun *l_tail2 = m_tail;

    if (!l_tail2->usedUnits)
    {
        RefRun *l_prev = l_tail2->prev;
        bool l_flag3 = l_prev == 0;
        m_tail = l_prev;

        if (l_flag3)
        {
            m_head = 0;
            p_refRun->prev = 0;
        }
        else
        {
            if (l_tail2 == p_refRun->prev)
            {
                p_refRun->prev = l_prev;
                p_refRun->next = (RefRun *)m_blockCapacity;
            }

            m_tail->next = 0;
        }

        DeleteBlock(l_tail2);
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
u32 RefTab::RunNxtRef(RefRun *p_refRun)
{
    u32 *result = RunNxtRefPtr(p_refRun);

    return result ? *result : 0;
}

// MATCHED
u32 *RefTab::RunNxtRefPtr(RefRun *p_refRun)
{
    if (m_poolSize < 0)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 373);
        l_sysCom->LogMessage("ERROR: Illegal operation inside REFTAB loop");

        l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 374);
        l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 374);

        DebugBreak();
    }

    if (p_refRun->next == (RefRun *)m_blockCapacity)
    {
        RefRun *prevTmp = p_refRun->prev;

        p_refRun->next = 0;
        p_refRun->prev = prevTmp->next;
    }

    RefRun *l_prev = p_refRun->prev;

    if (p_refRun->prev)
    {
        i32 l_next = (i32)p_refRun->next;

        if (l_next != m_tail->usedUnits || l_prev->next)
        {
            p_refRun->next = (RefRun *)(l_next + m_size);

            return (u32 *)((char *)l_prev + sizeof(u32) * l_next + 12);
        }
        else
        {
            p_refRun->prev = 0;
            return 0;
        }
    }

    return (u32 *)l_prev;
}

// MATCHED
u32 RefTab::RunPrevRef(RefRun *p_refRun)
{
    u32 *result = RunPrevRefPtr(p_refRun);
    return result ? *result : 0;
}

// MATCHED
u32 *RefTab::RunPrevRefPtr(RefRun *p_refRun)
{
    if (m_poolSize < 0)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 424);
        l_sysCom->LogMessage("ERROR: Illegal operation inside REFTAB loop");

        l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 425);
        l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 425);

        DebugBreak();
    }

    RefRun *prev;

    if (!p_refRun->prev)
        return 0;

    i32 l_size = m_size;

    bool l_flag = (i32)p_refRun->next - l_size < 0;
    p_refRun->next = (RefRun *)((char *)p_refRun->next - l_size);

    if (l_flag)
    {
        prev = p_refRun->prev->prev;
        p_refRun->prev = prev;

        if (!prev)
            return 0;

        p_refRun->next = (RefRun *)(m_blockCapacity - m_size);
    }

    return &p_refRun->prev->data[(i32)p_refRun->next];
}

// MATCHED
u32 *RefTab::RunToRefPtr(RefRun *p_refRun)
{
    return &p_refRun->prev->data[(i32)p_refRun->next];
}

// MATCHED
void RefTab::DeleteBlock(void *p_lpMem)
{
    delete p_lpMem;
}

// MATCHED
RefRun *RefTab::NewBlock()
{
    i32 newSize = sizeof(RefRun) + sizeof(u32) * m_blockCapacity;
    return reinterpret_cast<RefRun *>(operator new(newSize));
}
