#ifndef RACEINFO_H
#define RACEINFO_H

#include <types.h>
#include <JSystem/JMath/JMath.h>

#include "KartInfo.h"
#include "RaceTime.h"

class RaceInfo {
public:
    ~RaceInfo();

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
    s16 playerNum;
    s16 consoleNum;
    s16 statusNum;
    s16 lod;
    s16 gpStageNo;
    s32 _0x28;
    bool isMirror;
    // padding bytes
    // this might be a structure 
    KartInfo kartInfo[8];
    u8 _0xf0[0x120 - 0x30];
    // padding bytes
    s16 awardKartNo;
    // padding bytes
    s32 demoNextPhase;
    u8 _0x128[0x138 - 0x128];
    RaceTime finishTime[8];
    RaceTime lapTimes[80]; // 10 per player so perhaps it's lapTimes[8][10] or lapTimes[10][8](why do i not know how arrays work)
    s32 _0x298;
    s16 hideConsole;
    s8 _0x29e[0x2e0 - 0x29e]; // unknown
};

RaceInfo gRaceInfo;

#endif // !RACEINFO_H
