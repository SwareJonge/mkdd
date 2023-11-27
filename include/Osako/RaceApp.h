#ifndef RACEAPP_H
#define RACEAPP_H

#include "Osako/GameApp.h"
#include "Osako/PadRecorder.h"
#include "types.h"

#include "Kaneshige/RaceMgr.h"

class RaceApp : public GameApp
{
public:
    static RaceApp *create();
    static void call();
    static RaceApp *ptr() { return mspRaceApp; };
    PadRecorder *getPadRecorder() { return mPadRecorder; }

    RaceApp();
    virtual ~RaceApp();  // override
    virtual void draw(); // override
    virtual void calc(); // override

    static RaceApp *mspRaceApp;

private:
    RaceMgr *mRaceMgr;
    u8 _10[0x28 - 0x10];
    PadRecorder *mPadRecorder;
}; // Size: 0x54

#endif