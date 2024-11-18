#include "directplayproxy.h"

DirectPlayProxy::~DirectPlayProxy()
{
    ZDynamicLoader::~ZDynamicLoader();
}

void DirectPlayProxy::Init()
{
    m_directPlay = new DirectPlay();
}

void DirectPlayProxy::End()
{
    if (m_directPlay)
    {
        m_directPlay->~DirectPlay();
    }
}
