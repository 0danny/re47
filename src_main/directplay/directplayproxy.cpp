#include "directplayproxy.h"

DirectPlayProxy::~DirectPlayProxy()
{
    ZDynamicLoader::~ZDynamicLoader();
}

void DirectPlayProxy::Unknown2()
{
    m_directPlay = new DirectPlay();
}

void DirectPlayProxy::Unknown3()
{
    if (m_directPlay)
    {
        m_directPlay->~DirectPlay();
    }
}
