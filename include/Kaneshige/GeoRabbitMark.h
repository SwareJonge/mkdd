#ifndef GEORABBITMARK_H
#define GEORABBITMARK_H

#include "types.h"

// placeholder classes

class GeoRabbitMarkSupervisor
{
public:
    short getWinFrame() const
    {
        return mWinFrame * 60;
    }

private:
    u8 _0[0x40];
    s16 mWinFrame;
};

class GeoRabbitMark
{
public:
    static GeoRabbitMarkSupervisor *getSupervisor()
    {
        return sSupervisor;
    }

private:
    static u16 sSupervisorCreateNum;
    static GeoRabbitMarkSupervisor *sSupervisor;
};

#endif