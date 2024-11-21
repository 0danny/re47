#include "zcounter.h"

ZCounter::ZCounter()
{
    m_linkTab = 0;
    m_unkInt5 = 0;
    m_currentCounter = 0;
    m_activeCounters = 0;
    m_isDisabled = 0;
    m_historyIndex = 0;
    m_unkInt1 = 0;
    m_linkTabCount = 0;
}

ZCounter::~ZCounter()
{
    if (m_linkTab)
        m_linkTab->~LinkRefTab();
}

void ZCounter::BeginCounter(char *p_counterName, u64 p_startTime)
{
    RefRun l_refRun;
    ZCounterInstance *l_foundCounter;
    ZCounterInstance *l_counterStruct;
    u32 *l_nextRefPtr;

    if (!m_isDisabled)
    {
        if (strlen(p_counterName) + 1 > 19)
            p_counterName = "Too Much Text";

        if (!m_linkTab)
        {
            i32 l_unused = 0;

            m_linkTab = new LinkRefTab(8, 420);

            l_unused = -1;
        }

        if (m_linkTab && (m_linkTab->RunInitNxtRef(&l_refRun), (l_foundCounter = (ZCounterInstance *)m_linkTab->RunNxtRefPtr(&l_refRun)) != 0))
        {
            while (l_foundCounter->parentCounter != m_currentCounter || _strcmpi(l_foundCounter->counterName, p_counterName))
            {
                l_foundCounter = (ZCounterInstance *)m_linkTab->RunNxtRefPtr(&l_refRun);

                if (!l_foundCounter)
                    goto CREATE_NEW_COUNTER;
            }
        }
        else
        {
        CREATE_NEW_COUNTER:
            if (!m_currentCounter || (l_nextRefPtr = m_linkTab->GetNextRefPtr((u32 *)m_currentCounter)) == 0 || (l_counterStruct = (ZCounterInstance *)(m_linkTab->InsertBefore(l_nextRefPtr, 0) - 1), l_counterStruct == 0))
            {
                l_counterStruct = (ZCounterInstance *)(m_linkTab->Add(0) - 1);
            }

            l_counterStruct->totalTime = 0;

            l_counterStruct->currentRunTime = 0;
            l_counterStruct->cpuCyclesCount = 0;
            l_counterStruct->overheadTime = 0;

            memset(l_counterStruct->historicalEntries, 0, sizeof(l_counterStruct->historicalEntries));

            l_counterStruct->depth = m_activeCounters;
            l_counterStruct->callCount = 0;
            l_counterStruct->isMarkedForDeletion = 0;
            l_counterStruct->parentCounter = m_currentCounter;
            l_counterStruct->isActive = 0;

            strcpy(l_counterStruct->counterName, p_counterName);

            l_foundCounter = l_counterStruct;
        }

        l_foundCounter->startTimestamp = p_startTime;

        if (l_foundCounter->parentCounter != m_currentCounter)
        {
            l_foundCounter->isActive = 1;
            l_foundCounter->parentCounter = m_currentCounter;
        }

        m_currentCounter = l_foundCounter;

        ++m_activeCounters;

        u64 l_cpuCycles = GetRDTSC();
        l_foundCounter->cpuCyclesCount = (l_cpuCycles - p_startTime);
    }
}

void ZCounter::EndCounter(u64 p_endTime)
{
    if (!m_isDisabled)
    {
        ZCounterInstance *l_curCounter = m_currentCounter;

        u64 elapsedTime = p_endTime - l_curCounter->startTimestamp;

        l_curCounter->totalTime += elapsedTime;
        l_curCounter->currentRunTime += elapsedTime;
        l_curCounter->callCount++;

        m_currentCounter = l_curCounter->parentCounter;

        --m_activeCounters;

        u64 currentCycles = GetRDTSC();
        l_curCounter->overheadTime += (currentCycles - p_endTime);
    }
}

void ZCounter::UpdateCounters()
{
    RefRun l_refRun;

    if (g_pSysInterface->m_enableDebugOptions == 0.0 || g_pSysInterface->m_enableTimers == 0.0 || m_isDisabled)
        return;

    if (m_activeCounters)
    {
        ZSysCom *l_sysCom = g_pSysCom->SetPathAndLine("Z:\\Engine\\Drawing\\Source\\ZCounter.cpp", 126);
        l_sysCom->LogMessage("ERROR: Time Counters not pairing correctly -> Disabling Counters");

        if (m_linkTab)
            m_linkTab->~LinkRefTab();

        i32 l_unused = 0;

        m_linkTab = new LinkRefTab(8, 420);

        l_unused = -1;

        ZCounterInstance *l_data = (ZCounterInstance *)(m_linkTab->Add(0) - 1);

        strcpy(l_data->counterName, "Not pairing correct");

        l_data->totalTime = 0;
        l_data->currentRunTime = 0;
        l_data->cpuCyclesCount = 0;
        l_data->overheadTime = 0;

        l_data->parentCounter = 0;

        m_isDisabled = 1;
        return;
    }

    m_linkTabCount = m_linkTab->GetCount();

    if (m_linkTab)
    {
        m_linkTab->RunInitNxtRef(&l_refRun);

        for (ZCounterInstance *l_item = (ZCounterInstance *)m_linkTab->RunNxtRefPtr(&l_refRun); l_item; l_item = (ZCounterInstance *)m_linkTab->RunNxtRefPtr(&l_refRun))
        {
            l_item->historicalEntries[m_historyIndex] = l_item->totalTime;
            l_item->totalTime = 0;
            l_item->currentRunTime = 0;

            if (l_item->callCount == 0)
            {
                if (l_item->lastActiveIndex == m_historyIndex && !l_item->isMarkedForDeletion)
                {
                    m_linkTab->RunDelRef(&l_refRun);
                    continue;
                }
            }
            else
            {
                l_item->lastActiveIndex = m_historyIndex;
            }

            l_item->cpuCyclesCount = 0;
            l_item->overheadTime = 0;
            l_item->callCount = 0;
        }
    }

    i32 l_historyIndex = m_historyIndex + 1;

    m_currentCounter = 0;

    m_historyIndex = l_historyIndex;

    if (l_historyIndex == 200)
        m_historyIndex = 0;
}

void ZCounter::ResetHistory()
{
    RefRun l_refRun;

    if (g_pSysInterface->m_enableDebugOptions != 0.0 && g_pSysInterface->m_enableTimers != 0.0 && !m_isDisabled)
    {
        if (m_linkTab)
        {
            m_linkTab->RunInitNxtRef(&l_refRun);

            for (ZCounterInstance *l_item = (ZCounterInstance *)m_linkTab->RunNxtRefPtr(&l_refRun); l_item; l_item = (ZCounterInstance *)m_linkTab->RunNxtRefPtr(&l_refRun))
            {
                memset(l_item->historicalEntries, 0, sizeof(l_item->historicalEntries));
            }
        }
    }
}

i32 ZCounter::GetCount()
{
    if (m_linkTab)
        return m_linkTab->GetCount();
    else
        return 0;
}

i32 ZCounter::GetHistoryIndex()
{
    return m_historyIndex;
}

LinkRefTab *ZCounter::GetLinkTab()
{
    return m_linkTab;
}
