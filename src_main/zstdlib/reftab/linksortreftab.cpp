#include "linksortreftab.h"

LinkSortRefTab::LinkSortRefTab(i32 p_poolSize, i32 p_size) : LinkRefTab(p_poolSize, p_size + 1)
{
    m_curElement = 0;
}

LinkSortRefTab::~LinkSortRefTab() {}

void LinkSortRefTab::Clear()
{
    RefRun *l_refRun;
    RefTab *l_refTab = m_refTab;

    m_curElement = 0;

    if (l_refTab)
        l_refTab->~RefTab();

    RefRun *l_head = m_head;
    i32 p_poolSize = m_poolSize | 0x80000000;
    m_refTab = 0;
    m_next = 0;
    m_prev = 0;
    m_poolSize = p_poolSize;

    while (l_head)
    {
        l_refRun = l_head;
        l_head = l_head->next;
        DeleteBlock(l_refRun);
    }

    m_count = 0;
    m_head = 0;
}

void LinkSortRefTab::ClearThis()
{
    m_curElement = 0;
}

void LinkSortRefTab::DelRefPtr(u32 *p_refPtr)
{
    if (m_curElement == p_refPtr)
        m_curElement = 0;

    LinkRefTab::DelRefPtr(p_refPtr);
}

void LinkSortRefTab::PrintStatus()
{
    RefRun l_refRun;

    if (this)
    {
        RunInitNxtRef(&l_refRun);

        for (u32 *l_item = RunNxtRefPtr(&l_refRun); l_item; l_item = RunNxtRefPtr(&l_refRun))
        {
            float l_sortValue = l_item[m_size - 3];

            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1169);
            l_sysCom->LogMessage("Ref %x Sort Value %f", *l_item, l_sortValue);
        }
    }
}

void LinkSortRefTab::RunDelRef(RefRun *p_refRun)
{
    if (m_curElement == RunToRefPtr(p_refRun))
        m_curElement = 0;

    LinkRefTab::RunDelRef(p_refRun);
}

float *LinkSortRefTab::AddSort(u32 p_refNum, float p_sortKey, i32 p_insertMode)
{
    float *l_dataPtr;
    u32 *l_curElement = m_curElement;

    if (l_curElement)
    {
        // First scan backwards until we find an element with sort key <= our new key
        do
        {
            if (p_sortKey >= *(float *)&l_curElement[m_size - 3])
                break;

            l_curElement = GetPrevRefPtr(l_curElement);
        } while (l_curElement);

        // For insert mode 1 (ascending order)
        if (p_insertMode == 1)
        {
            if (!l_curElement)
            {
            ADD_TO_START:
                l_dataPtr = (float *)AddStart(p_refNum);

            SET_SORT_KEY_AND_RETURN:
                l_dataPtr[m_size - 4] = p_sortKey;
                m_curElement = (u32 *)(l_dataPtr - 1);
                return l_dataPtr;
            }

            while (p_sortKey <= *(float *)&l_curElement[m_size - 3])
            {
                l_curElement = GetPrevRefPtr(l_curElement);

                if (!l_curElement)
                    goto ADD_TO_START;
            }
        }

        if (!l_curElement)
            goto ADD_TO_START;
    }
    else
    {
        RefRun **l_next = &m_next->prev;

        if (!l_next)
            goto CHECK_INSERT_MODE;

        l_curElement = (u32 *)&l_next[2 - m_size];

        if (!l_curElement)
            goto CHECK_INSERT_MODE;
    }

    do
    {
        if (*(float *)&l_curElement[m_size - 3] >= p_sortKey)
            break;

        l_curElement = GetNextRefPtr(l_curElement);
    } while (l_curElement);

CHECK_INSERT_MODE:
    // For insert mode 2 (descending order)
    if (p_insertMode == 2)
    {
        if (!l_curElement)
        {
        ADD_TO_END:
            l_dataPtr = (float *)AddEnd(p_refNum);
            goto SET_SORT_KEY_AND_RETURN;
        }

        while (*(float *)&l_curElement[m_size - 3] <= p_sortKey)
        {
            l_curElement = GetNextRefPtr(l_curElement);
            if (!l_curElement)
                goto ADD_TO_END;
        }
    }

    if (!l_curElement)
        goto ADD_TO_END;

    l_dataPtr = (float *)InsertBefore(l_curElement, p_refNum);
    l_dataPtr[m_size - 4] = p_sortKey;
    m_curElement = (u32 *)(l_dataPtr - 1);

    return l_dataPtr;
}

float LinkSortRefTab::GetSort(u32 *p_refPtr)
{
    return *(float *)&p_refPtr[m_size - 3];
}

void LinkSortRefTab::SetSortNr(u32 *p_refPtr, float p_sortKey)
{
    memcpy((AddSort(*p_refPtr, p_sortKey, 0) - 4), p_refPtr, 4 * m_size - 16);

    DelRefPtr(p_refPtr);
}