#pragma once

#include "common.h"

class ZDynamicLoader
{
public:
    char *m_libName;
    HMODULE m_hModule;

    ZDynamicLoader();

    virtual ~ZDynamicLoader();

    virtual void Unknown1();
    virtual void Unknown2();
    virtual void Unknown3();
    virtual void Unknown4();
    virtual void Unknown5();
    virtual void Unknown6();
    virtual void Unknown7();
    virtual void Unknown8();
    virtual void Unknown9();
    virtual void Unknown10();

    virtual void NewLibrary(HMODULE p_hModule, const char *p_libName);
    virtual FARPROC GetProc(const char *p_procName);

    virtual void Unknown11();
};