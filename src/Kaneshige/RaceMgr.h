#ifndef RACEMGR_H
#define RACEMGR_H

#include <types.h>
#include <JSystem/JKernel/JKRDisposer.h>
#include <JSystem/JKernel/JKRHeap.h>

#include "RaceInfo.h"
#include "RaceTime.h"
#include "KartChecker.h"

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
        void changeTargetNo(s32, bool);
    private:
        s32 _0x0;
        u8 _0x4;
        s32 _0x8;
        u16 _0xc;
    };

private:
    RaceDirector * raceDirector;
    RaceDrawer * raceDrawer;
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
    RaceBGMPlayer * raceBGMPlayer;
    Console * console;
    Course * course;
    KartChecker * kartChecker[8];
    KartLoader * kartLoader[8];
    StaffRoll2D * staffRoll2D;
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

#endif // RACEMGR_H
