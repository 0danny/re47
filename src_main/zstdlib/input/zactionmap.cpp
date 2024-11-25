#include "zactionmap.h"

ZActionMap::ZActionMap(ZInputActions *p_inputActions, ZActionMapDefinition *p_actionMapDef, char *p_mapName)
{
}

ZInputAction *ZActionMap::AddAction(ZActionOverride *p_actionOverride)
{
    return 0;
}

ZInputAction *ZActionMap::AddAction(ZActionDefinition *p_actionDefinition)
{
    return 0;
}

void ZActionMap::RemoveAction(char *p_str)
{
}

void ZActionMap::RemoveActions()
{
}

ZInputAction *ZActionMap::GetActionBase(char *p_str)
{
    return 0;
}

void ZActionMap::UnkFunc6(i32 p_unkInt, boolean p_flag)
{
}

void ZActionMap::UnkFunc7(i32 p_unkInt, boolean p_flag)
{
}

void ZActionMap::ActivateMap()
{
}

void ZActionMap::DeactivateMap()
{
}

boolean ZActionMap::IsActivated()
{
    return FALSE;
}
