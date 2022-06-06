#ifndef RACEINFO_H
#define RACEINFO_H

#include <types.h>
#include <JSystem/JMath/JMath.h>

#include "KartInfo.h"
#include "RaceTime.h"

class RaceInfo {
public:
    inline RaceInfo(); // this is not inlined in the debug version 
    ~RaceInfo();

    void reset();

private:
    bool isTinyProcess;
    bool isLanMode;
    bool isTrueEnding;
    JMath::TRandom_fast_ random;
    ERaceMode raceMode;
    ERaceGpCup gpCup;
    ERaceLevel raceLevel;
    s32 itemSlotType; // perhaps this is an enum too
    s16 vsLapNum;
    s16 lapNumLAN;
    s16 kartNum;
    s16 playerNum;
    s16 consoleNum;
    s16 statusNum;
    s16 lod;
    s16 gpStageNo;
    s32 _0x28; 
    bool isMirror;
    // padding bytes
    KartInfo kartInfo[8];
    s16 startPosIndex[8];
    s16 pointTable[8];
    s16 rivalKarts[2];
    s16 _0x114[4];
    u8 _0x11c[4]; // not sure what these two arrays do
    s16 awardKartNo;
    // padding bytes
    s32 demoNextPhase;
    s16 rank[8]; // stores what rank you finished at previous race, basically the same as startPosIndex
    RaceTime finishTime[8];
    RaceTime lapTimes[80]; // 10 per player so perhaps it's lapTimes[8][10] or lapTimes[10][8](why do i not know how arrays work)
    s32 _0x298;
    s16 hideConsole;
    s8 _0x29e[0x2e0 - 0x29e]; // unknown
};

RaceInfo gRaceInfo;

#endif // !RACEINFO_H
