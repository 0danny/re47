#include "reftab32.h"

RefTab32::RefTab32() : RefTab(0, 0)
{
    m_blockCapacity = 32;
    m_count = 0;
    m_size = 1;
    m_poolSize = 32;
    m_head = 0;
    m_tail = 0;
}

RefTab32::~RefTab32()
{
    Destroy();
}

void RefTab32::Destroy()
{
    RefRun *l_headPtr;
    RefRun *l_head = m_head;

    m_poolSize |= 0x80000000;

    while (l_head)
    {
        l_headPtr = l_head;
        l_head = l_head->next;

        DeleteBlock(l_headPtr);
    }

    int l_poolRes = m_poolSize | 0x80000000;

    m_poolSize = l_poolRes;
    m_count = 0;
    m_head = 0;
}

void RefTab32::DeleteBlock(void *p_lpMem)
{
    if (p_lpMem != &m_refRun)
    {
        delete p_lpMem;
    }
}

RefRun *RefTab32::NewBlock()
{
    if (!m_head)
        return &m_refRun;

    int l_blockSize = 4 * m_blockCapacity + 12;

    return reinterpret_cast<RefRun *>(operator new(l_blockSize));
}
