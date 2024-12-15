#ifndef ZINPUTACTIONS_H
#define ZINPUTACTIONS_H

#include "common.h"
#include "zstdlib/reftab/reftab.h"
#include "zstdlib/cfastlookup2.h"
#include "zstdlib/input/sinputnode.h"
#include "zstdlib/input/zactionmapdefinition.h"
#include "system/zsysinterface.h"

class ZActionMap;
class ZInputAction;

struct SInputEntry
{
    char *vkName;    // 0
    i32 index;       // 4
    char *entryName; // 8
}; // 12 in size.

struct SInputList
{
    i32 totalCount;    // 0
    char *listName;    // 4
    SInputEntry *list; // 8
    i32 listSize;      // 12
}; // 16 in size.

static SInputEntry g_keysInputList[] = {
    {"VK_ESCAPE", 1, "Escape"},
    {"VK_1", 2, "1"},
    {"VK_2", 3, "2"},
    {"VK_3", 4, "3"},
    {"VK_4", 5, "4"},
    {"VK_5", 6, "5"},
    {"VK_6", 7, "6"},
    {"VK_7", 8, "7"},
    {"VK_8", 9, "8"},
    {"VK_9", 10, "9"},
    {"VK_0", 11, "0"},
    {"?2", 12, "?2"},
    {"?3", 13, "?3"},
    {"VK_BACK", 14, "Backspace"},
    {"VK_TAB", 15, "Tab"},
    {"VK_Q", 16, "Q"},
    {"VK_W", 17, "W"},
    {"VK_E", 18, "E"},
    {"VK_R", 19, "R"},
    {"VK_T", 20, "T"},
    {"VK_Y", 21, "Y"},
    {"VK_U", 22, "U"},
    {"VK_I", 23, "I"},
    {"VK_O", 24, "O"},
    {"VK_P", 25, "P"},
    {"?4", 26, "?4"},
    {"?5", 27, "?5"},
    {"VK_RETURN", 28, "Return"},
    {"VK_CONTROL", 29, "LeftControl"},
    {"VK_A", 30, "A"},
    {"VK_S", 31, "S"},
    {"VK_D", 32, "D"},
    {"VK_F", 33, "F"},
    {"VK_G", 34, "G"},
    {"VK_H", 35, "H"},
    {"VK_J", 36, "J"},
    {"VK_K", 37, "K"},
    {"VK_L", 38, "L"},
    {"?6", 39, "M"},
    {"?7", 40, "?7"},
    {"?8", 41, "?8"},
    {"VK_SHIFT", 42, "LeftShift"},
    {"?9", 43, "?9"},
    {"VK_Z", 44, "Z"},
    {"VK_X", 45, "X"},
    {"VK_C", 46, "C"},
    {"VK_V", 47, "V"},
    {"VK_B", 48, "B"},
    {"VK_N", 49, "N"},
    {"VK_M", 50, "M"},
    {"?9", 51, "?9"},
    {"?10", 52, "?10"},
    {"VK_DIVIDE", 53, "Divide"},
    {"VK_RSHIFT", 54, "VK_RSHIFT"},
    {"VK_MULTIPLY", 55, "NUM *"},
    {"VK_MENU", 56, "LeftMenu"},
    {"VK_SPACE", 57, "Space"},
    {"VK_CAPITAL", 58, "Caps lock"},
    {"VK_F1", 59, "F1"},
    {"VK_F2", 60, "F2"},
    {"VK_F3", 61, "F3"},
    {"VK_F4", 62, "F4"},
    {"VK_F5", 63, "F5"},
    {"VK_F6", 64, "F6"},
    {"VK_F7", 65, "F7"},
    {"VK_F8", 66, "F8"},
    {"VK_F9", 67, "F9"},
    {"VK_F10", 68, "F10"},
    {"VK_NUMLOCK", 69, "NUM Lock"},
    {"VK_SCROLL", 70, "Scroll lock"},
    {"VK_NUMPAD7", 71, "NUM 7"},
    {"VK_NUMPAD8", 72, "NUM 8"},
    {"VK_NUMPAD9", 73, "NUM 9"},
    {"VK_SUBTRACT", 74, "NUM -"},
    {"VK_NUMPAD4", 75, "NUM 4"},
    {"VK_NUMPAD5", 76, "NUM 5"},
    {"VK_NUMPAD6", 77, "NUM 6"},
    {"VK_ADD", 78, "NUM +"},
    {"VK_NUMPAD1", 79, "NUM 1"},
    {"VK_NUMPAD2", 80, "NUM 2"},
    {"VK_NUMPAD3", 81, "NUM 3"},
    {"VK_NUMPAD0", 82, "NUM 0"},
    {"VK_DECIMAL", 83, "NUM ,"},
    {"VK_SNAPSHOT", 84, "Snapshot"},
    {"?12", 85, "?12"},
    {"?13", 86, "?13"},
    {"VK_F11", 87, "F11"},
    {"VK_F12", 88, "F12"},
    {"?14", 89, "?14"},
    {"?15", 90, "?15"},
    {"?16", 91, "?16"},
    {"?17", 92, "?17"},
    {"?18", 93, "?18"},
    {"?19", 94, "?19"},
    {"?20", 95, "?20"},
    {"?21", 96, "?21"},
    {"?22", 97, "?22"},
    {"?23", 98, "?23"},
    {"VK_HELP", 99, "Help"},
    {"VK_F13", 100, "F13"},
    {"VK_F14", 101, "F14"},
    {"VK_F15", 102, "F15"},
    {"VK_F16", 103, "F16"},
    {"VK_F17", 104, "F17"},
    {"VK_F18", 105, "F18"},
    {"VK_F19", 106, "F19"},
    {"VK_F20", 107, "F20"},
    {"VK_F21", 108, "F21"},
    {"VK_F22", 109, "F22"},
    {"VK_F23", 110, "F23"},
    {"?24", 111, "?24"},
    {"?25", 112, "?25"},
    {"?26", 113, "?26"},
    {"?27", 114, "?27"},
    {"?28", 115, "?28"},
    {"?29", 116, "?29"},
    {"?30", 117, "?30"},
    {"VK_F24", 118, "F24"},
    {"???", 119, "???"},
    {"???", 120, "???"},
    {"???", 121, "???"},
    {"???", 122, "???"},
    {"???", 123, "???"},
    {"???", 124, "???"},
    {"???", 125, "???"},
    {"???", 126, "???"},
    {"???", 127, "???"},
    {"???", 128, "???"},
    {"???", 129, "???"},
    {"???", 130, "???"},
    {"???", 131, "???"},
    {"???", 132, "???"},
    {"???", 133, "???"},
    {"???", 134, "???"},
    {"???", 135, "???"},
    {"???", 136, "???"},
    {"???", 137, "???"},
    {"???", 138, "???"},
    {"???", 139, "???"},
    {"???", 140, "???"},
    {"???", 141, "???"},
    {"???", 142, "???"},
    {"???", 143, "???"},
    {"VK_NUMRETURN", 284, "NumReturn"},
    {"VK_RCONTROL", 285, "VK_RCONTROL"},
    {"VK_NUMDIVIDE", 309, "Num Divide"},
    {"VK_RMENU", 312, "VK_RMENU"},
    {"VK_UP", 328, "Up"},
    {"VK_LEFT", 331, "Left"},
    {"VK_RIGHT", 333, "Right"},
    {"VK_DOWN", 336, "Down"},
    {"VK_LBUTTON", 356, "Left button"},
    {"VK_RBUTTON", 357, "Right button"},
    {"VK_MBUTTON", 358, "Middle button"},
    {"VK_SELECT", 359, "Select"},
    {"VK_CANCEL", 326, "Cancel"},
    {"VK_HOME", 327, "Home"},
    {"VK_PRIOR", 329, "PageUp"},
    {"VK_CLEAR", 332, "Clear"},
    {"VK_END", 335, "End"},
    {"VK_NEXT", 337, "PageDown"},
    {"VK_INSERT", 338, "Insert"},
    {"VK_DELETE", 339, "Delete"},
    {"VK_INVALID", 511, "Invalid"},
    {0, 0, 0},
};

static SInputEntry g_mouseInputList[] = {
    {"MW_UP", 0, "Mouse wheel up"},
    {"MW_DOWN", 1, "Mouse wheel down"},
    {0, 0, 0},
};

static SInputList g_inputLists[] = {
    {0, "KeysInputList", g_keysInputList, 165},
    {0, "MouseWheelInputList", g_mouseInputList, 3},
    {0, 0, 0, 0},
};

struct SInputNodeList
{
    i32 count;
    SInputNode *nodes;
};

class ZInputActions
{
public:
    SInputNode *m_inputNodeList;    // 4
    i32 m_unkInt2;                  // 8
    i32 m_activeCount;              // 12
    i32 m_inputNodeCount;           // 16
    RefTab *m_actionMapDefinitions; // 20
    RefTab *m_activeActions;        // 24
    bool *m_curStates;              // 28
    f32 m_lastUpdateTime;           // 32
    i32 m_unkInt4;                  // 36
    f32 m_timeSinceLastUpdate;      // 40
    CFastLookup2 *m_fastLookup;     // 44
    ZActionMap *m_actionMap;        // 48
    RefTab *m_overrideList;         // 52

    ZInputActions();
    ~ZInputActions();

    virtual void OverrideActions(RefTab *l_actionsList);
    virtual void OverrideAction(SActionOverride *p_actionOverride);

    virtual SActionOverride *FindOverriden(char *p_str);
    virtual void AddActionMap(ZActionMapDefinition *p_actionMapDef, char *p_str);

    virtual void RemoveActionMap(ZActionMap *p_actionMap);
    virtual void RemoveActionMap(char *p_actionStr);
    virtual void RemoveActionMap(ZActionMapDefinition *p_actionMapDef);
    virtual void RemoveActionMaps();

    virtual bool RegisterInputAction(ZInputAction *p_action, i32 p_refNum);
    virtual bool RegisterInputActionByName(char *p_actionName, u32 p_refNum);
    virtual void RefreshKeyStates(RefTab *p_tab1, RefTab *p_unused);

    virtual ZInputAction *GetInputAction(char *p_actionName);
    virtual ZActionMap *GetActionMap(ZActionMapDefinition *p_actionMapDef);
    virtual ZActionMap *GetActionMap(char *p_str);
    virtual i32 GetInputNode(char *p_vkName);

    virtual void EnableActions();
    virtual void DisableActions();

    virtual void ActivateMap(char *p_str);
    virtual void DeactivateMap(char *p_str);

    virtual void MakeActionExclusiveOwner(char *p_vkName, bool p_flag);
    virtual SInputEntry *GetListByIndex(i32 p_index);

    void RemoveOverrideList();
    void RefreshMouseStates();

}; // 56 in size.

#endif