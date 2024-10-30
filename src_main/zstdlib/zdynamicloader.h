#ifndef ZDYNAMICLOADER_H
#define ZDYNAMICLOADER_H

#include "common.h"

class ZDynamicLoader
{
public:
    char *m_libName;
    HMODULE m_hModule;

    ZDynamicLoader();

    virtual ~ZDynamicLoader();

    virtual void FreeMem();
    virtual void Init();
    virtual void End();
    virtual void PushScene();
    virtual void PopScene();
    virtual void AllocSequenceStart();
    virtual void AllocSequenceEnd();
    virtual void InsertStart();
    virtual void InsertEnd();
    virtual void CrashFree();

    virtual void Set(HMODULE p_hModule, const char *p_libName);
    virtual FARPROC GetAddress(const char *p_procName);

    virtual void PrintStatus();
};

#endif