#ifndef DIRECTPLAYPROXY_H
#define DIRECTPLAYPROXY_H

#include "common.h"
#include "zstdlib/zdynamicloader.h"
#include "directplay.h"

class DirectPlayProxy : public ZDynamicLoader
{
public:
    DirectPlay *m_directPlay;

    virtual ~DirectPlayProxy();

    // Overrides
    void Init() override;
    void End() override;
};

#endif