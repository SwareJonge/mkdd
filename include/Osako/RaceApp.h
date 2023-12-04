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
    static PadRecorder *mspRecorder;

private:
    RaceMgr *mRaceMgr;
    u32 mState;
    u32 mDrawFlags;
    u8 _18[0x28 - 0x18];
    PadRecorder *mPadRecorder;
    KartPadRecord *mPadRecord[4];
    KartPadRecord *mGhostRecord;
}; // Size: 0x54

#endif