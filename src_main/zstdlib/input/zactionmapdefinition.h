#ifndef ZACTIONMAPDEFINITION_H
#define ZACTIONMAPDEFINITION_H

#include "common.h"
#include "zstdlib/utilities/mystr.h"

#pragma pack(push, 1)

struct SActionOverride
{
    MyStr actionName; // 0
    i32 unkArray[4];  // 124
    bool unkByte1;    // 144
    i32 handlerType;  // 145
    char *unkInt7;    // 149
    char *unkInt8;    // 153
}; // 157 in size.

struct ZActionNode
{
    char *nodeName; // 0
    bool flag;      // 4
}; // 5 in size.

struct SInputActionDefinition
{
    char *commandName; // 0
    i32 handlerType;   // 4
    char *keyBinding;  // 8
    char *modifierKey; // 12
    char *unkInt2;     // 16
    char *unkInt3;     // 20
    bool unkByte1;     // 24
}; // 25 in size.

class ZActionMapDefinition
{
public:
    virtual SInputActionDefinition *GetMap() = 0;
    virtual bool UnkFunc0() = 0;
};

#pragma pack(pop)

#endif