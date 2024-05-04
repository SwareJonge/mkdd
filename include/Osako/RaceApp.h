#ifndef RACEAPP_H
#define RACEAPP_H

#include "Kaneshige/RaceMgr.h"
#include "Kameda/Scene.h"

#include "Osako/GhostFile.h"
#include "Osako/GameApp.h"
#include "Osako/PadRecorder.h"
#include "Osako/ResMgr.h"
#include "Osako/SystemRecord.h"
#include "types.h"


class RaceApp : public GameApp
{
public:
    static RaceApp *create();
    static void call();

    void ctrlRace();         // 0x801fbcc0
    void ctrlRecorder();     // 0x801fbfb8
    void setAudioRaceMode(); // 0x801fc444

    static RaceApp *ptr() { return mspRaceApp; };
    PadRecorder *getPadRecorder() { return mRecorder; }

    // TODO: unlocked, or loaded?
    bool isStaffGhostLoaded() { return msStaffGhostFlag & 1 << SystemRecord::convCourseID(ResMgr::msCourseID); }

    RaceApp();
    virtual ~RaceApp();  // override
    virtual void draw(); // override
    virtual void calc(); // override

    static RaceApp *mspRaceApp;
    static volatile u16 msStaffGhostFlag;

private:
    RaceMgr *mRaceMgr;
    u32 mState;
    u32 mDrawFlags;
    u32 _18;
    u32 _1C;
    u8 _20;
    Scene::SceneType mNextScene;
    PadRecorder *mRecorder;
    KartPadRecord *mPadRecord[4];
    KartPadRecord *mGhostRecord;
    KartPadRecord *mStaffGhostRecord;
    KartGamePad *_44[3];
    u8 _50;
}; // Size: 0x54

#endif
