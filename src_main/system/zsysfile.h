#ifndef ZSYSFILE_H
#define ZSYSFILE_H

#include "common.h"

#pragma pack(push, 1)

class ZSysFile
{
public:
    ZSysFile();
};

#pragma pack(pop)

extern __declspec(dllimport) ZSysFile *g_pSysFile;

#endif