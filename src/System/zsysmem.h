#pragma once

#include <windows.h>
#include <stdio.h>

#include "typedefs.h"

#pragma pack(push, 1)

class ZSysMem;

class ZSysMem
{
public:
};

#pragma pack(pop)

extern __declspec(dllimport) ZSysMem *g_pSysMem;