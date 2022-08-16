#ifndef RACEMGR_H
#define RACEMGR_H

#include "types.h"

#include "JSystem/JUtility/JUTAssert.h"

#include "JSystem/JKernel/JKRHeap.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/RaceDirector.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Kaneshige/KartChecker.h"
#include "Kaneshige/KartLoader.h"

class RaceMgr : JKRDisposer
{
public:
    RaceMgr(RaceInfo *);
    virtual ~RaceMgr();

    static RaceMgr *sRaceManager;
    static short sForceTotalLapNum;
    static short sDispFrameCounter;

    static short sMyStartPointID;

    class Console
    {
    public:
        Console();
        void changeTargetNo(int, bool);

    private:
        int _0x0;
        u8 _0x4;
        int _0x8;
        ushort _0xc;
    };

    bool isLANMode() const
    {
        return raceInfo->isLANMode();
    }

    static RaceMgr * getManager(); /*{
        return sRaceManager;
    }*/

    bool checkRaceEnd()
    {
        return mRaceDirector->checkRaceEnd();
    }

    Course *getCourse() const; /*
       return course;
   */

    KartInfo * getKartInfo(int index);

    KartLoader * getKartLoader(int index) const {
        JUT_RANGE_ASSERT(257, 0, index, 8);
        return kartLoader[index];
    }

    KartChecker *getKartChecker(int index) const
    {
        JUT_RANGE_ASSERT(267, 0, index, 8);
        return kartChecker[index];
    }

    ERaceMode getRaceMode() const; /*{
        return raceInfo->getRaceMode();
    }*/

    const RaceTime &getBestLapTime()
    {
        return mBestLapTime;
    }

    const RaceTime &getBestTotalTime(int recID)
    {
        JUT_RANGE_ASSERT(328, 0, recID, 5);
        return mBestTotalTimes[recID];
    }

private:
    RaceDirector *mRaceDirector;
    void *raceDrawer;
    u16 areaLight;
    u8 _0x22;
    u8 _0x23; // probably padding
    u32 frame;
    JKRHeap *raceHeap;
    s16 replayMode;
    s16 totalLapNumber;
    s32 _0x30;
    bool _0x31;
    RaceInfo *raceInfo;
    void *raceBGMPlayer;
    Console *console;
    Course *course;
    KartChecker *kartChecker[8];
    KartLoader *kartLoader[8];
    void * mAward2D;
    void *staffRoll2D;
    void * mResMgr;
    RaceTime mBestLapTime;
    RaceTime mBestTotalTimes[5];
    s16 events;

    // these values store the amount of time/frames it took to execute a certain set of functions
    // 0x0 GX Issue
    // 0x100 Calc
    // 0x200 update
    // 0x300 ViewCalc
    s16 proctime1;
    s16 proctime2;
    s16 proctime3;
    s16 proctime4;
};

Course *RCMGetCourse(); /* { // might be inline off, auto?(Kameda)
    return RaceMgr::getManager()->getCourse();
}*/

#endif // RACEMGR_H
