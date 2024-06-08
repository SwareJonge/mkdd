#ifndef RIVALCTRLBASE_H
#define RIVALCTRLBASE_H

#include "types.h"

class RivalKart;

class RivalCtrlBase
{
public:
    class RankParam
    {
    public:
        RankParam(int);
        void reset();
    };

    RivalCtrlBase(RivalKart *);
    void getMyID();
    void getBodyCtrl();
    void getBody();
    void getRandom();
    void getRankParamID();
    void tstEndRace();
    bool isRival() const;
    void getMyRank();
    // Inline/Unused
    // void getSpeedCtrl();
    // void getItemCtrl();

    virtual ~RivalCtrlBase() {} // 0x8027ea9c

    RivalKart *mRival;
};

#endif