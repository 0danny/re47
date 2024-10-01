#include "zdynamicloader.h"

ZDynamicLoader::ZDynamicLoader()
{
    m_libName = 0;
    m_hModule = 0;
}

ZDynamicLoader::~ZDynamicLoader()
{
    Common::FreeArr(m_libName);
}

void ZDynamicLoader::NewLibrary(HMODULE p_hModule, const char *p_libName)
{
    if (m_libName)
    {
        Common::FreeArr(m_libName);
    }

    int l_strLen = strlen(p_libName) + 1;

    char *l_strMalloc = (char *)Common::AllocArr(l_strLen);

    m_libName = l_strMalloc;

    strcpy(l_strMalloc, p_libName);

    m_hModule = p_hModule;
}

FARPROC ZDynamicLoader::GetProc(const char *p_procName)
{
    if (m_hModule)
        return GetProcAddress(m_hModule, p_procName);
    else
        return 0;
}

void ZDynamicLoader::Unknown1()
{
    return;
}

void ZDynamicLoader::Unknown2()
{
    return;
}

void ZDynamicLoader::Unknown3()
{
    return;
}

void ZDynamicLoader::Unknown4()
{
    return;
}

void ZDynamicLoader::Unknown5()
{
    return;
}

void ZDynamicLoader::Unknown6()
{
    return;
}

void ZDynamicLoader::Unknown7()
{
    return;
}

void ZDynamicLoader::Unknown8()
{
    return;
}

void ZDynamicLoader::Unknown9()
{
    return;
}

void ZDynamicLoader::Unknown10()
{
    return;
}

void ZDynamicLoader::Unknown11()
{
    return;
}
