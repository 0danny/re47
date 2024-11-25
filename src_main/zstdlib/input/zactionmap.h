#ifndef ZACTIONMAP_H
#define ZACTIONMAP_H

#include "common.h"
#include "zstdlib/reftab/reftab.h"
#include "zstdlib/input/zactionmapdefinition.h"
#include "zstdlib/input/zinputactions.h"
#include "zstdlib/cfastlookup2.h"

#pragma pack(push, 1)

class ZInputAction;

class ZActionMap
{
public:
    ZInputActions *m_inputActions;        // 4
    char *m_mapName;                      // 8
    ZActionMapDefinition *m_actionMapDef; // 12
    RefTab *m_refTab;                     // 16
    i32 m_unkInt1;                        // 20
    u8 m_unkByte;                         // 24
    CFastLookup2 *m_fastLookup;           // 25
    i32 m_unkInt3;                        // 29

    ZActionMap(ZInputActions *p_inputActions, ZActionMapDefinition *p_actionMapDef, char *p_mapName);

    virtual ZInputAction *AddAction(ZActionOverride *p_actionOverride);
    virtual ZInputAction *AddAction(ZActionDefinition *p_actionDefinition);

    virtual void RemoveAction(char *p_str);
    virtual void RemoveActions();

    virtual ZInputAction *GetActionBase(char *p_str);

    virtual void UnkFunc6(i32 p_unkInt, boolean p_flag);
    virtual void UnkFunc7(i32 p_unkInt, boolean p_flag);

    virtual void ActivateMap();
    virtual void DeactivateMap();

    virtual boolean IsActivated();
}; // 33 in size.

#pragma pack(pop)

#endif