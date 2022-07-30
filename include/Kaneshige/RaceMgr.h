#ifndef RACEMGR_H
#define RACEMGR_H

#include "types.h"

#include "JSystem/JKernel/JKRHeap.h"

#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceTime.h"
#include "Kaneshige/KartChecker.h"

#include "Kaneshige/Course/Course.h"

class RaceMgr : JKRDisposer {
public:
    RaceMgr(RaceInfo *);
    virtual ~RaceMgr();

    static RaceMgr * sRaceManager;
    static s16 sForceTotalLapNum;
    static s16 sDispFrameCounter;

    static s16 sMyStartPointID;

    class Console {
    public:
        Console();
        void changeTargetNo(int, bool);
    private:
        s32 _0x0;
        u8 _0x4;
        s32 _0x8;
        u16 _0xc;
    };

    static RaceMgr *getManager(); /*{
        return sRaceManager;
    }*/

    Course * getCourse() const; /*
        return course;
    */

    ERaceMode getRaceMode() const; /*{
        return raceInfo->getRaceMode();
    }*/

private:
    void * raceDirector;
    void * raceDrawer;
    u16 areaLight;
    u8 _0x22;
    u8 _0x23; // probably padding
    u32 frame;
    JKRHeap * raceHeap;
    s16 replayMode;
    s16 totalLapNumber;
    s32 _0x30;
    bool _0x31;
    RaceInfo * raceInfo;
    void * raceBGMPlayer;
    Console * console;
    Course * course;
    KartChecker * kartChecker[8];
    void * kartLoader[8];
    void * staffRoll2D;
    RaceTime bestLapTime;
    RaceTime bestTotalTime[5];
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

Course * RCMGetCourse() { // might be inline off, auto?(Kameda)
    return RaceMgr::getManager()->getCourse();
}

#endif // RACEMGR_H
