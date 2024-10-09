#include "staticreftab.h"

StaticRefTab::StaticRefTab(int p_poolSize, int p_size) : RefTab(p_poolSize, p_size + 1)
{
    RefTab *l_refTab = new RefTab(0, 0);

    if (l_refTab)
    {
        int l_poolSize = m_poolSize & 0x7FFFFFFF;

        if (l_poolSize < 1)
            l_poolSize = 1;

        l_refTab->m_blockCapacity = l_poolSize;
        l_refTab->m_count = 0;
        l_refTab->m_size = 1;
        l_refTab->m_poolSize = l_poolSize;
        l_refTab->m_head = 0;
        l_refTab->m_tail = 0;

        m_refTab = l_refTab;
    }
    else
    {
        m_refTab = 0;
    }
}

StaticRefTab::~StaticRefTab()
{
    Destroy();
}

void StaticRefTab::Destroy()
{
    RefRun *l_head;

    if (m_refTab)
        m_refTab->~RefTab();

    m_poolSize = m_poolSize | 0x80000000;

    while (m_head)
    {
        l_head = m_head;
        m_head = m_head->next;

        DeleteBlock(l_head);
    }
    m_count = 0;
    m_head = 0;
}

UINT *StaticRefTab::Add(UINT p_refNum)
{
    m_count += 1;

    if (!m_refTab->GetCount())
    {
        RefRun *l_newBlock = NewBlock();

        l_newBlock->prev = 0;
        l_newBlock->next = m_head;
        l_newBlock->usedUnits = 0;

        if (m_head)
            m_head->prev = l_newBlock;

        int l_poolRes = m_poolSize & 0x7FFFFFFF;

        m_head = l_newBlock;

        UINT l_refNum = (UINT)&l_newBlock[1];
        int l_offset = 12;

        if (l_poolRes)
        {
            int l_count = l_poolRes;
            int l_newSize;

            do
            {
                m_refTab->Add(l_refNum);

                *(UINT *)(l_refNum + 4 * m_size - 4) = (UINT)l_newBlock;
                l_newSize = 4 * m_size;

                l_refNum += l_newSize;
                l_offset += l_newSize;

                --l_count;
            } while (l_count);
        }
    }

    UINT *l_nxtRef;
    UINT *l_nxtRefPtr;
    RefRun l_refRun;

    if (m_refTab && (m_refTab->RunInitNxtRef(&l_refRun), l_nxtRef = (UINT *)m_refTab->RunNxtRef(&l_refRun), l_refRun.prev))
    {
        l_nxtRefPtr = l_nxtRef;
        m_refTab->RunDelRef(&l_refRun);
    }
    else
    {
        l_nxtRefPtr = (UINT *)p_refNum;
    }

    RefRun *l_data = (RefRun *)l_nxtRefPtr[m_size - 1];

    ++l_data->usedUnits;
    *l_nxtRefPtr = p_refNum;

    return l_nxtRefPtr + 1;
}

void StaticRefTab::AddUnique(UINT p_refNum)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 616);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 616);

    DebugBreak();
}

void StaticRefTab::Clear()
{
    if (m_refTab)
    {
        m_refTab->~RefTab();
        m_refTab = 0;
    }

    m_poolSize |= 0x80000000;

    RefRun *l_head;
    while (m_head)
    {
        l_head = m_head;
        m_head = m_head->next;

        DeleteBlock(l_head);
    }

    m_count = 0;
    m_head = 0;
}

void StaticRefTab::ClearThis()
{
    if (m_refTab)
    {
        m_refTab->~RefTab();
        m_refTab = 0;
    }
}

void StaticRefTab::DelRefPtr(UINT *p_refNum)
{
    m_count = m_count - 1;
    RefRun *l_data = (RefRun *)p_refNum[m_size - 1];

    if (l_data->usedUnits-- == 1)
    {
        if (l_data->prev)
            l_data->prev->next = l_data->next;
        else
            m_head = l_data->next;

        if (l_data->next)
            l_data->next->prev = l_data->prev;

        if (m_refTab)
        {
            RefRun l_refRun;
            m_refTab->RunInitNxtRef(&l_refRun);

            for (UINT i = m_refTab->RunNxtRef(&l_refRun); l_refRun.prev; i = m_refTab->RunNxtRef(&l_refRun))
            {
                printf("Reference is -> %d\n", i);

                if (*(RefRun **)(i + 4 * this->m_size - 4) == l_data)
                    m_refTab->RunDelRef(&l_refRun);
            }
        }

        DeleteBlock(l_data);
    }
    else
    {
        m_refTab->Add((UINT)p_refNum);
    }
}

bBool StaticRefTab::Exists(UINT p_refNum)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 668);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 668);

    DebugBreak();
    return 0;
}

UINT *StaticRefTab::Find(UINT p_refNum)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 673);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 673);

    DebugBreak();
    return 0;
}

UINT StaticRefTab::GetRefNr(int p_refIndex)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 678);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 678);

    DebugBreak();
    return 0;
}

UINT *StaticRefTab::GetRefPtrNr(int p_refIndex)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 683);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 683);

    DebugBreak();
    return 0;
}

void StaticRefTab::PrintStatus()
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 688);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 688);

    DebugBreak();
}

void StaticRefTab::Remove(UINT p_refNum)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 692);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 692);

    DebugBreak();
}

bBool StaticRefTab::RemoveIfExists(UINT p_refNum)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 696);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 696);

    DebugBreak();

    return 0;
}

void StaticRefTab::RunDelRef(RefRun *p_refRun)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 701);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 701);

    DebugBreak();
}

void StaticRefTab::RunInitNxtRef(RefRun *p_refRun)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 705);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 705);

    DebugBreak();
}

void StaticRefTab::RunInitPrevRef(RefRun *p_refRun)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 709);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 709);

    DebugBreak();
}

UINT StaticRefTab::RunNxtRef(RefRun *p_refRun)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 713);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 713);

    DebugBreak();
    return 0;
}

UINT *StaticRefTab::RunNxtRefPtr(RefRun *p_refRun)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 718);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 718);

    DebugBreak();
    return 0;
}

UINT StaticRefTab::RunPrevRef(RefRun *p_refRun)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 723);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 723);

    DebugBreak();
    return 0;
}

UINT *StaticRefTab::RunPrevRefPtr(RefRun *p_refRun)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 728);
    l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 728);

    DebugBreak();
    return 0;
}