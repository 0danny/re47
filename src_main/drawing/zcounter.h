#ifndef ZCOUNTER_H
#define ZCOUNTER_H

#include "zstdlib/reftab/linkreftab.h"
#include "system/zsysinterface.h"
#include "common.h"

#pragma pack(push, 1)

// The last entry "lastActiveIndex" is at (this + 1677), and is a full register mov.
// Which doesn't make sense because that would make the struct 1681 bytes, when the linkreftab specifies 420 * 4 = 1680 bytes.
// but it works, so im leaving it (:
struct ZCounterInstance
{
    i64 totalTime;
    i64 currentRunTime;
    i64 startTimestamp;
    i64 cpuCyclesCount;
    i64 overheadTime;
    i64 historicalEntries[200];
    char counterName[20];
    ZCounterInstance *parentCounter;
    i32 depth;
    u8 isActive;
    i32 callCount;
    i32 isMarkedForDeletion;
    i32 lastActiveIndex;
};

class ZCounter
{
public:
    i32 m_unkInt1;                      // 4
    i32 m_linkTabCount;                 // 8
    i32 m_historyIndex;                 // 12
    ZCounterInstance *m_currentCounter; // 16
    i32 m_unkInt5;                      // 20
    LinkRefTab *m_linkTab;              // 24
    i32 m_activeCounters;               // 28
    u8 m_isDisabled;                    // 32

    ZCounter();
    ~ZCounter();

    virtual void BeginCounter(char *p_counterName, u64 p_timeTaken);
    virtual void EndCounter(u64 p_endTime);
    virtual void UpdateCounters();
    virtual void ResetHistory();

    virtual i32 GetCount();
    virtual i32 GetHistoryIndex();
    virtual LinkRefTab *GetLinkTab();
}; // 33 in size.

#pragma pack(pop)

#endif