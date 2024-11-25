#ifndef DIRECTPLAYPROXY_H
#define DIRECTPLAYPROXY_H

#include "common.h"
#include "zstdlib/zdynamicloader.h"
#include "directplay.h"

class DirectPlayDll : public ZDynamicLoader
{
public:
    DirectPlay *m_directPlay;

    virtual ~DirectPlayDll();

    // Overrides
    void Init() override;
    void End() override;
};

#endif