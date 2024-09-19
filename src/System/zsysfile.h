#pragma once

#include <windows.h>
#include <stdio.h>

#include "typedefs.h"

#pragma pack(push, 1)

class ZSysFile;

class ZSysFile
{
public:
};

#pragma pack(pop)

extern __declspec(dllimport) ZSysFile *g_pSysFile;