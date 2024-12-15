#include "reftab.h"
#include "allocreftab.h"

AllocRefTab::AllocRefTab()
{
    m_refShitAmount = 18;
    m_maxRefIndex = 0x3FFFF;

    m_refTab = new RefTab(128, 0);

    m_array = new u32[0x40000];

    for (u32 i = 0; i != 0x40000; ++i)
    {
        m_array[i] = 1;
    }

    m_nextFreeRef = 0;
}

AllocRefTab::~AllocRefTab()
{
    delete[] m_array;

    if (m_refTab)
    {
        delete m_refTab;
    }
}

u32 AllocRefTab::NewRef()
{
    u32 l_nextRef;
    u32 l_arrayRef;
    u32 l_result;
    RefRun l_refRun;

    if (m_refTab && (m_refTab->RunInitNxtRef(&l_refRun), l_nextRef = m_refTab->RunNxtRef(&l_refRun), l_refRun.prev))
    {
        while (1)
        {
            m_refTab->RunDelRef(&l_refRun);

            l_arrayRef = (m_array[l_nextRef - 1] << m_refShitAmount) | (l_nextRef - 1);

            if ((~m_maxRefIndex & l_arrayRef) != 0)
                return l_arrayRef;

            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1381);
            l_sysCom->LogMessage("Ref %x usedup\n", l_arrayRef & m_maxRefIndex);

            l_nextRef = m_refTab->RunNxtRef(&l_refRun);

            if (!l_refRun.prev)
                goto ALLOCATE_NEW_REF;
        }
    }
    else
    {

    ALLOCATE_NEW_REF:
        if (m_nextFreeRef > m_maxRefIndex)
            g_pSysCom->ThrowFatal("AllocRefTab too small %d\n", m_maxRefIndex + 1);

        l_result = m_nextFreeRef | (1 << m_refShitAmount);

        m_nextFreeRef = m_nextFreeRef + 1;
    }

    return l_result;
}

bool AllocRefTab::FreeRef(u32 p_refNum)
{
    if ((p_refNum & 0xFFFC0000) != 0)
    {
        if (CheckRefActive(p_refNum))
        {
            u32 *l_arrayRef = &m_array[m_maxRefIndex & p_refNum];

            ++*l_arrayRef;

            m_refTab->Add((m_maxRefIndex & p_refNum) + 1);
            return 1;
        }
        else
        {
            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1397);
            l_sysCom->LogMessage("Tried to free unallocated REF %x", p_refNum);

            PrintRef(p_refNum);
            return 0;
        }
    }
    else
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1392);
        l_sysCom->LogMessage("Tried to free non valid ref REF %x", p_refNum);
        return 0;
    }
}

bool AllocRefTab::CheckRefActive(u32 p_refNum)
{
    return m_array[p_refNum & m_maxRefIndex] == p_refNum >> m_refShitAmount;
}

void AllocRefTab::PrintRef(u32 p_refNum)
{
    ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1365);
    l_sysCom->LogMessage("REF %x lo=%d hi=%d\n", p_refNum, p_refNum & m_maxRefIndex, p_refNum >> m_refShitAmount);
}
