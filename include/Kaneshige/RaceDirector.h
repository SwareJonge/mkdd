#ifndef RACEDIRECTOR_H
#define RACEDIRECTOR_H

#include "Kaneshige/DemoTimeKeeper.h"
#include "kartEnums.h"

class RaceDirector
{
public:
    // Global
    RaceDirector(bool, DemoTimeKeeper *, int); // 0x801c752c
    bool checkRaceEnd();
    bool isFrameRenewal() const;
    ERacePhase getRacePhase() const /* { return mRacePhase}*/;
    void reset();
};

#endif