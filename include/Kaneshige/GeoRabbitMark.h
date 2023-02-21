#ifndef GEORABBITMARK_H
#define GEORABBITMARK_H

#include "types.h"

// placeholder classes

class GeoRabbitMarkSupervisor;

class GeoRabbitMark
{
public:
    void equip(int);

    static GeoRabbitMarkSupervisor *getSupervisor()
    {
        return sSupervisor;
    }

private:
    static u16 sSupervisorCreateNum;
    static GeoRabbitMarkSupervisor *sSupervisor;
};

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

    void changeOwner(int ownerNum);/* {
        mRabbitMark->equip(ownerNum);
    }*/

    void startWinnerEffect();

private:
    u8 _0[0x3c];
    GeoRabbitMark * mRabbitMark;
    s16 mWinTime;
};

#endif