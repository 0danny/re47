#include "zinputactions.h"

ZInputActions::ZInputActions()
{
}

ZInputActions::~ZInputActions()
{
}

void ZInputActions::OverrideActions(RefTab *l_actionsList)
{
}

void ZInputActions::OverrideAction(ZActionOverride *p_actionOverride)
{
}

ZActionOverride *ZInputActions::FindOverriden(char *p_str)
{
    return 0;
}

void ZInputActions::AddActionMap(ZActionMapDefinition *p_actionMapDef, const char *p_str)
{
}

void ZInputActions::RemoveActionMap(ZActionMap *p_actionMap)
{
}

void ZInputActions::RemoveActionMap(char *p_actionStr)
{
}

void ZInputActions::RemoveActionMap(ZActionMapDefinition *p_actionMapDef)
{
}

void ZInputActions::RemoveActionMaps()
{
}

boolean ZInputActions::UnkFunc7(i32 p_unkInt, i32 p_unkInt2)
{
    return FALSE;
}

boolean ZInputActions::UnkFunc8(char *p_actionName, i32 p_unkInt)
{
    return FALSE;
}

void ZInputActions::UnkFunc9(i32 p_unkInt)
{
}

u32 *ZInputActions::UnkFunc10(const char *p_str)
{
    return 0;
}

ZActionMap *ZInputActions::GetActionMap(ZActionMapDefinition *p_actionMapDef)
{
    return 0;
}

ZActionMap *ZInputActions::GetActionMap(char *p_str)
{
    return 0;
}

u32 ZInputActions::GetInputNode(char *p_str)
{
    return 0;
}

void ZInputActions::EnableActions()
{
}

void ZInputActions::DisableActions()
{
}

void ZInputActions::ActivateMap(char *p_str)
{
}

void ZInputActions::DeactivateMap(char *p_str)
{
}

void ZInputActions::UnkFunc18(i32 p_unkInt, i32 p_unkInt2)
{
}

void *ZInputActions::UnkFunc19(i32 p_unkInt)
{
    return 0;
}
