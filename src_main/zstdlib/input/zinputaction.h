#ifndef ZINPUTACTION_H
#define ZINPUTACTION_H

#include "common.h"
#include "zstdlib/reftab/reftab.h"
#include "zstdlib/mystr.h"
#include "zstdlib/input/zactionmap.h"

#pragma pack(push, 1)

struct ZInputActionBinding
{
    ZInputAction *inputAction; // 0
    boolean flag;              // 4
}; // 5 in size.

class ZInputAction
{
public:
    ZActionMap *m_actionMap;   // 4
    i32 m_activeNodeCount;     // 8
    i32 m_lockCount;           // 12
    char *m_actionName;        // 16
    i32 m_totalNodeCount;      // 20
    RefTab *m_nodeReferences;  // 24
    u8 m_unkByte1;             // 28
    u8 m_unkByte2;             // 29
    f32 m_unkFloat1;           // 30
    f32 m_unkFloat2;           // 34
    RefTab *m_pressHandlers;   // 38
    RefTab *m_holdHandlers;    // 42
    RefTab *m_releaseHandlers; // 46
    u8 m_unkByte3;             // 50
    RefTab *m_dependentNodes;  // 51

    ZInputAction(ZActionMap *p_actionMap, SActionOverride *p_actionOverride);
    ZInputAction(ZActionMap *p_actionMap, SInputActionDefinition *p_actionDefinition);

    ~ZInputAction();

    virtual void AddNode(char *p_vkKey, boolean p_flag);
    virtual void AddNode(i32 p_refNum, boolean p_flag);

    virtual void AddNodes(i32 *p_ref, boolean p_flag);
    virtual void AddNodes(const char *p_str, boolean p_flag);

    virtual void LockInputNode(char *p_vkKey, boolean p_flag, boolean p_localMapOnly);
    virtual void UnlockInputNode(char *p_vkKey, boolean p_flag, boolean p_localMapOnly);

    virtual void MakeActionExclusiveOwnerOfNodes(boolean p_lock);

    void DependOnActionsNodes(const char *p_str, boolean p_flag);
    void DependOnActionNodes(const char *p_str, boolean p_flag);

    boolean CheckDependencies();

}; // 55 in size.

#pragma pack(pop)

#endif