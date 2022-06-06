#include "RaceInfo.h"

void RaceInfo::reset() {
    isTinyProcess = false;
    isLanMode = false;
    isTrueEnding = false;
    random.setSeed(0);
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
