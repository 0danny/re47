#include "linkreftab.h"

LinkRefTab::LinkRefTab(int p_poolSize, int p_size) : RefTab(p_poolSize, p_size + 2)
{
    m_refTab = 0;
    m_next = 0;
    m_prev = 0;
}

LinkRefTab::~LinkRefTab()
{
    Destroy();
}

void LinkRefTab::Destroy()
{
    Clear();
}

UINT *LinkRefTab::Add(UINT p_refNum)
{
    UINT *l_nextRefNum;
    RefRun l_refRun;

    if (m_refTab)
    {
        m_refTab->RunInitNxtRef(&l_refRun);
        UINT l_nextRef = m_refTab->RunNxtRef(&l_refRun);

        if (l_refRun.prev)
        {
            l_nextRefNum = (UINT *)(l_nextRef + 4 * (2 - m_size));
            m_refTab->RunDelRef(&l_refRun);
        }
        else
        {
            l_nextRefNum = (UINT *)p_refNum;
        }

        *l_nextRefNum = p_refNum;

        if (!m_refTab->GetCount())
        {
            if (m_refTab)
                m_refTab->~RefTab();

            m_refTab = 0;
        }
    }
    else
    {
        l_nextRefNum = RefTab::Add(p_refNum) - 1;
    }

    RefRun *l_nextRefRun = (RefRun *)&l_nextRefNum[m_size - 2];

    l_nextRefRun->prev = 0;
    l_nextRefRun->next = 0;

    l_nextRefRun->prev = m_prev;
    l_nextRefRun->next = 0;

    RefRun *l_prev = m_prev;

    if (l_prev)
        l_prev->next = l_nextRefRun;
    else
        m_next = l_nextRefRun;

    m_prev = l_nextRefRun;

    return l_nextRefNum + 1;
}

void LinkRefTab::Clear()
{
    if (m_refTab)
        m_refTab->~RefTab();

    m_refTab = 0;
    m_next = 0;
    m_prev = 0;
    m_poolSize = m_poolSize | 0x80000000;

    RefTab::~RefTab();
}

void LinkRefTab::ClearThis()
{
    if (m_refTab)
        m_refTab->~RefTab();

    m_refTab = 0;
    m_next = 0;
    m_prev = 0;
}

int LinkRefTab::GetCount()
{
    if (m_refTab)
        return m_count - m_refTab->GetCount();
    else
        return m_count;
}

void LinkRefTab::DelRefPtr(UINT *p_refPtr)
{
    RefRun *l_data = (RefRun *)&p_refPtr[m_size - 2];

    if (!m_refTab)
    {
        m_refTab = new RefTab(0, 0);

        if (m_refTab)
        {
            m_refTab->m_blockCapacity = 8;
            m_refTab->m_count = 0;
            m_refTab->m_size = 1;
            m_refTab->m_poolSize = 8;
            m_refTab->m_head = 0;
            m_refTab->m_tail = 0;
        }
        else
        {
            m_refTab = 0;
        }
    }

    m_refTab->Add((UINT)l_data);

    if (l_data->prev)
        l_data->prev->next = l_data->next;
    else
        m_next = l_data->next;

    RefRun *l_next = l_data->next;

    if (l_next)
        l_next->prev = l_data->prev;
    else
        m_prev = l_data->prev;

    l_data->next = 0;
    l_data->prev = 0;

    m_poolSize |= 0x80000000;
}

UINT LinkRefTab::GetRefNr(int p_refIndex)
{
    if (!this)
        return 0;

    RefRun l_refRun;

    RunInitNxtRef(&l_refRun);
    UINT result = RunNxtRef(&l_refRun);

    if (!l_refRun.prev)
        return 0;

    while (p_refIndex--)
    {
        result = RunNxtRef(&l_refRun);

        if (!l_refRun.prev)
            return 0;
    }

    return result;
}

UINT *LinkRefTab::GetRefPtrNr(int p_refIndex)
{
    if (!this)
        return 0;

    RefRun l_refRun;
    RunInitNxtRef(&l_refRun);

    UINT *l_refPtr = RunNxtRefPtr(&l_refRun);

    if (!l_refPtr)
        return 0;

    while (p_refIndex--)
    {
        l_refPtr = RunNxtRefPtr(&l_refRun);

        if (!l_refPtr)
            return 0;
    }

    return l_refPtr;
}

void LinkRefTab::PrintStatus()
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1070);

    int l_count = GetCount();

    l_sysCom->LogMessage("Nr Ele %d NrLinks %d\n", m_count, l_count);

    if (this)
    {
        RefRun l_refRun;
        RunInitNxtRef(&l_refRun);

        for (int i = RunNxtRef(&l_refRun); l_refRun.prev; i = RunNxtRef(&l_refRun))
        {
            l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1074);
            l_sysCom->LogMessage("Ref %d\n", i);
        }
    }
}

void LinkRefTab::RunDelRef(RefRun *p_refRun)
{
    RefRun *l_prev = p_refRun->prev;

    if (!p_refRun->prev)
        g_pSysCom->ThrowFatal("Dit Svin\n");

    if (!m_refTab)
    {
        m_refTab = new RefTab(0, 0);

        if (m_refTab)
        {
            m_refTab->m_blockCapacity = 8;
            m_refTab->m_count = 0;
            m_refTab->m_size = 1;
            m_refTab->m_poolSize = 8;
            m_refTab->m_head = 0;
            m_refTab->m_tail = 0;
        }
        else
        {
            m_refTab = 0;
        }
    }

    m_refTab->Add((UINT)l_prev);

    if ((int)p_refRun->next <= 0)
        p_refRun->prev = l_prev->next;
    else
        p_refRun->prev = l_prev->prev;

    if (l_prev->prev)
        l_prev->prev->next = l_prev->next;
    else
        m_next = l_prev->next;

    RefRun *l_next = l_prev->next;

    if (l_next)
        l_next->prev = l_prev->prev;
    else
        m_prev = l_prev->prev;

    l_prev->next = 0;
    l_prev->prev = 0;
}

void LinkRefTab::RunInitNxtRef(RefRun *p_refRun)
{
    p_refRun->next = (RefRun *)1;
    p_refRun->prev = 0;
    m_poolSize &= ~0x80000000;
}

void LinkRefTab::RunInitPrevRef(RefRun *p_refRun)
{
    p_refRun->next = (RefRun *)-1;
    p_refRun->prev = 0;
    m_poolSize &= ~0x80000000;
}

UINT *LinkRefTab::RunNxtRefPtr(RefRun *p_refRun)
{
    if (m_poolSize < 0)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1007);
        l_sysCom->LogMessage("ERROR: Illegal operation inside LINKREFTAB loop");
    }

    if (p_refRun->prev)
        p_refRun->prev = p_refRun->prev->next;
    else
        p_refRun->prev = m_next;

    UINT *l_result = (UINT *)p_refRun->prev;

    if (p_refRun->prev)
        l_result += 2 - m_size;

    return l_result;
}

UINT *LinkRefTab::RunPrevRefPtr(RefRun *p_refRun)
{
    if (m_poolSize < 0)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1025);
        l_sysCom->LogMessage("ERROR: Illegal operation inside LINKREFTAB loop");
    }

    if (p_refRun->prev)
        p_refRun->prev = p_refRun->prev->prev;
    else
        p_refRun->prev = m_prev;

    UINT *l_result = (UINT *)p_refRun->prev;

    if (p_refRun->prev)
        l_result += 2 - m_size;

    return l_result;
}

UINT *LinkRefTab::RunToRefPtr(RefRun *p_refRun)
{
    UINT *l_result = (UINT *)p_refRun->prev;

    if (p_refRun->prev)
        l_result += 2 - m_size;

    return l_result;
}

// Added
UINT *LinkRefTab::AddStart(UINT p_refNum)
{
    UINT *l_nextRefNum;
    RefRun l_refRun;

    if (m_refTab)
    {
        m_refTab->RunInitNxtRef(&l_refRun);
        UINT l_nextRef = m_refTab->RunNxtRef(&l_refRun);

        if (l_refRun.prev)
        {
            l_nextRefNum = (UINT *)(l_nextRef + 4 * (2 - m_size));

            m_refTab->RunDelRef(&l_refRun);
        }
        else
        {
            l_nextRefNum = (UINT *)p_refNum;
        }

        *l_nextRefNum = p_refNum;

        if (!m_refTab->GetCount())
        {
            if (m_refTab)
                m_refTab->~RefTab();

            m_refTab = 0;
        }
    }
    else
    {
        l_nextRefNum = RefTab::Add(p_refNum) - 1;
    }

    RefRun *l_data = (RefRun *)&l_nextRefNum[m_size - 2];

    l_data->prev = 0;
    l_data->next = 0;

    l_data->next = m_next;
    l_data->prev = 0;

    RefRun *l_next = m_next;

    if (l_next)
        l_next->prev = l_data;
    else
        m_prev = l_data;

    m_next = l_data;

    return l_nextRefNum + 1;
}

void LinkRefTab::AddEnd(UINT p_refNum)
{
    AddUnique(p_refNum);
}

UINT *LinkRefTab::InsertBefore(UINT *p_uintPtr, UINT p_refNum)
{
    UINT *l_nextRefNum;
    RefRun l_refRun;

    if (m_refTab)
    {
        m_refTab->RunInitNxtRef(&l_refRun);
        UINT l_nextRef = m_refTab->RunNxtRef(&l_refRun);

        if (l_refRun.prev)
        {
            l_nextRefNum = (UINT *)(l_nextRef + 4 * (2 - m_size));
            m_refTab->RunDelRef(&l_refRun);
        }
        else
        {
            l_nextRefNum = (UINT *)p_refNum;
        }

        *l_nextRefNum = p_refNum;

        if (!m_refTab->GetCount())
        {
            if (m_refTab)
                m_refTab->~RefTab();

            m_refTab = 0;
        }
    }
    else
    {
        l_nextRefNum = RefTab::Add(p_refNum) - 1;
    }

    RefRun *l_data = (RefRun *)&l_nextRefNum[m_size - 2];

    l_data->prev = 0;
    l_data->next = 0;
    UINT *l_data2 = &p_uintPtr[m_size - 2];

    if (&p_uintPtr[m_size] == (UINT *)8)
    {
        l_data->prev = m_prev;
        l_data->next = 0;
        RefRun *l_prev = m_prev;

        if (l_prev)
            l_prev->next = l_data;
        else
            m_next = l_data;

        m_prev = l_data;

        return l_nextRefNum + 1;
    }
    else
    {
        RefRun *l_dataRefRun = (RefRun *)*l_data2;
        bBool l_check = *l_data2 == 0;
        l_data->prev = (RefRun *)*l_data2;

        l_data->next = (RefRun *)l_data2;

        if (l_check)
            m_next = l_data;
        else
            l_dataRefRun->next = l_data;

        *l_data2 = (UINT)l_data;

        return l_nextRefNum + 1;
    }
}

UINT *LinkRefTab::GetPrevRefPtr(UINT *p_refPtr)
{
    UINT l_refNum = p_refPtr[m_size - 2];

    if (l_refNum)
        return (UINT *)(l_refNum + 4 * (2 - m_size));
    else
        return 0;
}

UINT *LinkRefTab::GetNextRefPtr(UINT *p_refPtr)
{
    UINT l_refPtr = p_refPtr[m_size - 1];

    if (l_refPtr)
        return (UINT *)(l_refPtr + 4 * (2 - m_size));
    else
        return 0;
}