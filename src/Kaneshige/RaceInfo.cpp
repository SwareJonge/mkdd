#include "RaceInfo.h"

void RaceInfo::reset() {
    isTinyProcess = false;
    isLanMode = false;
    isTrueEnding = false;
    randomSeed = 0;
    gpCup = -1;
    raceMode = 0;
    raceLevel = -1;
    itemSlotType = 0;
    vsLapNum = 0;
    lapNumLAN = 0;
    kartNum = 0;
    playerNum = 0;
    consoleNum = 0;
    statusNum = 0;
    lod = 0;
    gpStageNo = 0;
    _0x28 = 0;
    isMirror = false;
    _0x298 = 0;
    hideConsole = 0;

    for (s32 i = 0; i < 8; i++) {
        kartInfo[i].reset();
        rank[i] = 0;
    }

    for (s32 i = 0; i < 8; i++) {
        startPosIndex[i] = i;
        pointTable[i] = 0;
    }

    for (s32 i = 0; i < 2; i++) {
        rivalKarts[i] = -1;
    }

    for (s32 i = 0; i < 4; i++) {
        _0x114[i] = i;
        _0x11c[i] = 0;
    }

    awardKartNo = -1;
    demoNextPhase = 6;
}

void RaceInfo::setConsoleTarget(s32 idx, s16 p2, bool p3) {
    _0x114[idx] = p2;
    _0x11c[idx] = p3;
}

void RaceInfo::setRace(ERaceMode RaceMode, s32 kartCount, s32 playerCount, s32 consoleCount, s32 p5) {
    reset();
    raceMode = RaceMode;
    kartNum = (s16)kartCount;
    playerNum = playerCount;
    consoleNum = (s16)consoleCount;
    statusNum = (s16)p5;
    lod = 0;

    if (consoleNum >= 3)
        lod |= 2;

    if ((consoleCount >= 3) && (kartCount >= 5))
        isTinyProcess = true;
}

void RaceInfo::setRaceLevel(ERaceLevel raceLvl) {
    raceLevel = raceLvl;
    if (raceLvl == LVL_MIRROR)
        isMirror = true;
    else
        isMirror = false;
}

void RaceInfo::shuffleRandomSeed() {
    // maybe randomSeed is TRandom_fast_? uses a template function in the debug version, probably inlined here
    JMath::TRandom_fast_ rndm(randomSeed);
    randomSeed = rndm.get();
}

void RaceInfo::shuffleStartNo() {
    JMath::TRandom_fast_ rndm(randomSeed);
    u32 seed;

    for (u32 i = 0; i < (u32)getKartNumber(); i++) {
        s32 kartIdx = getKartNumber();
        u32 seed = rndm.get();

        u32 newidx = i + (seed % (kartIdx - i));
        s32 playerStartIdx = startPosIndex[i];
        startPosIndex[i] = startPosIndex[newidx];
        startPosIndex[newidx] = playerStartIdx;
    }
}

void RaceInfo::hideConsole(u32 param_2) {
    hideConsole = hideConsole | (u16)(1 << param_2);
}
