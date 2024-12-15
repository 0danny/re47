#include "zinputaction.h"

ZInputAction::ZInputAction(ZActionMap *p_actionMap, SActionOverride *p_actionOverride)
{
    m_actionMap = p_actionMap;
    m_actionName = 0;
    m_totalNodeCount = 0;
    m_activeNodeCount = 0;
    m_unkByte1 = 1;
    m_unkByte2 = 0;

    m_nodeReferences = new RefTab(2, 0);
    m_pressHandlers = 0;
    m_holdHandlers = 0;
    m_releaseHandlers = 0;
    m_unkFloat1 = 0;
    m_unkFloat2 = 0;
    m_lockCount = 0;
    m_unkByte3 = 0;

    char *l_actionNamePtr = p_actionOverride->actionName.m_pointer;

    i32 l_count = -1;
    bool l_flag;

    if (p_actionOverride->actionName.m_pointer)
    {
        do
        {
            if (!l_count)
                break;

            l_flag = *l_actionNamePtr++ == 0;
            --l_count;

        } while (!l_flag);

        u32 l_modCount = -l_count - 2 + 1;

        m_actionName = new char[l_modCount];

        strcpy(m_actionName, p_actionOverride->actionName.m_pointer);
    }
    else if (p_actionMap->m_mapName)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 997);
        l_sysCom->LogMessage("WARNING: Defining action without a name for actionmap %s", p_actionMap->m_mapName);
    }
    else
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 999);
        l_sysCom->LogMessage("WARNING: Defining action without a name for actionmap without a name");
    }

    switch (p_actionOverride->handlerType)
    {
    case 1:
        m_pressHandlers = new RefTab(2, 0);
        break;
    case 2:
        m_holdHandlers = new RefTab(2, 0);
        break;
    case 3:
        m_releaseHandlers = new RefTab(2, 0);
        break;
    }

    m_dependentNodes = 0;

    if (p_actionOverride->unkInt7)
        DependOnActionsNodes(p_actionOverride->unkInt7, 1);

    if (p_actionOverride->unkInt8)
        DependOnActionsNodes(p_actionOverride->unkInt8, 0);

    i32 *l_unkArray = p_actionOverride->unkArray;

    for (i32 l_item = 0; l_item != 4; ++l_item)
    {
        if (*l_unkArray == -1)
            break;

        AddNode(*l_unkArray++, 1);
    }
}

ZInputAction::ZInputAction(ZActionMap *p_actionMap, SInputActionDefinition *p_actionDefinition)
{
    m_actionMap = p_actionMap;
    m_actionName = 0;
    m_totalNodeCount = 0;
    m_activeNodeCount = 0;
    m_unkByte1 = 1;
    m_unkByte2 = 0;

    m_nodeReferences = new RefTab(2, 0);
    m_pressHandlers = 0;
    m_holdHandlers = 0;
    m_releaseHandlers = 0;
    m_unkFloat1 = 0;
    m_unkFloat2 = 0;
    m_lockCount = 0;
    m_unkByte3 = p_actionMap->m_unkByte;

    if (p_actionDefinition->unkByte1)
        m_unkByte3 = p_actionDefinition->unkByte1;

    if (p_actionDefinition->commandName)
    {
        m_actionName = new char[(strlen(p_actionDefinition->commandName) + 1)];

        strcpy(m_actionName, p_actionDefinition->commandName);
    }
    else if (p_actionMap->m_mapName)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 891);
        l_sysCom->LogMessage("WARNING: Defining action without a name for actionmap %s", p_actionMap->m_mapName);
    }
    else
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 893);
        l_sysCom->LogMessage("WARNING: Defining action without a name for actionmap without a name");
    }

    if (p_actionDefinition->handlerType == 1)
    {
        m_pressHandlers = new RefTab(2, 0);
    }

    if (p_actionDefinition->handlerType == 2)
    {
        m_holdHandlers = new RefTab(2, 0);
    }

    if (p_actionDefinition->handlerType == 3)
    {
        m_releaseHandlers = new RefTab(2, 0);
    }

    m_dependentNodes = 0;

    if (p_actionDefinition->unkInt2)
        DependOnActionsNodes(p_actionDefinition->unkInt2, 1);

    if (p_actionDefinition->unkInt3)
        DependOnActionsNodes(p_actionDefinition->unkInt3, 0);

    if (p_actionDefinition->keyBinding)
        AddNodes(p_actionDefinition->keyBinding, 1);

    if (p_actionDefinition->modifierKey)
        AddNodes(p_actionDefinition->modifierKey, 0);
}

ZInputAction::~ZInputAction()
{
    RefRun l_refRun2;
    RefRun l_refRun;

    if (m_dependentNodes)
    {
        m_dependentNodes->RunInitNxtRef(&l_refRun);
        for (ZActionNode *l_i = (ZActionNode *)m_dependentNodes->RunNxtRefPtr(&l_refRun); l_i; l_i = (ZActionNode *)m_dependentNodes->RunNxtRefPtr(&l_refRun))
        {
            char *l_nodeName = l_i->nodeName;

            if (l_nodeName)
            {
                delete l_nodeName;
            }
        }

        if (m_dependentNodes)
            m_dependentNodes->~RefTab();

        m_dependentNodes = 0;
    }

    if (m_pressHandlers)
        m_pressHandlers->~RefTab();

    if (m_holdHandlers)
        m_holdHandlers->~RefTab();

    if (m_releaseHandlers)
        m_releaseHandlers->~RefTab();

    if (m_actionName)
        delete m_actionName;

    if (m_nodeReferences)
    {
        m_nodeReferences->RunInitNxtRef(&l_refRun2);

        for (u32 l_j = m_nodeReferences->RunNxtRef(&l_refRun2); l_refRun2.prev; l_j = m_nodeReferences->RunNxtRef(&l_refRun2))
        {
            RefTab *l_nodeListTab = m_actionMap->m_inputActions->m_inputNodeList[l_j].m_refTab;

            if (l_nodeListTab)
            {
                l_nodeListTab->RunInitNxtRef(&l_refRun);

                for (ZInputActionBinding *l_inputNode = (ZInputActionBinding *)l_nodeListTab->RunNxtRefPtr(&l_refRun); l_inputNode; l_inputNode = (ZInputActionBinding *)l_nodeListTab->RunNxtRefPtr(&l_refRun))
                {
                    if (l_inputNode->inputAction == this)
                        l_nodeListTab->RunDelRef(&l_refRun);
                }
            }
        }
    }

    if (m_nodeReferences)
        m_nodeReferences->~RefTab();
}

void ZInputAction::AddNode(char *p_vkKey, bool p_flag)
{
    i32 l_nodeIndex = m_actionMap->m_inputActions->GetInputNode(p_vkKey);

    if (l_nodeIndex == -1)
    {

        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 1098);
        l_sysCom->LogMessage("ERROR: Input type %s used by actionmap %s does not exist", p_vkKey, m_actionMap->m_mapName);

        ZSysCom *l_sysCom2 = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 1099);
        l_sysCom2->LogMessage("INT3 in %s at line %d", "Z:\\Engine\\System\\source\\Actions.cpp", 1099);

        DebugBreak();
    }
    else
    {
        AddNode(l_nodeIndex, p_flag);
    }
}

void ZInputAction::AddNode(i32 p_refNum, bool p_flag)
{
    ZInputActions *l_inputActions = m_actionMap->m_inputActions;

    if (l_inputActions->m_inputNodeCount > p_refNum)
    {
        RefTab *l_refTab = l_inputActions->m_inputNodeList[p_refNum].m_refTab;

        ZInputActionBinding *l_binding = (ZInputActionBinding *)(l_refTab->Add(0) - 1);

        l_binding->inputAction = this;
        l_binding->state = p_flag;

        if (p_flag == m_actionMap->m_inputActions->m_inputNodeList[p_refNum].m_nodeState)
            ++m_activeNodeCount;

        m_nodeReferences->Add(p_refNum);

        m_totalNodeCount++;

        if (m_activeNodeCount != m_totalNodeCount)
            m_unkByte1 = 0;
    }
}

void ZInputAction::AddNodes(i32 *p_ref, bool p_flag)
{
    for (i32 l_item = 0; l_item != 4; ++l_item)
    {
        if (*p_ref == -1)
            break;

        AddNode(*p_ref++, p_flag);
    }
}

void ZInputAction::AddNodes(const char *p_str, bool p_flag)
{
    char *l_strBuffer;
    char l_currChar;
    char l_savedChar;
    char l_spaceChar;

    if (p_str)
    {
        u32 l_strLength = strlen(p_str) + 1;

        l_strBuffer = new char[l_strLength];

        char *l_strCopy = l_strBuffer;

        strcpy(l_strBuffer, p_str);
        i32 l_currPosition = 0;

        if (*l_strBuffer == ' ')
        {
            do
                l_currChar = *(l_strCopy + ++l_currPosition);
            while (l_currChar == ' ');
        }

        char l_nextChar = *(l_strCopy + l_currPosition);
        char *l_tokenStart = l_strCopy + l_currPosition;

        if (l_nextChar != '0' && l_nextChar)
        {
            do
            {
                do
                {
                    if (l_nextChar == '+')
                        break;

                    l_nextChar = *(l_strCopy + ++l_currPosition);
                } while (l_nextChar);

                l_savedChar = *(l_strCopy + l_currPosition);
                *(l_strCopy + l_currPosition) = 0;

                AddNode(l_tokenStart, p_flag);

                *(l_strCopy + l_currPosition) = l_savedChar;

                if (l_savedChar == '+')
                    ++l_currPosition;

                if (*(l_strCopy + l_currPosition) == ' ')
                {
                    do
                        l_spaceChar = *(l_strCopy + ++l_currPosition);
                    while (l_spaceChar == ' ');
                }

                l_nextChar = *(l_strCopy + l_currPosition);
                l_tokenStart = l_strCopy + l_currPosition;
            } while (l_nextChar);
        }

        delete l_strCopy;
    }
}

void ZInputAction::LockInputNode(char *p_vkKey, bool p_flag, bool p_localMapOnly)
{
    RefTab *l_nodeListTab;
    RefRun l_refRun;

    i32 l_inputNode = m_actionMap->m_inputActions->GetInputNode(p_vkKey);

    if (m_nodeReferences->Exists(l_inputNode))
    {
        i32 l_inputNodeIndex = l_inputNode;
        RefTab *l_refTab = m_actionMap->m_inputActions->m_inputNodeList[l_inputNodeIndex].m_refTab;

        if (l_refTab)
        {
            l_refTab->RunInitNxtRef(&l_refRun);
            RefTab *l_inputNodeRefTab = m_actionMap->m_inputActions->m_inputNodeList[l_inputNodeIndex].m_refTab;

            for (ZInputActionBinding *l_item = (ZInputActionBinding *)l_inputNodeRefTab->RunNxtRefPtr(&l_refRun); l_item; l_item = (ZInputActionBinding *)l_nodeListTab->RunNxtRefPtr(&l_refRun))
            {
                if (l_item->state == p_flag)
                {
                    if ((!p_localMapOnly || l_item->inputAction->m_actionMap == m_actionMap) && l_item->inputAction != this)
                        ++l_item->inputAction->m_lockCount;
                }

                l_nodeListTab = m_actionMap->m_inputActions->m_inputNodeList[l_inputNodeIndex].m_refTab;
            }
        }
    }
    else
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 1142);
        l_sysCom->LogMessage("WARNING: Trying to lock node to action that does not contaion that node itself");
    }
}

void ZInputAction::UnlockInputNode(char *p_vkKey, bool p_flag, bool p_localMapOnly)
{
    RefTab *l_nodeListTab;
    RefRun l_refRun;

    i32 l_inputNode = m_actionMap->m_inputActions->GetInputNode(p_vkKey);

    if (m_nodeReferences->Exists(l_inputNode))
    {
        i32 l_inputNodeIndex = l_inputNode;
        RefTab *l_refTab = m_actionMap->m_inputActions->m_inputNodeList[l_inputNodeIndex].m_refTab;

        if (l_refTab)
        {
            l_refTab->RunInitNxtRef(&l_refRun);
            RefTab *l_inputNodeRefTab = m_actionMap->m_inputActions->m_inputNodeList[l_inputNodeIndex].m_refTab;

            for (ZInputActionBinding *l_item = (ZInputActionBinding *)l_inputNodeRefTab->RunNxtRefPtr(&l_refRun); l_item; l_item = (ZInputActionBinding *)l_nodeListTab->RunNxtRefPtr(&l_refRun))
            {
                if (l_item->state == p_flag)
                {
                    if ((!p_localMapOnly || l_item->inputAction->m_actionMap == m_actionMap) && l_item->inputAction != this)
                        --l_item->inputAction->m_lockCount;
                }

                l_nodeListTab = m_actionMap->m_inputActions->m_inputNodeList[l_inputNodeIndex].m_refTab;
            }
        }
    }
    else
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\System\\source\\Actions.cpp", 1142);
        l_sysCom->LogMessage("WARNING: Trying to lock node to action that does not contaion that node itself");
    }
}

void ZInputAction::MakeActionExclusiveOwnerOfNodes(bool p_lock)
{
    RefRun l_refRun;
    RefRun l_refRun2;

    if (m_nodeReferences)
    {
        m_nodeReferences->RunInitNxtRef(&l_refRun);

        for (u32 l_index = m_nodeReferences->RunNxtRef(&l_refRun); l_refRun.prev; l_index = m_nodeReferences->RunNxtRef(&l_refRun))
        {
            RefTab *l_inputNodeTab = m_actionMap->m_inputActions->m_inputNodeList[l_index].m_refTab;

            if (l_inputNodeTab)
            {
                l_inputNodeTab->RunInitNxtRef(&l_refRun2);

                for (ZInputActionBinding *l_item = (ZInputActionBinding *)l_inputNodeTab->RunNxtRefPtr(&l_refRun2); l_item; l_item = (ZInputActionBinding *)l_inputNodeTab->RunNxtRefPtr(&l_refRun2))
                {
                    if (l_item->inputAction != this)
                    {
                        i32 l_lockCount;

                        if (p_lock)
                            l_lockCount = l_item->inputAction->m_lockCount + 1;
                        else
                            l_lockCount = l_item->inputAction->m_lockCount - 1;

                        l_item->inputAction->m_lockCount = l_lockCount;
                    }
                }
            }
        }
    }
}

void ZInputAction::DependOnActionsNodes(const char *p_str, bool p_flag)
{
    char *l_strBuffer;
    char l_currChar;
    char l_savedChar;
    char l_spaceChar;

    if (p_str)
    {
        u32 l_strLength = strlen(p_str) + 1;

        l_strBuffer = new char[l_strLength];

        char *l_strCopy = l_strBuffer;

        strcpy(l_strBuffer, p_str);
        i32 l_currPosition = 0;

        if (*l_strBuffer == ' ')
        {
            do
                l_currChar = *(l_strCopy + ++l_currPosition);
            while (l_currChar == ' ');
        }

        char l_nextChar = *(l_strCopy + l_currPosition);
        char *l_tokenStart = l_strCopy + l_currPosition;

        if (l_nextChar != '0' && l_nextChar)
        {
            do
            {
                do
                {
                    if (l_nextChar == '+')
                        break;

                    l_nextChar = *(l_strCopy + ++l_currPosition);
                } while (l_nextChar);

                l_savedChar = *(l_strCopy + l_currPosition);
                *(l_strCopy + l_currPosition) = 0;

                DependOnActionNodes(l_tokenStart, p_flag);

                *(l_strCopy + l_currPosition) = l_savedChar;

                if (l_savedChar == '+')
                    ++l_currPosition;

                if (*(l_strCopy + l_currPosition) == ' ')
                {
                    do
                        l_spaceChar = *(l_strCopy + ++l_currPosition);
                    while (l_spaceChar == ' ');
                }

                l_nextChar = *(l_strCopy + l_currPosition);
                l_tokenStart = l_strCopy + l_currPosition;
            } while (l_nextChar);
        }

        delete l_strCopy;
    }
}

void ZInputAction::DependOnActionNodes(const char *p_str, bool p_flag)
{
    if (!m_dependentNodes)
    {
        m_dependentNodes = new RefTab(2, 1);
    }

    ZActionNode *l_actionNode = (ZActionNode *)(m_dependentNodes->Add(0) - 1);

    char *l_nodeNameBuffer = new char[(strlen(p_str) + 1)];

    l_actionNode->nodeName = l_nodeNameBuffer;

    strcpy(l_nodeNameBuffer, p_str);
    l_actionNode->flag = p_flag;
}

bool ZInputAction::CheckDependencies()
{
    RefRun l_refRun;

    if (!m_dependentNodes)
        return true;

    m_dependentNodes->RunInitNxtRef(&l_refRun);
    ZActionNode *l_nextNode = (ZActionNode *)m_dependentNodes->RunNxtRefPtr(&l_refRun);

    if (!l_nextNode)
        return true;

    char *l_nodeName;
    ZInputAction *l_actionBase1;

    while (true)
    {
        l_nodeName = l_nextNode->nodeName;

        if (!l_nextNode->flag)
            break;

        l_actionBase1 = m_actionMap->GetActionBase(l_nodeName);

        if (l_actionBase1 && !l_actionBase1->m_unkByte1)
            return false;

    NEXT_REF_LBL:

        l_nextNode = (ZActionNode *)m_dependentNodes->RunNxtRefPtr(&l_refRun);

        if (!l_nextNode)
            return true;
    }

    ZInputAction *l_actionBase2 = m_actionMap->GetActionBase(l_nodeName);

    if (!l_actionBase2 || !l_actionBase2->m_unkByte1)
        goto NEXT_REF_LBL;

    return false;
}
