#include "zinputactions.h"
#include "zinputaction.h"

ZInputActions::ZInputActions()
{
    // Yes this is intentional,
    // You would think that it would set the total count of the keys input list to 165,
    // but instead it sets it to 0 and sets the mouse input list count to 165.
    if (g_inputLists[0].list)
    {
        SInputList *l_curList = &g_inputLists[0];

        i32 l_totalCount = 0;

        do
        {
            l_curList->totalCount = l_totalCount;
            l_totalCount += l_curList->listSize;
            l_curList++;

        } while (l_curList->list);
    }

    i32 l_mainNodeCount = 0;

    if (g_inputLists[0].list)
    {
        SInputList *l_curList = &g_inputLists[0];

        do
        {
            l_mainNodeCount += l_curList->listSize;
            l_curList++;

        } while (l_curList->list);
    }

    m_inputNodeCount = l_mainNodeCount;

    m_curStates = new u8[m_inputNodeCount];
    memset(m_curStates, 0, m_inputNodeCount);

    SInputNodeList *l_listAlloc = new SInputNodeList;
    l_listAlloc->count = m_inputNodeCount;
    l_listAlloc->nodes = new SInputNode[m_inputNodeCount];

    m_inputNodeList = l_listAlloc->nodes;

    if (m_inputNodeCount)
    {
        i32 l_count = 0;
        i32 l_nodeIndex = 0;

        do
        {
            m_inputNodeList[l_nodeIndex].m_parent = this;

            ++l_count;
            ++l_nodeIndex;

        } while (l_count != m_inputNodeCount);
    }

    m_actionMapDefinitions = new RefTab(4, 0);
    m_unkInt2 = 0;

    m_refTab2 = new RefTab(4, 0);
    m_unkFloat3 = 0.0;
    m_unkFloat1 = g_pSysInterface->m_unkInt70;
    m_unkFloat2 = 0.0;

    m_fastLookup = new CFastLookup2(32);
    m_unkInt3 = 0;
    m_actionMap = 0;
    m_overrideList = 0;
}

ZInputActions::~ZInputActions()
{
    RefRun l_refRun;

    RemoveOverrideList();

    if (m_refTab2)
        m_refTab2->~RefTab();

    if (m_actionMapDefinitions)
    {
        m_actionMapDefinitions->RunInitNxtRef(&l_refRun);
        for (ZActionMap *l_item = (ZActionMap *)m_actionMapDefinitions->RunNxtRef(&l_refRun); l_refRun.prev; l_item = (ZActionMap *)m_actionMapDefinitions->RunNxtRef(l_refRun.next))
        {
            m_actionMapDefinitions->RunDelRef(&l_refRun);

            if (l_item)
            {
                l_item->~ZActionMap();
            }
        }
    }

    if (m_actionMapDefinitions)
        m_actionMapDefinitions->~RefTab();

    if (m_inputNodeList)
    {
        for (i32 i = 0; i < m_inputNodeCount; ++i)
        {
            m_inputNodeList[i].~SInputNode();
        }

        delete[] m_inputNodeList;
    }

    if (m_curStates)
        delete m_curStates;

    if (m_fastLookup)
        m_fastLookup->~CFastLookup2();
}

void ZInputActions::OverrideActions(RefTab *l_actionsList)
{
    printf("[HOOK]: ZInputActions::OverrideActions called\n");

    RefRun l_refRun;

    if (l_actionsList)
    {
        l_actionsList->RunInitNxtRef(&l_refRun);

        for (SActionOverride *l_item = (SActionOverride *)l_actionsList->RunNxtRefPtr(&l_refRun); l_item; l_item = (SActionOverride *)l_actionsList->RunNxtRefPtr(&l_refRun))
        {
            OverrideAction(l_item);
        }
    }
}

void ZInputActions::OverrideAction(SActionOverride *p_actionOverride)
{
    printf("[HOOK]: ZInputActions::OverrideAction called -> %s\n", p_actionOverride->actionName.m_pointer);

    /*
    typedef void(__fastcall * OverrideAction_t)(ZInputActions * _this, void *EDX, SActionOverride *p_actionOverride);
    OverrideAction_t OverrideAction = (OverrideAction_t)0x0FFA14A0;
    OverrideAction(this, 0, p_actionOverride);
    return;*/

    RefRun l_refRun;
    SActionOverride *l_actionOverridePtr;
    SActionOverride *l_actionOverride;

    if (!m_overrideList)
    {
        m_overrideList = new RefTab(32, 39);
        goto MAKE_NEW_OVERRIDE;
    }

    m_overrideList->RunInitNxtRef(&l_refRun);
    l_actionOverride = (SActionOverride *)m_overrideList->RunNxtRefPtr(&l_refRun);

    if (!l_actionOverride)
    {
    MAKE_NEW_OVERRIDE:
        l_actionOverridePtr = (SActionOverride *)(m_overrideList->Add(0) - 1);

        if (l_actionOverridePtr)
        {
            l_actionOverridePtr->actionName.m_pointer = l_actionOverridePtr->actionName.m_buffer;
            l_actionOverridePtr->actionName.m_buffer[0] = '\0';
        }
        else
        {
            l_actionOverridePtr = 0;
        }

        l_actionOverride = l_actionOverridePtr;

        goto COPY_OVERRIDE;
    }

    while (strcmp(l_actionOverride->actionName.m_pointer, p_actionOverride->actionName.m_pointer))
    {
        l_actionOverride = (SActionOverride *)m_overrideList->RunNxtRefPtr(&l_refRun);

        if (!l_actionOverride)
            goto MAKE_NEW_OVERRIDE;
    }

COPY_OVERRIDE:
    l_actionOverride->unkByte1 = p_actionOverride->unkByte1;
    l_actionOverride->unkInt7 = p_actionOverride->unkInt7;
    l_actionOverride->unkInt8 = p_actionOverride->unkInt8;

    SInputList *l_keysInputList = &g_inputLists[0];
    SInputList *l_keysInputListPtr = l_keysInputList;

    i32 l_index = 0;
    i32 *l_arrayPtr = p_actionOverride->unkArray;
    i32 l_offset = (char *)l_actionOverride - (char *)p_actionOverride;

    while (*l_arrayPtr)
    {
        i32 listSize = l_keysInputList->listSize;
        i32 l_list = 0;
        i32 l_loopCount = 0;
        i32 l_finalIndex = 0;

        if (listSize)
        {
            for (i32 l_list = 0; l_list < listSize; ++l_list)
            {
                if (l_keysInputListPtr->list[l_list].index == *l_arrayPtr)
                    break;
            }

            if (l_list == listSize)
                goto NEGATIVE_INDEX;

            l_finalIndex = l_list + l_keysInputListPtr->totalCount;
        }
        else
        {
        NEGATIVE_INDEX:
            l_finalIndex = -1;
        }

        *(i32 *)((char *)l_arrayPtr + l_offset) = l_finalIndex;

        if (l_finalIndex == -1)
        {
            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 173);
            l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\source\\Actions.cpp", 173);
            DebugBreak();
        }

        ++l_arrayPtr;

        if (++l_index == 4)
            goto OVERRIDE_ACTION;

        l_keysInputList = l_keysInputListPtr;
    }

    l_actionOverride->unkArray[l_index] = -1;

OVERRIDE_ACTION:
    if (l_actionOverride != p_actionOverride)
        l_actionOverride->actionName = p_actionOverride->actionName;

    ZInputAction *l_inputAction = GetInputAction(l_actionOverride->actionName.m_pointer);
    l_actionOverride->handlerType = 0;

    if (l_inputAction)
    {
        if (l_inputAction->m_pressHandlers)
            l_actionOverride->handlerType = 1;

        if (l_inputAction->m_holdHandlers)
            l_actionOverride->handlerType = 2;

        if (l_inputAction->m_releaseHandlers)
            l_actionOverride->handlerType = 3;

        l_inputAction->m_actionMap->RemoveAction(l_actionOverride->actionName.m_pointer);
        l_inputAction->m_actionMap->AddAction(l_actionOverride)->m_lockCount = l_inputAction->m_lockCount;
    }
}

SActionOverride *ZInputActions::FindOverriden(char *p_str)
{
    RefRun l_refRun;

    if (!m_overrideList)
        return 0;

    m_overrideList->RunInitNxtRef(&l_refRun);
    SActionOverride *l_ref = (SActionOverride *)m_overrideList->RunNxtRefPtr(&l_refRun);

    if (!l_ref)
        return 0;

    while (_strcmpi(l_ref->actionName.m_pointer, p_str))
    {
        l_ref = (SActionOverride *)m_overrideList->RunNxtRefPtr(&l_refRun);

        if (!l_ref)
            return NULL;
    }

    return l_ref;
}

void ZInputActions::AddActionMap(ZActionMapDefinition *p_actionMapDef, char *p_str)
{
    ZActionMap *l_getMap = GetActionMap(p_actionMapDef);

    if (l_getMap)
    {
        ++l_getMap->m_unkInt1;
    }
    else
    {
        ZActionMap *l_actionMap = new ZActionMap(this, p_actionMapDef, p_str);

        m_actionMapDefinitions->Add((u32)l_actionMap);
    }
}

void ZInputActions::RemoveActionMap(ZActionMap *p_actionMap)
{
    if (p_actionMap->m_unkInt1-- == 1)
    {
        m_actionMapDefinitions->Remove((u32)p_actionMap);

        if (p_actionMap)
        {
            if (p_actionMap->m_mapName)
                delete p_actionMap->m_mapName;

            p_actionMap->RemoveActions();

            if (p_actionMap->m_refTab)
                p_actionMap->m_refTab->~RefTab();

            if (p_actionMap->m_fastLookup)
                p_actionMap->m_fastLookup->~CFastLookup2();

            ZInputActions *l_actions = p_actionMap->m_inputActions;

            if (l_actions->m_actionMap == p_actionMap)
                l_actions->m_actionMap = 0;

            delete p_actionMap;
        }
    }
}

void ZInputActions::RemoveActionMap(char *p_actionStr)
{
    ZActionMap *l_actionMap = GetActionMap(p_actionStr);

    if (l_actionMap)
        RemoveActionMap(l_actionMap);
}

void ZInputActions::RemoveActionMap(ZActionMapDefinition *p_actionMapDef)
{
    ZActionMap *l_actionMap = GetActionMap(p_actionMapDef);

    if (l_actionMap)
        RemoveActionMap(l_actionMap);
}

void ZInputActions::RemoveActionMaps()
{
    RefRun l_refRun;

    if (m_actionMapDefinitions)
    {
        m_actionMapDefinitions->RunInitNxtRef(&l_refRun);

        for (ZActionMap *l_item = (ZActionMap *)m_actionMapDefinitions->RunNxtRef(&l_refRun); l_refRun.prev; l_item = (ZActionMap *)m_actionMapDefinitions->RunNxtRef((RefRun *)&l_refRun.next))
        {
            m_actionMapDefinitions->RunDelRef(&l_refRun);

            if (l_item)
            {
                if (l_item->m_mapName)
                    delete l_item->m_mapName;

                l_item->RemoveActions();

                if (l_item->m_refTab)
                    l_item->m_refTab->~RefTab();

                if (l_item->m_fastLookup)
                    l_item->m_fastLookup->~CFastLookup2();

                ZInputActions *l_actions = l_item->m_inputActions;

                if (l_actions->m_actionMap == l_item)
                    l_actions->m_actionMap = 0;

                delete l_item;
            }
        }
    }
}

// Disable frame pointer omission
// These functions access the return address for some reason
// Therefore we need to ensure its a stack frame
#pragma optimize("y", off)

boolean ZInputActions::UnkFunc7(ZInputAction *p_action, i32 p_refNum)
{
    RefRun l_refRun;

    u32 l_refNum = p_refNum;

    if (!p_refNum)
    {
        l_refNum = GetReturnAddress();
    }

    if (!p_action)
        return 0;

    if (m_unkInt3)
        return 0;

    if (p_action->m_actionMap->IsActivated())
        return 0;

    if (p_action->m_unkByte3 && g_pSysInterface->m_enableDebugOptions == 0.0)
        return 0;

    if (p_action->m_lockCount > 0)
        return 0;

    if (!p_action->m_pressHandlers)
    {
        if (p_action->m_holdHandlers)
        {
            if (!p_action->m_unkByte1 && p_action->m_unkByte2)
            {
                p_action->m_holdHandlers->RunInitNxtRef(&l_refRun);
                u32 l_nxtHoldRef = p_action->m_holdHandlers->RunNxtRef(&l_refRun);

                if (l_refRun.prev)
                {
                    while (l_refNum != l_nxtHoldRef)
                    {
                        l_nxtHoldRef = p_action->m_holdHandlers->RunNxtRef(&l_refRun);

                        if (!l_refRun.prev)
                            goto ADD_HOLD_REF;
                    }

                    return 0;
                }

            ADD_HOLD_REF:
                p_action->m_holdHandlers->Add(l_refNum);

                return p_action->CheckDependencies();
            }
        }
        else
        {
            if (!p_action->m_releaseHandlers)
                return p_action->m_unkByte1 && p_action->CheckDependencies();

            if (p_action->m_unkByte1 && p_action->m_unkFloat2 - p_action->m_unkFloat1 < 0.25)
            {
                p_action->m_releaseHandlers->RunInitNxtRef(&l_refRun);
                u32 l_nxtReleaseRef = p_action->m_releaseHandlers->RunNxtRef(&l_refRun);

                if (l_refRun.prev)
                {
                    while (l_refNum != l_nxtReleaseRef)
                    {
                        l_nxtReleaseRef = p_action->m_releaseHandlers->RunNxtRef(&l_refRun);

                        if (!l_refRun.prev)
                            goto ADD_RELEASE_REF;
                    }
                    return 0;
                }

            ADD_RELEASE_REF:
                p_action->m_releaseHandlers->Add(l_refNum);

                return p_action->CheckDependencies();
            }
        }

        return 0;
    }

    if (!p_action->m_unkByte1)
        return 0;

    p_action->m_pressHandlers->RunInitNxtRef(&l_refRun);
    u32 l_nxtPressRef = p_action->m_pressHandlers->RunNxtRef(&l_refRun);

    if (l_refRun.prev)
    {
        while (l_refNum != l_nxtPressRef)
        {
            l_nxtPressRef = p_action->m_pressHandlers->RunNxtRef(&l_refRun);

            if (!l_refRun.prev)
                goto ADD_PRESS_REF;
        }

        return 0;
    }

ADD_PRESS_REF:
    p_action->m_pressHandlers->Add(l_refNum);

    return 1;
}

boolean ZInputActions::UnkFunc8(char *p_actionName, u32 p_refNum)
{
    if (!p_refNum)
        p_refNum = GetReturnAddress();

    ZInputAction *l_ref = GetInputAction(p_actionName);

    if (l_ref)
    {
        return !l_ref->m_actionMap->IsActivated() && UnkFunc7(l_ref, p_refNum);
    }
    else
    {
        if (p_actionName)
        {
            ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 517);
            l_sysCom->LogMessage("WARNING: Action %s does not exist", p_actionName);
        }
        else
        {
            ZSysCom *l_sysCom2 = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 519);
            l_sysCom2->LogMessage("WARNING: Action without name does not exist");
        }

        return FALSE;
    }
}

#pragma optimize("", on) // Enable frame pointer omission

// Function not working yet
void ZInputActions::UnkFunc9(RefTab *p_tab1, RefTab *p_tab2)
{
    printf("[HOOK]: ZInputActions::UnkFunc9 called\n");

    typedef void(__fastcall * UnkFunc9_t)(ZInputActions * _this, void *EDX, RefTab *p_tab1, RefTab *p_tab2);
    UnkFunc9_t UnkFunc9 = (UnkFunc9_t)0x0FFA1E80;
    UnkFunc9(this, 0, p_tab1, p_tab2);
    return;

    RefTab *l_refTab2;           // ecx
    ZInputAction *i;             // eax
    SInputEntry *list;           // eax
    int v7;                      // esi
    SInputEntry **l_list;        // ecx
    SInputList *v9;              // esi
    int totalCount;              // edx
    int v11;                     // ecx
    int v12;                     // esi
    int v13;                     // ebp
    SInputNode *l_inputNodeList; // edx
    u8 v15;                      // bl
    RefTab *l_refTab;            // ecx
    RefTab *l_refTab3;           // ecx
    ZInputActionBinding *v18;    // eax
    bool v19;                    // zf
    ZInputAction *inputAction;   // esi
    int l_activeNodeCount;       // eax
    int v22;                     // eax
    RefTab *l_refTab4;           // ecx
    RefTab *l_refTab2_1;         // ecx
    ZInputAction *j;             // esi
    u8 l_unkByte2;               // al
    RefTab *l_holdHandlers;      // ecx
    RefTab *l_pressHandlers;     // ecx
    RefTab *l_releaseHandlers;   // ecx
    int listSize;                // [esp+38h] [ebp-34h]
    int v31;                     // [esp+38h] [ebp-34h]
    RefRun v32;                  // [esp+3Ch] [ebp-30h] BYREF
    RefRun v33;                  // [esp+48h] [ebp-24h] BYREF
    RefRun v34;                  // [esp+54h] [ebp-18h] BYREF
    RefRun v35;                  // [esp+60h] [ebp-Ch] BYREF

    l_refTab2 = this->m_refTab2;
    if (l_refTab2)
    {
        l_refTab2->RunInitNxtRef(&v32);
        for (i = (ZInputAction *)this->m_refTab2->RunNxtRef(&v32);
             v32.prev;
             i = (ZInputAction *)this->m_refTab2->RunNxtRef(&v32))
        {
            i->m_unkByte2 = i->m_unkByte1;
            this->m_refTab2->RunDelRef(&v32);
        }
    }

    list = g_inputLists[0].list;
    v7 = 0;
    if (g_inputLists[0].list)
    {
        l_list = &g_inputLists[0].list;
        while (list != g_inputLists[0].list)
        {
            list = l_list[4];
            l_list += 4;
            ++v7;
            if (!list)
                goto LABEL_8;
        }
        v9 = &g_inputLists[v7];
    }
    else
    {
    LABEL_8:
        v9 = 0;
    }

    totalCount = v9->totalCount;

    if (v9->listSize)
    {
        v11 = 0;
        listSize = v9->listSize;
        do
        {
            m_curStates[totalCount++] = g_pSysInterface->m_unkPad23[15 * v9->list[v11++].index + 6];
            --listSize;
        } while (listSize);
    }

    UnkFunc20();

    v12 = 0;
    v31 = 0;
    if (m_inputNodeCount)
    {
        v13 = 0;
        do
        {
            l_inputNodeList = m_inputNodeList;
            v15 = m_curStates[v12];
            if (l_inputNodeList[v13].m_isToggleNode != v15)
            {
                l_inputNodeList[v13].m_isToggleNode = v15;
                l_refTab = m_inputNodeList[v13].m_refTab;
                if (l_refTab)
                {
                    l_refTab->RunInitNxtRef(&v35);
                    l_refTab3 = m_inputNodeList[v13].m_refTab;
                    v18 = (ZInputActionBinding *)l_refTab3->RunNxtRefPtr(&v35);
                    if (v18)
                    {
                        do
                        {
                            v19 = v18->flag == v15;
                            inputAction = v18->inputAction;
                            l_activeNodeCount = v18->inputAction->m_activeNodeCount;
                            if (v19)
                                v22 = l_activeNodeCount + 1;
                            else
                                v22 = l_activeNodeCount - 1;
                            inputAction->m_activeNodeCount = v22;
                            if (!m_refTab2->Exists((u32)inputAction))
                                m_refTab2->Add((UINT)inputAction);

                            l_refTab4 = m_inputNodeList[v13].m_refTab;
                            v18 = (ZInputActionBinding *)l_refTab4->RunNxtRefPtr(&v35);
                        } while (v18);
                        v12 = v31;
                    }
                }
            }
            ++v12;
            ++v13;
            v31 = v12;
        } while (v12 != m_inputNodeCount);
    }
    l_refTab2_1 = m_refTab2;
    if (l_refTab2_1)
    {
        l_refTab2_1->RunInitNxtRef(&v35);
        for (j = (ZInputAction *)m_refTab2->RunNxtRef(&v35);
             v35.prev;
             j = (ZInputAction *)m_refTab2->RunNxtRef(&v35))
        {
            l_unkByte2 = j->m_unkByte2;
            if (j->m_totalNodeCount == j->m_activeNodeCount)
            {
                if (!l_unkByte2 && !j->m_unkByte1)
                {
                    l_holdHandlers = j->m_holdHandlers;
                    if (l_holdHandlers)
                    {
                        l_holdHandlers->RunInitNxtRef(&v32);
                        j->m_holdHandlers->RunNxtRef(&v32);
                        while (v32.prev)
                        {
                            j->m_holdHandlers->RunDelRef(&v32);
                            j->m_holdHandlers->RunNxtRef(&v32);
                        }
                    }
                    j->m_unkByte1 = 1;
                    j->m_unkFloat2 = g_pSysInterface->m_unkInt80;
                    p_tab1->Add((UINT)j);
                }
            }
            else if (l_unkByte2 && j->m_unkByte1)
            {
                j->m_unkByte1 = 0;
                j->m_unkFloat1 = g_pSysInterface->m_unkInt80;
                p_tab1->Add((UINT)j);
                l_pressHandlers = j->m_pressHandlers;
                if (l_pressHandlers)
                {
                    l_pressHandlers->RunInitNxtRef(&v33);
                    j->m_pressHandlers->RunNxtRef(&v33);
                    while (v33.prev)
                    {
                        j->m_pressHandlers->RunDelRef(&v33);
                        j->m_pressHandlers->RunNxtRef(&v33);
                    }
                }
                l_releaseHandlers = j->m_releaseHandlers;
                if (l_releaseHandlers)
                {
                    l_releaseHandlers->RunInitNxtRef(&v34);
                    j->m_releaseHandlers->RunNxtRef(&v34);
                    while (v34.prev)
                    {
                        j->m_releaseHandlers->RunDelRef(&v34);
                        j->m_releaseHandlers->RunNxtRef(&v34);
                    }
                }
            }
        }
    }
}

ZInputAction *ZInputActions::GetInputAction(char *p_actionName)
{
    ZInputAction *l_ref;

    if (!m_actionMap || (l_ref = (ZInputAction *)m_actionMap->m_fastLookup->GetLowerCase(p_actionName)) == 0)
    {
        l_ref = (ZInputAction *)m_fastLookup->GetLowerCase(p_actionName);

        if (l_ref)
            m_actionMap = l_ref->m_actionMap;
    }

    return l_ref;
}

ZActionMap *ZInputActions::GetActionMap(ZActionMapDefinition *p_actionMapDef)
{
    RefRun l_refRun;

    if (!m_actionMapDefinitions)
        return 0;

    m_actionMapDefinitions->RunInitNxtRef(&l_refRun);
    ZActionMap *l_result = (ZActionMap *)m_actionMapDefinitions->RunNxtRef(&l_refRun);

    if (!l_refRun.prev)
        return 0;

    while (l_result->m_actionMapDef != p_actionMapDef)
    {
        l_result = (ZActionMap *)m_actionMapDefinitions->RunNxtRef(&l_refRun);

        if (!l_refRun.prev)
            return NULL;
    }

    return l_result;
}

ZActionMap *ZInputActions::GetActionMap(char *p_str)
{
    RefRun l_refRun;

    if (!p_str)
        return 0;

    if (!m_actionMapDefinitions)
        return 0;

    m_actionMapDefinitions->RunInitNxtRef(&l_refRun);
    ZActionMap *l_nextRef = (ZActionMap *)m_actionMapDefinitions->RunNxtRef(&l_refRun);

    if (!l_refRun.prev)
        return 0;

    while (!l_nextRef->m_mapName || _strcmpi(p_str, l_nextRef->m_mapName))
    {
        l_nextRef = (ZActionMap *)m_actionMapDefinitions->RunNxtRef(&l_refRun);

        if (!l_refRun.prev)
            return NULL;
    }

    return l_nextRef;
}

i32 ZInputActions::GetInputNode(char *p_vkName)
{
    i32 l_listIndex = 0;

    while (g_inputLists[l_listIndex].list != NULL)
    {
        SInputEntry *l_currentList = g_inputLists[l_listIndex].list;
        i32 l_entryIndex = 0;

        while (l_currentList[l_entryIndex].vkName != NULL)
        {
            if (_stricmp(l_currentList[l_entryIndex].vkName, p_vkName) == 0)
            {
                i32 l_totalOffset = 0;
                for (i32 i = 0; i < l_listIndex; ++i)
                {
                    l_totalOffset += g_inputLists[i].listSize;
                }

                return (l_totalOffset + l_entryIndex);
            }

            ++l_entryIndex;
        }

        ++l_listIndex;
    }

    return -1;
}

void ZInputActions::EnableActions()
{
    --m_unkInt3;
}

void ZInputActions::DisableActions()
{
    ++m_unkInt3;
}

void ZInputActions::ActivateMap(char *p_str)
{
    ZActionMap *l_actionMap = GetActionMap(p_str);

    if (l_actionMap)
        l_actionMap->ActivateMap();
}

void ZInputActions::DeactivateMap(char *p_str)
{
    ZActionMap *l_actionMap = GetActionMap(p_str);

    if (l_actionMap)
        l_actionMap->DeactivateMap();
}

void ZInputActions::MakeActionExclusiveOwner(char *p_vkName, bool p_flag)
{
    ZInputAction *l_inputAction = GetInputAction(p_vkName);

    if (l_inputAction)
        l_inputAction->MakeActionExclusiveOwnerOfNodes(p_flag);
}

SInputEntry *ZInputActions::GetListByIndex(i32 p_index)
{
    i32 l_curIndex = 0;

    while (g_inputLists[l_curIndex].list)
    {
        if (l_curIndex == p_index)
            return g_inputLists[l_curIndex].list;

        ++l_curIndex;

        if (!g_inputLists[l_curIndex].listSize)
            break;
    }

    return NULL;
}

void ZInputActions::RemoveOverrideList()
{
    RefRun l_refRun;
    MyStr l_myStr;
    SActionOverride *l_curOverride;

    if (m_overrideList)
    {
        m_overrideList->RunInitNxtRef(&l_refRun);

        for (l_curOverride = (SActionOverride *)m_overrideList->RunNxtRefPtr(&l_refRun); l_curOverride; l_curOverride = (SActionOverride *)m_overrideList->RunNxtRefPtr(&l_refRun))
        {
            l_myStr = MyStr(g_emptyArray);
            l_curOverride->actionName = l_myStr;
        }

        if (m_overrideList)
            m_overrideList->~RefTab();

        m_overrideList = 0;
    }
}

void ZInputActions::UnkFunc20()
{
    i32 l_totalCount = g_inputLists[1].totalCount;

    m_curStates[l_totalCount] = 0;
    m_curStates[l_totalCount + 1] = 0;

    m_unkFloat3 = g_pSysInterface->m_unkInt70 - m_unkFloat1 + m_unkFloat3;
    m_unkFloat1 = g_pSysInterface->m_unkInt70;

    f64 l_num1;

    if (g_pSysInterface->m_unkInt67)
        l_num1 = (m_unkFloat3 / g_pSysInterface->m_unkInt67);

    f32 num2 = m_unkFloat2;
    m_unkFloat2 = l_num1;

    if (l_num1 - num2 <= 0)
    {
        if (l_num1 - num2 < 0)
            m_curStates[l_totalCount + 1] = 1;
    }
    else
    {
        m_curStates[l_totalCount] = 1;
    }
}
