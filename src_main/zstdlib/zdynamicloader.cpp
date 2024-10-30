#include "zdynamicloader.h"

ZDynamicLoader::ZDynamicLoader()
{
    m_libName = 0;
    m_hModule = 0;
}

ZDynamicLoader::~ZDynamicLoader()
{
    delete[] m_libName;
}

void ZDynamicLoader::Set(HMODULE p_hModule, const char *p_libName)
{
    if (m_libName)
    {
        delete[] m_libName;
    }

    i32 l_strLen = strlen(p_libName) + 1;

    char *l_strMalloc = new char[l_strLen];

    m_libName = l_strMalloc;

    strcpy(l_strMalloc, p_libName);

    m_hModule = p_hModule;
}

FARPROC ZDynamicLoader::GetAddress(const char *p_procName)
{
    if (m_hModule)
        return GetProcAddress(m_hModule, p_procName);
    else
        return 0;
}

void ZDynamicLoader::FreeMem()
{
    return;
}

void ZDynamicLoader::Init()
{
    return;
}

void ZDynamicLoader::End()
{
    return;
}

void ZDynamicLoader::PushScene()
{
    return;
}

void ZDynamicLoader::PopScene()
{
    return;
}

void ZDynamicLoader::AllocSequenceStart()
{
    return;
}

void ZDynamicLoader::AllocSequenceEnd()
{
    return;
}

void ZDynamicLoader::InsertStart()
{
    return;
}

void ZDynamicLoader::InsertEnd()
{
    return;
}

void ZDynamicLoader::CrashFree()
{
    return;
}

void ZDynamicLoader::PrintStatus()
{
    return;
}
