#include "zinputactions.h"
#include "zinputaction.h"

ZInputActions::ZInputActions()
{
    i32 l_totalCount = 0;
    if (g_inputLists[0].list)
    {
        SInputList *l_currentList = &g_inputLists[0];
        while (l_currentList->list)
        {
            l_currentList->totalCount = l_totalCount;
            l_totalCount += l_currentList->listSize;

            if (!l_currentList[1].listSize)
                break;

            ++l_currentList;
        }
    }

    i32 l_mainNodeCount = 0;
    if (g_inputLists[0].list)
    {
        SInputList *l_currentList = &g_inputLists[0];
        while (l_currentList->list)
        {
            l_mainNodeCount += l_currentList->listSize;

            if (!l_currentList[1].listSize)
                break;

            ++l_currentList;
        }
    }

    m_inputNodeCount = l_mainNodeCount;

    m_unkStr = new char[l_mainNodeCount];
    memset(m_unkStr, 0, m_inputNodeCount);
    i32 l_nodeCount = m_inputNodeCount;

    SInputNodeList *l_listAlloc = new SInputNodeList;
    l_listAlloc->count = l_nodeCount;
    l_listAlloc->nodes = new SInputNode[l_nodeCount];

    i32 l_count = 0;
    i32 l_nodePtr = 0;
    m_inputNodeList = l_listAlloc->nodes;

    if (m_inputNodeCount)
    {
        l_nodePtr = 0;
        do
        {
            m_inputNodeList[l_nodePtr].m_parent = this;

            ++l_count;
            ++l_nodePtr;
        } while (l_count != m_inputNodeCount);
    }

    m_actionMapDefinitions = new RefTab(4, 0);
    m_unkInt2 = 0;

    m_refTab2 = new RefTab(4, 0);
    m_unkFloat1 = 0;
    m_unkFloat2 = g_pSysInterface->m_unkInt70;
    m_unkInt7 = 0;

    m_fastLookup = new CFastLookup2(32);
    m_unkInt3 = 0;
    m_actionMap = 0;
    m_overrideList = 0;
}

ZInputActions::~ZInputActions()
{
    printf("[DESTRUCTOR HOOK]: ZInputActions called\n");
}

void ZInputActions::OverrideActions(RefTab *l_actionsList)
{
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

    typedef void(__fastcall * OverrideAction_t)(ZInputActions * _this, void *EDX, SActionOverride *p_actionOverride);
    OverrideAction_t OverrideAction = (OverrideAction_t)0x0FFA14A0;

    OverrideAction(this, 0, p_actionOverride);

    return;

    RefTab *l_overrideList;               // ecx
    RefTab *l_tabMalloc;                  // eax
    SActionOverride *l_actionOverride;    // edi MAPDST
    SActionOverride *l_actionOverridePtr; // eax
    SInputEntry *l_inputListPtr;          // eax
    int l_someCount;                      // edx
    int l_list;                           // ecx
    SInputList *l_keysInputList;          // eax
    int *l_arrayPtr;                      // edi
    int listSize;                         // edx
    int l_loopCount;                      // ecx
    int *l_keysIndex;                     // eax
    int l_finalIndex;                     // eax
    ZSysCom *l_sysCom;                    // eax
    char *m_pointer;                      // esi
    UINT v19;                             // kr08_4
    char *m_buffer;                       // eax
    ZInputAction *l_inputAction;          // eax
    ZActionMap *l_actionMap;              // esi
    int l_lockCount;                      // edi
    RefRun l_refRun;                      // [esp+Ch] [ebp-28h] BYREF
    int l_offset;                         // [esp+18h] [ebp-1Ch]
    ZInputActions *l_this;                // [esp+1Ch] [ebp-18h]
    int l_index;                          // [esp+20h] [ebp-14h]
    SInputList *l_keysInputListPtr;       // [esp+24h] [ebp-10h]
    int l_unused;                         // [esp+30h] [ebp-4h]

    l_overrideList = m_overrideList;
    l_this = this;

    if (!l_overrideList)
    {

        l_offset = (int)l_tabMalloc;
        l_unused = -1;

        m_overrideList = new RefTab(32, 39);

        goto LABEL_13;
    }

    l_overrideList->RunInitNxtRef(&l_refRun);
    l_actionOverride = (SActionOverride *)m_overrideList->RunNxtRefPtr(&l_refRun);

    if (!l_actionOverride)
    {
    LABEL_13:
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
        goto LABEL_17;
    }

    while (strcmp(l_actionOverride->actionName.m_pointer, p_actionOverride->actionName.m_pointer))
    {
        l_actionOverride = (SActionOverride *)m_overrideList->RunNxtRefPtr(&l_refRun);
        if (!l_actionOverride)
            goto LABEL_13;
    }
LABEL_17:
    l_actionOverride->unkByte1 = p_actionOverride->unkByte1;
    l_actionOverride->unkInt7 = p_actionOverride->unkInt7;
    l_actionOverride->unkInt8 = p_actionOverride->unkInt8;

    l_inputListPtr = g_inputLists[0].list;
    l_someCount = 0;

    if (g_inputLists[0].list)
    {
        l_list = (int)&g_inputLists[0].list;

        while (l_inputListPtr != (SInputEntry *)&g_keysInputList)
        {
            l_inputListPtr = *(SInputEntry **)(l_list + 16);
            l_list += 16;
            ++l_someCount;

            if (!l_inputListPtr)
                goto LABEL_21;
        }

        l_keysInputList = &g_inputLists[l_someCount];
        l_keysInputListPtr = l_keysInputList;
    }
    else
    {
    LABEL_21:
        l_keysInputListPtr = 0;
        l_keysInputList = 0;
    }

    l_index = 0;
    l_arrayPtr = p_actionOverride->unkArray;
    l_offset = (char *)l_actionOverride - (char *)p_actionOverride;

    while (*l_arrayPtr)
    {
        listSize = l_keysInputList->listSize;
        l_loopCount = 0;
        if (listSize)
        {
            l_keysIndex = &l_keysInputList->list->index;

            while (*l_keysIndex != *l_arrayPtr)
            {
                ++l_loopCount;

                l_keysIndex += 3;

                if (l_loopCount == listSize)
                    goto JUMP_LOOP;
            }
            l_finalIndex = l_loopCount + l_keysInputListPtr->totalCount;
        }
        else
        {
        JUMP_LOOP:
            l_finalIndex = -1;
        }

        *(int *)((char *)l_arrayPtr + l_offset) = l_finalIndex;

        if (l_finalIndex == -1)
        {

            l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 173);
            l_sysCom->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\source\\Actions.cpp", 173);
            DebugBreak();
        }

        ++l_arrayPtr;

        if (++l_index == 4)
            goto CLEANUP_LBL;

        l_keysInputList = l_keysInputListPtr;
    }
    l_actionOverride->unkArray[l_index] = -1;

CLEANUP_LBL:
    l_actionOverride->actionName = p_actionOverride->actionName;

    l_inputAction = GetInputAction(l_actionOverride->actionName.m_pointer);
    l_actionOverride->handlerType = 0;

    if (l_inputAction)
    {
        if (l_inputAction->m_pressHandlers)
            l_actionOverride->handlerType = 1;

        if (l_inputAction->m_holdHandlers)
            l_actionOverride->handlerType = 2;

        if (l_inputAction->m_releaseHandlers)
            l_actionOverride->handlerType = 3;

        l_actionMap = l_inputAction->m_actionMap;
        l_lockCount = l_inputAction->m_lockCount;

        l_actionMap->RemoveAction(l_actionOverride->actionName.m_pointer);
        l_actionMap->AddAction(l_actionOverride)->m_lockCount = l_lockCount;
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
            return 0;
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
            p_actionMap->~ZActionMap();
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
                l_item->~ZActionMap();
            }
        }
    }
}

boolean ZInputActions::UnkFunc7(i32 p_unkInt, i32 p_unkInt2)
{
    printf("[HOOK]: ZInputActions::UnkFunc7 called\n");
    return FALSE;
}

boolean ZInputActions::UnkFunc8(char *p_actionName, u32 p_refNum)
{
    if (!p_refNum)
        p_refNum = GetReturnAddress();

    ZInputAction *l_ref = GetInputAction(p_actionName);

    if (l_ref)
    {
        return !l_ref->m_actionMap->IsActivated() && UnkFunc7((i32)l_ref, p_refNum);
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

        return 0;
    }
}

void ZInputActions::UnkFunc9(i32 p_unkInt)
{
    printf("[HOOK]: ZInputActions::UnkFunc9 called\n");
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
            return 0;
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
            return 0;
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

                return l_totalOffset + l_entryIndex;
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
    printf("[HOOK]: ZInputActions::RemoveOverrideList called\n");
}
