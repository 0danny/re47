#include "equreftab.h"

EquRefTab::EquRefTab(i32 p_poolSize, i32 p_size) : RefTab(p_poolSize, p_size + 1) {}

EquRefTab::~EquRefTab()
{
    Destroy();
}

void EquRefTab::Destroy()
{
    RefRun *l_head = m_head;
    i32 l_poolRes = m_poolSize | 0x80000000;

    m_poolSize = l_poolRes;

    while (l_head)
    {
        l_head = l_head->next;
        DeleteBlock(l_head);
    }

    m_count = 0;
    m_head = 0;
}

void EquRefTab::PrintStatus()
{
    if (this)
    {
        RefRun l_refRun;
        u32 *l_nxtRef;

        RunInitNxtRef(&l_refRun);

        for (l_nxtRef = RunNxtRefPtr(&l_refRun); l_nxtRef; l_nxtRef = RunNxtRefPtr(&l_refRun))
        {
            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 828);
            l_sysCom->LogMessage("Ref %x %d\n", *l_nxtRef, l_nxtRef[m_size - 1]);
        }
    }
}

u32 *EquRefTab::FindEqu(i32 p_value)
{
    if (!this)
        return 0;

    RefRun l_refRun;
    RunInitNxtRef(&l_refRun);

    u32 *l_nxtRef = RunNxtRefPtr(&l_refRun);

    if (!l_nxtRef)
        return 0;

    while (1)
    {
        u32 l_data = l_nxtRef[m_size - 1];

        if (l_data != -1 && p_value == l_data)
            break;

        l_nxtRef = RunNxtRefPtr(&l_refRun);

        if (!l_nxtRef)
            return 0;
    }

    return l_nxtRef;
}

u32 *EquRefTab::AddEqu(u32 p_refNum, i32 p_value)
{
    u32 *l_add;

    l_add = RefTab::Add(p_refNum);
    l_add[this->m_size - 2] = p_value;

    return l_add;
}
