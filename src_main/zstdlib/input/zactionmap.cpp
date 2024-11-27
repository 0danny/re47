#include "zactionmap.h"
#include "zinputaction.h"

ZActionMap::ZActionMap(ZInputActions *p_inputActions, ZActionMapDefinition *p_actionMapDef, char *p_mapName)
{
    m_fastLookup = new CFastLookup2(32);
    m_unkByte = p_actionMapDef->UnkFunc0();
    m_inputActions = p_inputActions;
    m_unkInt1 = 1;
    m_actionMapDef = p_actionMapDef;
    m_mapName = 0;
    m_unkInt3 = 0;

    i32 l_bindingCount = 0;

    if (p_mapName)
    {
        m_mapName = new char[(strlen(p_mapName) + 1)];

        strcpy(m_mapName, p_mapName);
        l_bindingCount = 0;
    }

    SInputActionDefinition *l_mapLoop = p_actionMapDef->GetMap();
    SInputActionDefinition *l_mapPtr = l_mapLoop;

    SInputActionDefinition *l_nextMap;

    if (l_mapLoop->commandName)
    {
        do
        {
            l_nextMap = (SInputActionDefinition *)l_mapLoop[1].commandName;

            ++l_mapLoop;
            ++l_bindingCount;
        } while (l_nextMap);
    }

    m_refTab = new RefTab(8, 0);

    if (l_bindingCount)
    {
        p_inputActions->m_unkInt2 += l_bindingCount;

        do
        {
            SActionOverride *l_overriden = m_inputActions->FindOverriden(l_mapPtr->commandName);

            if (l_overriden)
            {
                l_overriden->handlerType = l_mapPtr->handlerType;
                AddAction(l_overriden);
            }
            else
            {
                AddAction(l_mapPtr);
            }

            ++l_mapPtr;
            --l_bindingCount;

        } while (l_bindingCount);
    }
}

ZInputAction *ZActionMap::AddAction(SActionOverride *p_actionOverride)
{
    ZInputAction *l_action = m_inputActions->GetInputAction(p_actionOverride->actionName.m_pointer);

    if (l_action)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 741);
        l_sysCom->LogMessage("WARNING: Action-name %s has already been used", p_actionOverride->actionName.m_pointer);
    }
    else
    {
        l_action = new ZInputAction(this, p_actionOverride);

        m_refTab->Add((u32)l_action);

        m_fastLookup->SetLowerCase(p_actionOverride->actionName.m_pointer, (u32)l_action);
        m_inputActions->m_fastLookup->SetLowerCase(p_actionOverride->actionName.m_pointer, (u32)l_action);
    }

    return l_action;
}

ZInputAction *ZActionMap::AddAction(SInputActionDefinition *p_actionDefinition)
{
    ZInputAction *l_action = m_inputActions->GetInputAction(p_actionDefinition->commandName);

    if (l_action)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 726);
        l_sysCom->LogMessage("WARNING: Action-name %s has already been used", p_actionDefinition->commandName);
    }
    else
    {
        l_action = new ZInputAction(this, p_actionDefinition);

        m_refTab->Add((u32)l_action);

        m_fastLookup->SetLowerCase(p_actionDefinition->commandName, (u32)l_action);
        m_inputActions->m_fastLookup->SetLowerCase(p_actionDefinition->commandName, (u32)l_action);
    }

    return l_action;
}

void ZActionMap::RemoveAction(char *p_actionName)
{
    RefRun l_refRun;

    if (m_refTab)
    {
        m_refTab->RunInitNxtRef(&l_refRun);
        ZInputAction *l_nextRef = (ZInputAction *)m_refTab->RunNxtRef(&l_refRun);

        if (l_refRun.prev)
        {
            while (1)
            {
                i32 l_cmpRes = _strcmpi(l_nextRef->m_actionName, p_actionName);

                if (!l_cmpRes)
                    break;

                l_nextRef = (ZInputAction *)m_refTab->RunNxtRef(&l_refRun);

                if (!l_refRun.prev)
                    return;
            }

            m_refTab->RunDelRef(&l_refRun);

            m_inputActions->m_fastLookup->RemoveLowerCase(p_actionName, 0);
            m_fastLookup->RemoveLowerCase(p_actionName, 0);

            if (l_nextRef)
            {
                l_nextRef->~ZInputAction();
                delete l_nextRef;
            }
        }
    }
}

void ZActionMap::RemoveActions()
{
    RefRun l_refRun;

    if (m_refTab)
    {
        m_refTab->RunInitNxtRef(&l_refRun);

        for (ZInputAction *l_item = (ZInputAction *)m_refTab->RunNxtRef(&l_refRun); l_refRun.prev; l_item = (ZInputAction *)m_refTab->RunNxtRef(&l_refRun))
        {
            m_refTab->RunDelRef(&l_refRun);

            m_inputActions->m_fastLookup->RemoveLowerCase(l_item->m_actionName, 0);
            m_fastLookup->RemoveLowerCase(l_item->m_actionName, 0);

            if (l_item)
            {
                l_item->~ZInputAction();
                delete l_item;
            }
        }
    }
}

ZInputAction *ZActionMap::GetActionBase(char *p_actionName)
{
    RefRun l_refRun;

    if (!m_refTab)
        return 0;

    m_refTab->RunInitNxtRef(&l_refRun);
    ZInputAction *l_ref = (ZInputAction *)m_refTab->RunNxtRef(&l_refRun);

    if (!l_refRun.prev)
        return 0;

    while (_strcmpi(l_ref->m_actionName, p_actionName))
    {
        l_ref = (ZInputAction *)m_refTab->RunNxtRef(&l_refRun);

        if (!l_refRun.prev)
            return 0;
    }

    return l_ref;
}

void ZActionMap::LockActions(char *p_str, boolean p_flag)
{
    RefTab *l_nxtRef;
    RefRun l_refRun;

    i32 l_index = m_inputActions->GetInputNode(p_str);
    RefTab *l_refTab = m_inputActions->m_inputNodeList[l_index].m_refTab;

    if (l_refTab)
    {
        l_refTab->RunInitNxtRef(&l_refRun);
        l_refTab = m_inputActions->m_inputNodeList[l_index].m_refTab;

        for (ZInputActionBinding *l_item = (ZInputActionBinding *)l_refTab->RunNxtRefPtr(&l_refRun); l_item; l_item = (ZInputActionBinding *)l_nxtRef->RunNxtRefPtr(&l_refRun))
        {
            if (l_item->flag == p_flag)
            {
                ZInputAction *l_binding = l_item->inputAction;

                if (l_binding->m_actionMap != this)
                    ++l_binding->m_lockCount;
            }

            l_nxtRef = m_inputActions->m_inputNodeList[l_index].m_refTab;
        }
    }
}

void ZActionMap::UnlockActions(char *p_str, boolean p_flag)
{
    RefTab *l_nxtRef;
    RefRun l_refRun;

    i32 l_index = m_inputActions->GetInputNode(p_str);
    RefTab *l_refTab = m_inputActions->m_inputNodeList[l_index].m_refTab;

    if (l_refTab)
    {
        l_refTab->RunInitNxtRef(&l_refRun);
        l_refTab = m_inputActions->m_inputNodeList[l_index].m_refTab;

        for (ZInputActionBinding *l_item = (ZInputActionBinding *)l_refTab->RunNxtRefPtr(&l_refRun); l_item; l_item = (ZInputActionBinding *)l_nxtRef->RunNxtRefPtr(&l_refRun))
        {
            if (l_item->flag == p_flag)
            {
                ZInputAction *l_binding = l_item->inputAction;

                if (l_binding->m_actionMap != this)
                    --l_binding->m_lockCount;
            }

            l_nxtRef = m_inputActions->m_inputNodeList[l_index].m_refTab;
        }
    }
}

void ZActionMap::ActivateMap()
{
    --m_unkInt3;
}

void ZActionMap::DeactivateMap()
{
    ++m_unkInt3;
}

boolean ZActionMap::IsActivated()
{
    return m_unkInt3 > 0;
}
