#include "reftab.h"

RefTab::RefTab(int p_poolSize, int p_size)
{
    if (p_poolSize < 1)
        p_poolSize = 1;

    this->m_blockCapacity = p_poolSize * (p_size + 1);
    this->m_count = 0;
    this->m_head = nullptr;
    this->m_tail = nullptr;
    this->m_size = p_size + 1;
    this->m_poolSize = p_poolSize;
}

RefTab::~RefTab()
{
    RefRun *current = m_head;
    m_poolSize |= 0x80000000;

    while (current)
    {
        RefRun *next = current->next;
        DeleteBlock(current);
        current = next;
    }

    m_count = 0;
    m_head = nullptr;
}

void *RefTab::Add(UINT refNum)
{
    if (!m_head || m_tail->usedUnits == m_blockCapacity)
    {
        RefRun *newBlock = NewBlock();
        newBlock->next = nullptr;

        if (m_head)
        {
            m_tail->next = newBlock;
            newBlock->prev = m_tail;
        }
        else
        {
            m_head = newBlock;
            newBlock->prev = nullptr;
        }

        m_tail = newBlock;
        newBlock->usedUnits = 0;
    }

    UINT *data = reinterpret_cast<UINT *>(m_tail + 1);
    UINT *ptr = data + m_tail->usedUnits;
    *ptr = refNum;

    m_tail->usedUnits += m_size;
    ++m_count;

    return ptr + 1;
}

void RefTab::AddUnique(UINT refNum)
{
    if (!Find(refNum))
        Add(refNum);
}

void RefTab::Clear()
{
    ClearThis();
}

void RefTab::ClearThis()
{
    RefRun *current = m_head;
    m_poolSize |= 0x80000000;

    while (current)
    {
        RefRun *next = current->next;
        DeleteBlock(current);
        current = next;
    }

    m_count = 0;
    m_head = nullptr;
}

int RefTab::GetCount()
{
    return m_count;
}

void *RefTab::DelRefPtr(UINT *refNum)
{
    if (!this)
        return nullptr;

    RefRun refRun;
    RunInitNextRef(&refRun);
    int *result = RunNextRefPtr(&refRun);

    while (result)
    {
        if (reinterpret_cast<UINT *>(result) == refNum)
        {
            RunDelRef(&refRun);
            m_poolSize |= 0x80000000;
            return result;
        }

        result = RunNextRefPtr(&refRun);
    }

    return nullptr;
}

bBool RefTab::Exists(UINT *refPtr)
{
    if (!this)
        return false;

    RefRun refRun;
    RunInitNextRef(&refRun);
    int *nextRef = RunNextRefPtr(&refRun);

    while (nextRef)
    {
        if (reinterpret_cast<UINT *>(nextRef) == refPtr)
            return true;

        nextRef = RunNextRefPtr(&refRun);
    }

    return false;
}

void *RefTab::Find(UINT id)
{
    if (!this)
        return nullptr;

    RefRun refRun;
    RunInitNextRef(&refRun);
    UINT *result = reinterpret_cast<UINT *>(RunNextRefPtr(&refRun));

    while (result)
    {
        if (*result == id)
            return result;

        result = reinterpret_cast<UINT *>(RunNextRefPtr(&refRun));
    }

    return nullptr;
}

int RefTab::GetRefNr(int refNum)
{
    if (refNum >= m_count)
        return 0;

    int poolRes = m_poolSize & 0x7FFFFFFF;
    RefRun *block = m_head;

    while (refNum >= poolRes)
    {
        block = block->next;
        refNum -= poolRes;
    }

    UINT *data = reinterpret_cast<UINT *>(block + 1);
    return data[refNum * m_size];
}

void *RefTab::GetRefPtrNr(int refNum)
{
    if (refNum >= m_count)
        return nullptr;

    int poolRes = m_poolSize & 0x7FFFFFFF;
    RefRun *block = m_head;

    while (refNum >= poolRes)
    {
        block = block->next;
        refNum -= poolRes;
    }

    UINT *data = reinterpret_cast<UINT *>(block + 1);
    return &data[refNum * m_size];
}

void RefTab::PrintStatus()
{
    // Allocate a buffer for formatting the output
    char *buffer;
    if (g_pSysMem)
    {
        // buffer = (*(g_pSysMem->vtable + 11))(g_pSysMem, 256);
    }
    else
    {
        buffer = (char *)malloc(256u);
    }

    // Count the number of blocks in the linked list
    RefRun *currentBlock = m_head;
    int blockCount = 0;

    while (currentBlock)
    {
        blockCount++;
        currentBlock = currentBlock->next;
    }

    ZSysCom *sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 518);
    sysCom->UnkFunc4("REFTAB::PrintStatus Nr %d\n", blockCount);

    // Iterate over each block
    for (currentBlock = m_head; currentBlock; currentBlock = currentBlock->next)
    {
        // Point to the data area after the RefRun structure
        UINT *dataPtr = (UINT *)(currentBlock + 1);
        int usedUnits = currentBlock->usedUnits;
        char *bufferPtr = buffer;

        if (usedUnits > 0)
        {
            // Iterate over each unit in the block
            for (int i = 0; i < usedUnits; i++)
            {
                UINT *unitPtr = dataPtr + i * m_size;

                // Convert the data to string and append to the buffer
                sprintf(bufferPtr, "%u ", *unitPtr);
                bufferPtr += strlen(bufferPtr);
            }

            // Remove the trailing space and null-terminate the string
            if (bufferPtr != buffer)
                *(bufferPtr - 1) = '\0';
        }
        else
        {
            *buffer = '\0';
        }

        // Print the used units and the data in the current block
        sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 534);
        sysCom->UnkFunc4("RefBlk %d -> %s\n", usedUnits, buffer);
    }

    // Free the allocated buffer
    if (g_pSysMem)
        //(*(g_pSysMem->vtable + 13))(g_pSysMem, l_mallocPtr);
        return;
    else
        free(buffer);
}

void RefTab::Remove(UINT refNumber)
{
    if (!RemoveIfExists(refNumber))
    {
        ZSysCom *sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 290);
        sysCom->UnkFunc4("WARNING: unable to remove REF %d\n", refNumber);
    }
}

bBool RefTab::RemoveIfExists(UINT refNumber)
{
    if (!this)
        return false;

    RefRun refRun;
    RunInitNextRef(&refRun);
    int nextRef = RunNextRef(&refRun);

    while (refRun.prev)
    {
        if (nextRef == refNumber)
        {
            RunDelRef(&refRun);
            m_poolSize |= 0x80000000;
            return true;
        }

        nextRef = RunNextRef(&refRun);
    }

    return false;
}

void RefTab::RunDelRef(RefRun *refRun)
{
    RefRun *tailBlock = m_tail;
    int unitSize = m_size;

    --m_count;
    tailBlock->usedUnits -= unitSize;

    if (refRun->usedUnits > 0)
    {
        refRun->usedUnits -= unitSize;

        if (refRun->usedUnits < 0)
        {
            refRun->usedUnits = m_blockCapacity - m_size;
            refRun->prev = refRun->prev->prev;
        }
    }

    if (m_count)
    {
        UINT *tailData = reinterpret_cast<UINT *>(tailBlock + 1) + tailBlock->usedUnits;
        UINT *refData = reinterpret_cast<UINT *>(refRun->prev + 1) + refRun->usedUnits;

        if (tailData != refData)
            memcpy(refData, tailData, sizeof(UINT) * m_size);
    }

    if (tailBlock->usedUnits == 0)
    {
        RefRun *prevBlock = tailBlock->prev;
        m_tail = prevBlock;

        if (prevBlock)
        {
            prevBlock->next = nullptr;
        }
        else
        {
            m_head = nullptr;
            refRun->prev = nullptr;

            if (m_count)
                g_pSysCom->ThrowFatal("RefPik\n");
        }

        DeleteBlock(tailBlock);
    }
}

void RefTab::RunInitNextRef(RefRun *refRun)
{
    refRun->usedUnits = 0;
    refRun->prev = m_head;
    m_poolSize &= ~0x80000000;
}

void RefTab::RunInitPrevRef(RefRun *refRun)
{
    refRun->usedUnits = -1;
    refRun->prev = m_tail;

    if (m_tail)
        refRun->usedUnits = m_tail->usedUnits;

    m_poolSize &= ~0x80000000;
}

int RefTab::RunNextRef(RefRun *refRun)
{
    int *result = RunNextRefPtr(refRun);

    return result ? *result : 0;
}

int *RefTab::RunNextRefPtr(RefRun *refRun)
{
    if (m_poolSize < 0)
    {
        ZSysCom *l_com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 373);
        l_com->UnkFunc4("ERROR: Illegal operation inside REFTAB loop");

        l_com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 374);
        l_com->UnkFunc4("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 374);

        DebugBreak();
    }

    if (refRun->usedUnits == m_blockCapacity)
    {
        refRun->usedUnits = 0;
        refRun->prev = refRun->prev->next;
    }

    if (refRun->prev)
    {
        int usedUnits = refRun->usedUnits;

        if (usedUnits != m_tail->usedUnits || refRun->prev->next)
        {
            refRun->usedUnits += m_size;
            UINT *data = reinterpret_cast<UINT *>(refRun->prev + 1);
            return reinterpret_cast<int *>(data + usedUnits);
        }
        else
        {
            refRun->prev = nullptr;
            return nullptr;
        }
    }

    return nullptr;
}

int RefTab::RunPrevRef(RefRun *refRun)
{
    int *result = RunPrevRefPtr(refRun);
    return result ? *result : 0;
}

int *RefTab::RunPrevRefPtr(RefRun *refRun)
{
    if (m_poolSize < 0)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 424);
        l_sysCom->UnkFunc4("ERROR: Illegal operation inside REFTAB loop");

        l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 425);
        l_sysCom->UnkFunc4("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 425);

        DebugBreak();
    }

    if (!refRun->prev)
        return nullptr;

    refRun->usedUnits -= m_size;

    if (refRun->usedUnits < 0)
    {
        refRun->prev = refRun->prev->prev;
        if (!refRun->prev)
            return nullptr;

        refRun->usedUnits = m_blockCapacity - m_size;
    }

    UINT *data = reinterpret_cast<UINT *>(refRun->prev + 1);
    return reinterpret_cast<int *>(data + refRun->usedUnits);
}

void *RefTab::RunToRefPtr(RefRun *refRun)
{
    UINT *data = reinterpret_cast<UINT *>(refRun->prev + 1);
    return data + refRun->usedUnits;
}

void RefTab::DeleteBlock(void *mem)
{
    if (g_pSysMem)
    {
        // TODO: Implement proper deletion using g_pSysMem
    }
    else
    {
        free(mem);
    }
}

RefRun *RefTab::NewBlock()
{
    int newSize = sizeof(RefRun) + sizeof(UINT) * m_blockCapacity;

    if (g_pSysMem)
    {
        // TODO: Implement proper allocation using g_pSysMem
        return nullptr;
    }
    else
    {
        return reinterpret_cast<RefRun *>(malloc(newSize));
    }
}