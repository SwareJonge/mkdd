#include "Kameda/SequenceInfo.h"
#include "Kaneshige/RaceInfo.h"

#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Dolphin/OS.h"

// i assume this data is from some include related to audio
// also thanks seeky
static const float lbl_80378500[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(lbl_80378500)
#pragma pop

u16 RaceInfo::sWaitDemoSelector;
ERaceGpCup RaceInfo::sAwardDebugCup;

int RaceInfo::sForceDemoNo = 0xFFFFFFFF;
u32 RaceInfo::sForceRandomSeed = 0xFFFFFFFF;
ERaceLevel RaceInfo::sAwardDebugLevel = LVL_150CC;
short RaceInfo::sAwardDebugRank = 1;

EKartID RaceInfo::sAwardDebugKartIDTable[] = {
    PARADE_KART,
    KOOPA_KING,
    HEART_COACH};
ECharID RaceInfo::sAwardDebugDriver1IDTable[] = {
    MARIO,
    BOWSER,
    PEACH};
ECharID RaceInfo::sAwardDebugDriver2IDTable[] = {
    LUIGI,
    BOWSERJR,
    DAISY};

RaceInfo::RaceInfo()
{
    reset();
}

void RaceInfo::reset()
{
    isTinyProcess = false;
    mLanMode = false;
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

    for (s32 i = 0; i < 8; i++)
    {
        kartInfo[i].reset();
        rank[i] = 0;
    }

    for (s32 i = 0; i < 8; i++)
    {
        startPosIndex[i] = i;
        pointTable[i] = 0;
    }

    for (s32 i = 0; i < 2; i++)
    {
        rivalKarts[i] = -1;
    }

    for (s32 i = 0; i < 4; i++)
    {
        _0x114[i] = i;
        _0x11c[i] = 0;
    }

    awardKartNo = -1;
    demoNextPhase = 6;
}

void RaceInfo::setConsoleTarget(int cnsNo, int target, bool p3)
{
    bool valid = false;
    if (0 <= cnsNo && cnsNo < 4)
        valid = true;
    if (!valid)
    {
        JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 453, "range over: %d <= cnsNo=%d < %d", 0, cnsNo, 4);
        OSPanic(__FILE__, 453, "Halt");
    }

    _0x114[cnsNo] = target;
    _0x11c[cnsNo] = p3;
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
        setAwardKartNo(sAwardDebugRank - 1);
    else
        setAwardKartNo(-1);

    for (int idx = 0; idx < getKartNumber(); idx++)
    {
        bool valid = false;
        if (idx >= 0 && idx < 3)
            valid = true;

        if (!valid)
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 585, "range over: %d <= idx=%d < %d", 0, idx, 3);
            OSPanic(__FILE__, 585, "Halt");
        }
        setKart(idx, sAwardDebugKartIDTable[idx], sAwardDebugDriver1IDTable[idx], 0, sAwardDebugDriver2IDTable[idx], 0);
    }
}

void RaceInfo::settingForStaffRoll(bool trueEnding)
{
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

void RaceInfo::setRace(ERaceMode RaceMode, int kartCount, int playerCount, int consoleCount, int p5)
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

    for (int idx = 0; 2 > idx; ++idx) // the most useful loop ever
    {
        if (!idx != false)
            kartInfo[kartNo].setDriver(idx, charID1, kartPad1);
        else
            kartInfo[kartNo].setDriver(idx, charID2, kartPad2);
    }
}

void RaceInfo::setRaceLevel(ERaceLevel raceLvl)
{
    this->raceLevel = raceLvl;
    if (raceLvl == LVL_MIRROR)
        isMirror = true;
    else
        isMirror = false;
}

void RaceInfo::shuffleRandomSeed()
{
    JMath::TRandom_<JMath::TRandom_fast_> rndm(randomSeed);
    randomSeed = ((JMath::TRandom_fast_)rndm).get();
}

void RaceInfo::shuffleStartNo()
{
    JMath::TRandom_<JMath::TRandom_fast_> rndm(randomSeed);

    for (u32 i = 0; i < (u32)getKartNumber(); i++)
    {
        u32 newidx = i + (((JMath::TRandom_fast_)rndm).get() % (getKartNumber() - i));

        if (newidx >= getKartNumber())
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 751, "range over: %d <= dst=%d < %d", 0, newidx, getKartNumber());
            OSPanic(__FILE__, 751, "Halt");
        }

        s32 playerStartIdx = startPosIndex[i];
        startPosIndex[i] = startPosIndex[newidx];
        startPosIndex[newidx] = playerStartIdx;
    }
}

void RaceInfo::hideConsole(u32 viewNo)
{
    bool valid = false;
    if (viewNo >= 1 && (viewNo < 5))
        valid = true;
    if (!valid)
    {
        JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 772, "range over: %d <= viewNo=%d < %d", 1, viewNo, 5);
        OSPanic(__FILE__, 772, "Halt");
    }

    HideConsole = HideConsole | (1 << viewNo);
}
