#ifndef RACEINFO_H
#define RACEINFO_H

#include "types.h"

#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceTime.h"

enum ERaceMode {
    INV_MODE = -1,
    TIME_ATTACK = 0x1,
    GRAND_PRIX = 0x2,
    VERSUS_RACE = 0x3,
    BALLOON_BATTLE = 0x4,
    ROBBERY_BATTLE =	0x5,
    BOMB_BATTLE =	0x6,
    ESCAPE_BATTLE =	0x7,
    AWARD_DEMO	= 0x8,
    STAFF_ROLL =	0x9,
};

enum ERaceLevel { // unsure of this
    LVL_INV = -1,
    LVL_50CC = 0,
    LVL_100CC = 1,
    LVL_150CC = 2,
    LVL_MIRROR = 3,
};

enum ERaceGpCup {
    INV_CUP = -1,
    MUSHROOM_CUP = 0,
    FLOWER_CUP = 1,
    STAR_CUP = 2,
    SPECIAL_CUP = 3,
    REVERSE2_CUP = 4,
};

class RaceInfo {
public:
    inline RaceInfo(); // this is not inlined in the debug version 
    ~RaceInfo();

    void reset();
    void setConsoleTarget(s32 idx, s16 p2, bool p3);
    void setRace(ERaceMode RaceMode, s32 kartCount, s32 playerCount, s32 consoleCount, s32 p5);
    void setRaceLevel(ERaceLevel raceLvl);

    void shuffleRandomSeed();
    void shuffleStartNo();
    void hideConsole(u32 param_2);

    s32 getKartNumber() {
        return kartNum;
    }

//private:
    bool isTinyProcess;
    bool isLanMode;
    bool isTrueEnding;
    u32 randomSeed;
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
    u16 lod;
    s16 gpStageNo;
    s32 inWaitDemo;
    bool isMirror;
    // padding bytes
    KartInfo kartInfo[8];
    s16 startPosIndex[8];
    s16 pointTable[8];
    s16 rivalKarts[2];
    
    s16 _0x114[4];
    bool _0x11c[4]; // not sure what these two arrays do, setConsoleTarget sets this so maybe this bool array means isActive and the other the console number

    s16 awardKartNo;
    // padding bytes
    s32 demoNextPhase;
    s16 rank[8]; // stores what rank you finished at previous race, basically the same as startPosIndex
    RaceTime finishTime[8];
    RaceTime lapTimes[80]; // 10 per player so perhaps it's lapTimes[8][10] or lapTimes[10][8](why do i not know how arrays work)
    s32 _0x298;
    s16 HideConsole;
    s8 _0x29e[0x2e0 - 0x29e]; // unknown
};

//RaceInfo gRaceInfo;

#endif // !RACEINFO_H
