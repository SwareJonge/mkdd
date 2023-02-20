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
        // retval might be used
        if (!mIsRaceEnd)
            return false;
        if(sForceStopFrame)
            return false;
        return true;
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
    u8 _10[0x2a - 0x10];
    bool mIsRaceEnd;

};

#endif