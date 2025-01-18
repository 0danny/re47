#include "directplaydll.h"

DirectPlayDll::~DirectPlayDll()
{
}

void DirectPlayDll::Init()
{
    m_directPlay = new DirectPlay();
}

void DirectPlayDll::End()
{
    if (m_directPlay)
        delete m_directPlay;
}
