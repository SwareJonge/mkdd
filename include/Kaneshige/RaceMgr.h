#ifndef RACEMGR_H
#define RACEMGR_H

#include "types.h"

#include "JSystem/JKernel/JKRHeap.h"

#include "Kaneshige/RaceDirector.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Kaneshige/KartChecker.h"
#include "Kaneshige/KartLoader.h"

#include "Kaneshige/Course/Course.h"

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

    KartLoader * getKartLoader(int index) {
        bool valid = false;
        if(index >= 0 && index < 8)
            valid = true;

        if(!valid) {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 257, "range over: %d <= index=%d < %d", 0, index, 8);
            OSPanic(__FILE__, 257, "Halt");
        }
        return kartLoader[index];
    }

    KartChecker *getKartChecker(int index) const
    {
        bool valid = false;
        if (index >= 0 && index < 8)
            valid = true;
        if (!valid)
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 267, "range over: %d <= index=%d < %d", 0, index, 8);
            OSPanic(__FILE__, 267, "Halt");
        }
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
        bool valid = false;
        if (recID >= 0 && recID < 5)
            valid = true;

        if (!valid)
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 328, "range over: %d <= recID=%d < %d", 0, recID, 5);
            OSPanic(__FILE__, 328, "Halt");
        }
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
    void *staffRoll2D;
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
