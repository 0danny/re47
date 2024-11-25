#ifndef ZINPUTACTIONS_H
#define ZINPUTACTIONS_H

#include "common.h"
#include "zstdlib/reftab/reftab.h"
#include "zstdlib/cfastlookup2.h"
#include "zstdlib/input/sinputnode.h"
#include "zstdlib/input/zactionmapdefinition.h"

class ZActionMap;

#pragma pack(push, 1)

class ZInputActions
{
public:
    SInputNode *m_inputNodeList;    // 4
    i32 m_unkInt2;                  // 8
    i32 m_unkInt3;                  // 12
    i32 m_inputNodeCount;           // 16
    RefTab *m_actionMapDefinitions; // 20
    RefTab *m_refTab2;              // 24
    i32 m_unkInt5;                  // 28
    i32 m_unkInt6;                  // 32
    i32 m_unkInt7;                  // 36
    i32 m_unkInt8;                  // 40
    CFastLookup2 *m_fastLookup;     // 44
    ZActionMap *m_actionMap;        // 48
    RefTab *m_overrideList;         // 52

    ZInputActions();
    ~ZInputActions();

    virtual void OverrideActions(RefTab *l_actionsList);
    virtual void OverrideAction(ZActionOverride *p_actionOverride);

    virtual ZActionOverride *FindOverriden(char *p_str);
    virtual void AddActionMap(ZActionMapDefinition *p_actionMapDef, const char *p_str);

    virtual void RemoveActionMap(ZActionMap *p_actionMap);
    virtual void RemoveActionMap(char *p_actionStr);
    virtual void RemoveActionMap(ZActionMapDefinition *p_actionMapDef);
    virtual void RemoveActionMaps();

    virtual boolean UnkFunc7(i32 p_unkInt, i32 p_unkInt2);
    virtual boolean UnkFunc8(char *p_actionName, i32 p_unkInt);
    virtual void UnkFunc9(i32 p_unkInt);
    virtual u32 *UnkFunc10(const char *p_str);

    virtual ZActionMap *GetActionMap(ZActionMapDefinition *p_actionMapDef);
    virtual ZActionMap *GetActionMap(char *p_str);
    virtual u32 GetInputNode(char *p_str);

    virtual void EnableActions();
    virtual void DisableActions();

    virtual void ActivateMap(char *p_str);
    virtual void DeactivateMap(char *p_str);

    virtual void UnkFunc18(i32 p_unkInt, i32 p_unkInt2);
    virtual void *UnkFunc19(i32 p_unkInt);
}; // 56 in size.

#pragma pack(pop)

#endif