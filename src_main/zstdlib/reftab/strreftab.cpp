#include "strreftab.h"

StrRefTab::StrRefTab(i32 p_poolSize, i32 p_size) : RefTab(p_poolSize, p_size) {}

StrRefTab::~StrRefTab()
{
    Destroy();
}

void StrRefTab::Destroy()
{
    if (this)
    {
        RefRun l_refRun;

        RunInitNxtRef(&l_refRun);

        for (u32 i = RunNxtRef(&l_refRun); l_refRun.prev; i = RunNxtRef(&l_refRun))
        {
            delete[] (char *)i;
        }
    }

    RefRun *l_head = m_head;

    m_poolSize |= 0x80000000;

    while (l_head)
    {
        l_head = l_head->next;

        DeleteBlock(l_head);
    }

    m_count = 0;
    m_head = 0;
}

void StrRefTab::Clear()
{
    ClearThis();

    RefRun *l_head = m_head;

    m_poolSize |= 0x80000000;

    while (l_head)
    {
        l_head = l_head->next;

        DeleteBlock(l_head);
    }

    m_count = 0;
    m_head = 0;
}

void StrRefTab::ClearThis()
{
    if (this)
    {
        RefRun l_refRun;

        RunInitNxtRef(&l_refRun);

        for (u32 i = RunNxtRef(&l_refRun); l_refRun.prev; i = RunNxtRef(&l_refRun))
        {
            delete[] (char *)i;
        }
    }

    m_poolSize |= 0x80000000;
}

void StrRefTab::PrintStatus()
{
    if (this)
    {
        RefRun l_refRun;

        RunInitNxtRef(&l_refRun);

        for (u32 i = RunNxtRef(&l_refRun); l_refRun.prev; i = RunNxtRef(&l_refRun))
        {
            ZSysCom *l_com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1320);
            l_com->LogMessage("String %s\n", i);
        }
    }
}

void StrRefTab::RunDelRef(RefRun *p_refRun)
{
    char *l_dataPtr;

    if (p_refRun->usedUnits <= 0)
    {
        l_dataPtr = (char *)*RunToRefPtr(p_refRun);
    }
    else
    {
        i32 l_size = m_size;

        i32 l_dataOffset = (i32)p_refRun->next - l_size;

        RefRun *l_prev = p_refRun->prev;

        if (l_dataOffset < 0)
        {
            l_prev = l_prev->prev;
            l_dataOffset = m_blockCapacity - l_size;
        }

        l_dataPtr = (char *)l_prev->data[l_dataOffset];
    }

    delete[] l_dataPtr;

    RefTab::RunDelRef(p_refRun);
}

char *StrRefTab::FindStr(char *const p_str)
{
    if (!this)
        return 0;

    RefRun l_refRun;

    RunInitNxtRef(&l_refRun);

    char *l_nxtRef = (char *)(*RunNxtRefPtr(&l_refRun));

    if (!l_nxtRef)
        return 0;

    while (_strcmpi(l_nxtRef, p_str))
    {
        l_nxtRef = (char *)(*RunNxtRefPtr(&l_refRun));

        if (!l_nxtRef)
            return 0;
    }

    return l_nxtRef;
}

char *StrRefTab::AddStr(char *const p_str)
{
    if (Exists(p_str))
        return 0;
    else
        return AddAlways(p_str);
}

char *StrRefTab::AddAlways(char *const p_str)
{
    i32 l_strLen = strlen(p_str) + 1;
    char *l_malloc = new char[l_strLen];

    strcpy(l_malloc, p_str);

    return (char *)(*RefTab::Add((u32)l_malloc));
}

void StrRefTab::RemoveStr(char *const p_str)
{
    char *l_nxtRef;
    RefRun l_refRun;

    if (this && (RunInitNxtRef(&l_refRun), l_nxtRef = (char *)RunNxtRef(&l_refRun), l_refRun.prev))
    {
        while (1)
        {
            i32 l_cmp = _strcmpi(l_nxtRef, p_str);

            if (!l_cmp)
                break;

            l_nxtRef = (char *)RunNxtRef(&l_refRun);

            if (!l_refRun.prev)
                goto UNABLE_LABEL;
        }

        RunDelRef(&l_refRun);
        m_poolSize |= 0x80000000;
    }
    else
    {
    UNABLE_LABEL:
        ZSysCom *l_com = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\RefTab.cpp", 1267);
        l_com->LogMessage("WARNING: unable to remove string %s\n", p_str);
    }
}

boolean StrRefTab::Exists(char *const p_str)
{
    if (!this)
        return 0;

    RefRun l_refRun;
    RunInitNxtRef(&l_refRun);

    char *l_nxtRef = (char *)RunNxtRef(&l_refRun);

    if (!l_refRun.prev)
        return 0;

    while (_strcmpi(l_nxtRef, p_str))
    {
        l_nxtRef = (char *)RunNxtRef(&l_refRun);

        if (!l_refRun.prev)
            return 0;
    }

    return 1;
}

void StrRefTab::Sort()
{
    i32 l_mallocCount = sizeof(char *) * GetCount();

    char **l_allocated = (char **)new char[l_mallocCount];

    size_t l_numOfElements = 0;

    if (this)
    {
        RefRun l_refRun;
        RunInitNxtRef(&l_refRun);

        char *l_nxtRef = (char *)RunNxtRef(&l_refRun);

        if (l_refRun.prev)
        {
            char **l_allocPtr = l_allocated;

            do
            {
                *l_allocPtr = l_nxtRef;
                ++l_numOfElements;
                ++l_allocPtr;

                l_nxtRef = (char *)RunNxtRef(&l_refRun);

            } while (l_refRun.prev);
        }
    }

    qsort(l_allocated, l_numOfElements, sizeof(char *), StrRefTab::Compare);

    RefRun *l_head = m_head;
    m_poolSize |= 0x80000000;

    while (l_head)
    {
        l_head = l_head->next;
        DeleteBlock(l_head);
    }

    m_count = 0;
    m_head = 0;

    if (l_numOfElements)
    {
        char **l_allocPtr2 = l_allocated;

        do
        {
            AddAlways(*l_allocPtr2);

            delete[] *l_allocPtr2;

            ++l_allocPtr2;
            --l_numOfElements;

        } while (l_numOfElements);
    }

    delete[] l_allocated;
}

i32 StrRefTab::Compare(const void *p_a, const void *p_b)
{
    return StrRefTab::CompareCall(*(char **)p_a, *(char **)p_b) != 0 ? 1 : -1;
}

boolean StrRefTab::CompareCall(const char *p_str1, const char *p_str2)
{
    if (!_strcmpi(p_str1, p_str2))
        return 0;

    u32 l_strLen1 = strlen(p_str1);
    u32 l_strLen2 = strlen(p_str2) + 1;

    if (l_strLen1 > (l_strLen2 - 1))
        l_strLen1 = l_strLen2 - 1;

    i32 l_index = 0;

    if (!l_strLen1)
        return p_str2[l_index] == 0;

    const char *l_str2Ptr = p_str2;
    i32 l_index2 = p_str1 - p_str2;

    while (1)
    {
        char l_test1 = l_str2Ptr[l_index2];

        char l_test2 = *l_str2Ptr;

        if (l_test1 >= 'A' && l_test1 <= 'Z')
            l_test1 += ' ';

        if (l_test2 >= 'A' && l_test2 <= 'Z')
            l_test2 += ' ';

        if (l_test1 > l_test2)
            break;

        if (l_test1 == l_test2)
        {
            ++l_index;
            ++l_str2Ptr;

            if (l_index != l_strLen1)
                continue;
        }

        return p_str2[l_index] == 0;
    }

    return 1;
}
