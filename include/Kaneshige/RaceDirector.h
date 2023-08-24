#ifndef RACEDIRECTOR_H
#define RACEDIRECTOR_H

#include "Kaneshige/DemoTimeKeeper.h"
#include "Kaneshige/RaceTime.h"
#include "kartEnums.h"

class RaceDirector
{
public:
    // Global
    RaceDirector(bool, DemoTimeKeeper *, int); // 0x801c752c
    void calc();
    bool checkRaceEnd();
    bool isFrameRenewal() const {
#ifdef DEBUG
        bool ret = false;
        if (mFrameRenewal && !sForceStopFrame) {
            ret = true;
        }
        return ret;
#else
        return mFrameRenewal;
#endif
    }
    ERacePhase getRacePhase() const { return mRacePhase; };
    bool isRaceEnd() const { return mIsRaceEnd; };
    int getWaitDemoResult() const { return mWaitDemoResult; }
    const RaceTime & getMiniGameTime() { return mMinigameTime; }
    void reset();

    static u8 sForceStopFrame;
private:
    u8 _00[0xc];
    ERacePhase mRacePhase;
    u8 _10[0x1c - 0x10];
    int mWaitDemoResult;
    u8 _20[0x28 - 0x20];
    bool mFrameRenewal;
    u8 _29;
    bool mIsRaceEnd;
    u8 _2B[0x44 - 0x2B];
    RaceTime mMinigameTime;
}; // Size 0x48

#endif