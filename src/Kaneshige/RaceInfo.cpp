#include "Kaneshige/RaceInfo.h"

#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Dolphin/OS.h"

RaceInfo::RaceInfo() {
    reset();
}

void RaceInfo::reset() {
    isTinyProcess = false;
    isLanMode = false;
    isTrueEnding = false;
    randomSeed = 0;
    gpCup = INV_CUP;
    raceMode = INV_MODE;
    raceLevel = LVL_INV;
    itemSlotType = 0;
    vsLapNum = 0;
    lapNumLAN = 0;
    kartNum = 0;
    playerNum = 0;
    consoleNum = 0;
    statusNum = 0;
    lod = 0;
    gpStageNo = 0;
    inWaitDemo = 0;
    isMirror = false;
    _0x298 = 0;
    HideConsole = 0;

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

void RaceInfo::setConsoleTarget(int idx, short cnsNo, bool p3) {
    bool valid = false;
    if (-1 < cnsNo && cnsNo < 4)
        valid = true;
    if(!valid) {
        JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 453, "range over: %d <= cnsNo=%d < %d", 0, cnsNo, 4);
        OSPanic(__FILE__, 453, "Halt");
    }

    _0x114[idx] = cnsNo;
    _0x11c[idx] = p3;
}

/*void RaceInfo::setRace(ERaceMode RaceMode, s32 kartCount, s32 playerCount, s32 consoleCount, s32 p5) {
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
    this->raceLevel = raceLvl;
    if (raceLvl == LVL_MIRROR)
        isMirror = true;
    else
        isMirror = false;
}

void RaceInfo::shuffleRandomSeed() {
    JMath::TRandom_<JMath::TRandom_fast_> rndm(randomSeed);
    randomSeed = ((JMath::TRandom_fast_)rndm).get();
}

void RaceInfo::shuffleStartNo() {
    JMath::TRandom_<JMath::TRandom_fast_> rndm(randomSeed);

    for (u32 i = 0; i < (u32)getKartNumber(); i++) {
        u32 newidx = i + (((JMath::TRandom_fast_)rndm).get() % (getKartNumber() - i));

        if (newidx >= getKartNumber()) {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 751, "range over: %d <= dst=%d < %d", 0, newidx, getKartNumber());
            OSPanic(__FILE__, 751, "Halt");
        }

        s32 playerStartIdx = startPosIndex[i];
        startPosIndex[i] = startPosIndex[newidx];
        startPosIndex[newidx] = playerStartIdx;
    }
}

void RaceInfo::hideConsole(u32 param_2) {
    HideConsole = HideConsole | (u16)(1 << param_2);
}*/
