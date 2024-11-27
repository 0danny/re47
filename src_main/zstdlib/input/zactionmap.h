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

    inline ~ZActionMap()
    {
        if (m_mapName)
            delete m_mapName;

        RemoveActions();

        if (m_refTab)
            m_refTab->~RefTab();

        if (m_fastLookup)
            m_fastLookup->~CFastLookup2();

        if (m_inputActions->m_actionMap == this)
            m_inputActions->m_actionMap = 0;
    }

    virtual ZInputAction *AddAction(SActionOverride *p_actionOverride);
    virtual ZInputAction *AddAction(SInputActionDefinition *p_actionDefinition);

    virtual void RemoveAction(char *p_actionName);
    virtual void RemoveActions();

    virtual ZInputAction *GetActionBase(char *p_actionName);

    virtual void LockActions(char *p_str, boolean p_flag);
    virtual void UnlockActions(char *p_str, boolean p_flag);

    virtual void ActivateMap();
    virtual void DeactivateMap();

    virtual boolean IsActivated();

}; // 33 in size.

#pragma pack(pop)

#endif