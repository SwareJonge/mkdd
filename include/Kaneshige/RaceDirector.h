#ifndef RACEDIRECTOR_H
#define RACEDIRECTOR_H

#include "Kaneshige/DemoTimeKeeper.h"
#include "kartEnums.h"

class RaceDirector
{
public:
    // Global
    RaceDirector(bool, DemoTimeKeeper *, int); // 0x801c752c
    void calc();
    bool checkRaceEnd();
    bool isFrameRenewal() const {
        bool ret = false;
        if (mFrameRenewal && !sForceStopFrame)
            ret = true;
        return ret;
    }
    ERacePhase getRacePhase() const { return mRacePhase; };
    bool isRaceEnd() const { return mIsRaceEnd; };
    void getWaitDemoResult() const;
    void getMiniGameTime();
    void reset();

    static u8 sForceStopFrame;
private:
    u8 _00[0xc];
    ERacePhase mRacePhase;
    u8 _10[0x28 - 0x10];
    bool mFrameRenewal;
    u8 _29;
    bool mIsRaceEnd;

};

#endif