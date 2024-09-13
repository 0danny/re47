#pragma once

#include <windows.h>

// 0x27 in size

// Virtual base class for the ZSysCom
class ZSysComBase
{
public:
};

class ZSysCom : public ZSysComBase
{
public:
};

extern __declspec(dllimport) ZSysCom *g_pSysCom;