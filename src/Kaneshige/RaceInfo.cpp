#include "Kameda/SequenceInfo.h"
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
    demoType = 0;
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

void RaceInfo::settingForWaitDemo(bool settingThing)
{
    bool iVar1 = true;
    s32 uVar7 = sWaitDemoSelector % 3;

    if (sWaitDemoSelector & 0x1)
    {
        iVar1 = false;
    }

    if (sForceDemoNo != 0xffffffff)
    {
        uVar7 = (u32)sForceDemoNo % 3;
    }
    sWaitDemoSelector++;

    switch (uVar7)
    {
    case 0:
        setRace(GRAND_PRIX, 8, 0, 1, 1);
        setRivalKartNo(0, 0);
        setRivalKartNo(1, 1);
        break;
    case 1:
        setRace(GRAND_PRIX, 8, 0, 2, 2);
        setRivalKartNo(0, 0);
        setRivalKartNo(1, 1);
        break;
    case 2:
        setRace(VERSUS_RACE, 4, 0, 4, 4);
        break;
    default:
        setRace(GRAND_PRIX, 8, 0, 1, 1);
        setRivalKartNo(0, 0);
        setRivalKartNo(1, 1);
        break;
    }

    setRandomSeed(OSGetTime());
    if (sForceRandomSeed != 0xffffffff)
    {
        setRandomSeed(sForceRandomSeed);
    }
    setRaceLevel(LVL_150CC);
    if (!iVar1)
    {
        gSequenceInfo.rndDemo(randomSeed);
    }

    for (int no = 0; no < getKartNumber(); no++)
    {
        EKartID demoKartIDs[] = { // WHY
            RED_FIRE,
            HEART_COACH,
            DK_JUMBO,
            KOOPA_KING,
            TURBO_YOSHI,
            KOOPA_DASHER,
            GOO_GOO_BUGGY,
            WARIO_CAR
        };
        EKartID kartID = demoKartIDs[no];
        ECharID driver = KartInfo::getDefaultDriver(kartID);
        ECharID partner = KartInfo::getDefaultPartner(driver);
        if (!iVar1)
        {
            gSequenceInfo.getDemoKart(no, driver, partner, kartID);
        }
        setKart(no, kartID, driver, 0, partner, 0);
    }

    if (settingThing)
    {
        demoType = 2;
    }
    else
    {
        demoType = 1;
    }
}

void RaceInfo::settingForAwardDemo()
{
    setRace(AWARD_DEMO, 3, 0, 1, 1);
    setGpCup(sAwardDebugCup);
    setRaceLevel(sAwardDebugLevel);

    if (sAwardDebugRank < 4)
        setAwardKartNo(sAwardDebugRank  -1);
    else
        setAwardKartNo(-1);

    for (int idx = 0; idx < getKartNumber(); idx++) {
        bool valid = false;
        if(idx >= 0 && idx < 3)
            valid = true;
        
        if(!valid) {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 585, "range over: %d <= idx=%d < %d", 0, idx, 3);
            OSPanic(__FILE__, 585, "Halt");
        }
        setKart(idx, sAwardDebugKartIDTable[idx], sAwardDebugDriver1IDTable[idx], 0, sAwardDebugDriver2IDTable[idx], 0);
    }
}

void RaceInfo::settingForStaffRoll(bool trueEnding) {
    setRace(STAFF_ROLL, 8, 0, 1, 1);
    isTrueEnding = trueEnding;
    setRivalKartNo(0, 3);
    setRivalKartNo(1, 7);
    setRandomSeed(0);
    setRaceLevel(LVL_150CC);

    for (int no = 0; no < getKartNumber(); no++)
    {
        EKartID demoKartIDs[] = {// WHY
                                 RED_FIRE,
                                 HEART_COACH,
                                 DK_JUMBO,
                                 KOOPA_KING,
                                 TURBO_YOSHI,
                                 KOOPA_DASHER,
                                 GOO_GOO_BUGGY,
                                 WARIO_CAR};
        EKartID kartID = demoKartIDs[no];
        ECharID driver = KartInfo::getDefaultDriver(kartID);
        ECharID partner = KartInfo::getDefaultPartner(driver);
        setKart(no, kartID, driver, 0, partner, 0);
    }
}



void RaceInfo::setRace(ERaceMode RaceMode, s32 kartCount, s32 playerCount, s32 consoleCount, s32 p5)
{
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

void RaceInfo::setKart(int kartNo, EKartID kartID, ECharID charID1, KartGamePad *kartPad1, ECharID charID2, KartGamePad *kartPad2)
{
    bool valid = false;
    if (kartNo >= 0 && kartNo < 8)
    {
        valid = true;
    }

    if (!valid)
    {
        JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 685, "range over: %d <= kartNo=%d < %d", 0, kartNo, 8);
        OSPanic(__FILE__, 685, "Halt");
    }
    kartInfo[kartNo].setKartID(kartID);

    if (charID1 == NONE)
    {
        JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, 694, "charID1 != cCharIDNone");
        OSPanic(__FILE__, 694, "Halt");
    }
    if (charID2 == NONE)
    {
        JUTAssertion::showAssert(JUTAssertion::getSDevice(), __FILE__, 695, "charID2 != cCharIDNone");
        OSPanic(__FILE__, 695, "Halt");
    }
    for (int idx = 0; idx < 2; idx++) // the most useful loop ever
    {
        if (idx == 0)
            kartInfo[kartNo].setDriver(idx, charID1, kartPad1);
        else
            kartInfo[kartNo].setDriver(idx, charID2, kartPad2);
    }
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
}
