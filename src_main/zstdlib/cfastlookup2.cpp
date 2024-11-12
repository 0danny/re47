#include "cfastlookup2.h"

CFastLookup2::CFastLookup2(i32 p_poolSize)
{
    m_valTree = new ZValTree(p_poolSize);
}

CFastLookup2::~CFastLookup2()
{
    Destroy();
}

void CFastLookup2::Destroy()
{
    Clear();

    if (m_valTree)
    {
        m_valTree->~ZValTree();
        delete m_valTree;
    }
}

void CFastLookup2::Clear()
{
    RefRun l_refRun;

    while (1)
    {
        SBinTreeNode *l_binNode = m_valTree->m_rootNode;

        if (l_binNode == m_valTree->m_nullNode || !l_binNode)
            break;

        LinkRefTab *l_link = (LinkRefTab *)l_binNode->data;

        if (l_link)
        {
            l_link->RunInitNxtRef(&l_refRun);

            for (CFastLink *l_item = (CFastLink *)l_link->RunNxtRefPtr(&l_refRun); l_item; l_item = (CFastLink *)l_link->RunNxtRefPtr(&l_refRun))
            {
                if (l_item->isAllocated)
                {
                    delete l_item->stringPtr;
                }
            }

            l_link->~LinkRefTab();

            m_valTree->Delete(l_binNode);
        }
        else
        {
            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\ZStdLib\\Source\\FastLookup.cpp", 340);
            l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\ZStdLib\\Source\\FastLookup.cpp", 340);
            DebugBreak();

            m_valTree->Delete(l_binNode);
        }
    }
}

void CFastLookup2::Reset()
{
    Clear();
}

void CFastLookup2::Remove(const char *p_str, i32 p_size)
{
    RefRun l_refRun;

    const char *l_str = p_str;
    i32 l_strSize = p_size;

    if (!p_size)
    {
        p_size = strlen(p_str);
        l_strSize = p_size;
    }

    i32 l_count = l_strSize >> 2;
    i32 l_key = l_strSize;

    if (l_strSize >> 2)
    {
        char *l_strPtr = (char *)p_str;

        do
        {
            l_key += *(i32 *)l_strPtr;
            l_strPtr += 4;
            --l_count;
        } while (l_count);
    }

    for (u32 l_strCount = l_strSize & 0xFFFFFFFC; l_strCount != l_strSize; ++l_strCount)
        l_key += p_str[l_strCount];

    SBinTreeNode *l_searchResult = m_valTree->Search(l_key, 0);

    if (l_searchResult)
    {
        LinkRefTab *l_data = (LinkRefTab *)l_searchResult->data;

        if (l_data)
        {
            l_data->RunInitNxtRef(&l_refRun);

            for (CFastLink *l_item = (CFastLink *)l_data->RunNxtRefPtr(&l_refRun); l_item; l_item = (CFastLink *)l_data->RunNxtRefPtr(&l_refRun))
            {
                if (l_item->stringLength == l_strSize)
                {

                    if (!memcmp(l_item->stringPtr, l_str, l_strSize))
                    {
                        if (l_item->isAllocated)
                        {
                            delete l_item->stringPtr;
                        }

                        l_data->DelRefPtr((u32 *)l_item);

                        if (!l_data->GetCount())
                        {
                            l_data->~LinkRefTab();
                            m_valTree->Delete(l_searchResult);
                        }
                        return;
                    }

                    l_strSize = p_size;
                    l_str = p_str;
                }
            }
        }
    }
}

CFastLink *CFastLookup2::FindFastLink(LinkRefTab *p_linkTab, const char *p_str, i32 p_size)
{
    RefRun l_refRun;

    if (!p_linkTab)
        return 0;

    p_linkTab->RunInitNxtRef(&l_refRun);
    CFastLink *l_result = (CFastLink *)p_linkTab->RunNxtRefPtr(&l_refRun);

    if (!l_result)
        return 0;

    do
    {
        if (l_result->stringLength == p_size && !memcmp(l_result->stringPtr, p_str, p_size))
            break;

        l_result = (CFastLink *)p_linkTab->RunNxtRefPtr(&l_refRun);

    } while (l_result);

    return l_result;
}

inline void CFastLookup2::Set(const char *p_str, u32 p_ref)
{
    CFastLink *l_fastLinkPtr;

    i32 l_strLen = strlen(p_str) + 1;
    char *l_buffer = new char[l_strLen];

    char *l_str = l_buffer;
    strcpy(l_buffer, p_str);

    i32 l_size = strlen(l_buffer);

    i32 l_count = l_size >> 2;
    i32 l_key = l_size;

    if (l_size >> 2)
    {
        char *l_strPtr = l_str;

        do
        {
            l_key += *l_strPtr;
            l_strPtr += 4;
            --l_count;
        } while (l_count);
    }

    for (u32 l_checkSumIter = l_size & 0xFFFFFFFC; l_checkSumIter != l_size; ++l_checkSumIter)
        l_key += l_str[l_checkSumIter];

    LinkRefTab *l_val = (LinkRefTab *)m_valTree->GetKeyVal(l_key);

    if (l_val)
    {
        CFastLink *l_fastLink = FindFastLink(l_val, l_str, l_size);
        l_fastLinkPtr = l_fastLink;

        if (l_fastLink)
        {
            if (l_fastLink->isAllocated)
            {
                char *l_unkStr = l_fastLink->stringPtr;

                delete l_unkStr;

                l_fastLinkPtr->stringPtr = 0;
            }
        }
        else
        {
            l_fastLinkPtr = (CFastLink *)(l_val->AddUnique(0) - 1);
        }
    }
    else
    {
        LinkRefTab *l_linkRef = new LinkRefTab(128, 3);

        m_valTree->InsertKey(l_key, (i32)l_linkRef);
        l_fastLinkPtr = (CFastLink *)(l_linkRef->AddUnique(0) - 1);
    }

    l_fastLinkPtr->stringLength = l_size;
    l_fastLinkPtr->stringPtr = l_str;
    l_fastLinkPtr->reference = p_ref;
    l_fastLinkPtr->isAllocated = 1;
}

inline u32 CFastLookup2::Get(const char *p_str)
{
    RefRun l_refRun;

    u32 l_reference;
    u32 l_pointerSize = strlen(p_str) + 1;
    u32 l_pointerSizeMinus1 = l_pointerSize - 1;
    i32 l_count = (l_pointerSize - 1) >> 2;
    i32 l_key = l_pointerSize - 1;

    CFastLink *l_nextRef;

    if (l_count)
    {
        const char *l_ptrIncrement = p_str;

        do
        {
            l_key += *l_ptrIncrement;
            l_ptrIncrement += 4;
            --l_count;
        } while (l_count);
    }

    for (u32 l_keyItem = l_pointerSizeMinus1 & 0xFFFFFFFC; l_keyItem != l_pointerSizeMinus1; ++l_keyItem)
        l_key += p_str[l_keyItem];

    LinkRefTab *l_initRef = (LinkRefTab *)this->m_valTree->GetKeyVal(l_key);

    if (l_initRef && (l_initRef->RunInitNxtRef(&l_refRun), (l_nextRef = (CFastLink *)l_initRef->RunNxtRefPtr(&l_refRun)) != 0))
    {
        while (l_nextRef->stringLength != l_pointerSizeMinus1 || memcmp(l_nextRef->stringPtr, p_str, l_pointerSize - 1))
        {
            l_nextRef = (CFastLink *)l_initRef->RunNxtRefPtr(&l_refRun);

            if (!l_nextRef)
                goto NULL_LABEL;
        }

        l_reference = l_nextRef->reference;
    }
    else
    {
    NULL_LABEL:
        l_reference = 0;
    }

    return l_reference;
}

void CFastLookup2::RemoveLowerCase(const char *p_str, u32 p_size)
{
    u32 l_size = p_size;

    if (!p_size)
        l_size = strlen(p_str);

    // This is an inlined constructor, inferred from debug symbols of Hitman 2.
    MyStr l_myStr(p_str);

    const char *l_pointer = *l_myStr;

    l_myStr.ToLower();

    Remove(l_pointer, l_size);
}

void CFastLookup2::SetLowerCase(const char *p_str, u32 p_ref)
{
    MyStr l_myStr(p_str);
    l_myStr.ToLower();

    const char *l_pointer = *l_myStr;

    Set(l_pointer, p_ref);
}

u32 CFastLookup2::GetLowerCase(const char *p_str)
{
    MyStr l_myStr(p_str);
    l_myStr.ToLower();

    const char *l_pointer = *l_myStr;

    u32 l_reference = Get(l_pointer);

    return l_reference;
}
