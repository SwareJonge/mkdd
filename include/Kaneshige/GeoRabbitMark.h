#ifndef GEORABBITMARK_H
#define GEORABBITMARK_H

#include "types.h"

// placeholder classes

class GeoRabbitMarkSupervisor
{
public:
    void release();    
    int getRabbitKartNo();
    short getWinFrame() const
    {
        return mWinTime * 60;
    }

    short getWinTime() const
    {
        return mWinTime;
    }
    void startWinnerEffect();

private:
    u8 _0[0x40];
    s16 mWinTime;
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