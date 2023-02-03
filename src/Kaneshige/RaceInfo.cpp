#include "dolphin/OS.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTDbg.h"

#include "Kameda/SequenceInfo.h"
#include "Kaneshige/RaceInfo.h"

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
    mTinyProcess = false;
    mLanMode = false;
    mTrueEnding = false;
    mRandomSeed = 0;
    mGpCup = INV_CUP;
    mRaceMode = INV_MODE;
    mRaceLevel = LVL_INV;
    mItemSlotType = 0;
    mVsLapNum = 0;
    mLapNumLAN = 0;
    mKartNum = 0;
    mPlayerNum = 0;
    mConsoleNum = 0;
    mStatusNum = 0;
    mLOD = 0;
    mGpStageNo = 0;
    mDemoType = 0;
    mMirror = false;
    _0x298 = 0;
    mHideConsole = 0;

    for (s32 i = 0; i < 8; i++)
    {
        mKartInfo[i].reset();
        mRank[i] = 0;
    }

    for (s32 i = 0; i < 8; i++)
    {
        mStartPosIndex[i] = i;
        mPointTable[i] = 0;
    }

    for (s32 i = 0; i < 2; i++)
    {
        mRivalKarts[i] = -1;
    }

    for (s32 i = 0; i < 4; i++)
    {
        _0x114[i] = i;
        _0x11c[i] = 0;
    }

    mAwardKartNo = -1;
    mDemoNextPhase = 6;
}

void RaceInfo::setConsoleTarget(int cnsNo, int target, bool p3)
{
    JUT_MINMAX_ASSERT(453, 0, cnsNo, 4);
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
        gSequenceInfo.rndDemo(mRandomSeed);
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
        mDemoType = 2;
    }
    else
    {
        mDemoType = 1;
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
        JUT_MINMAX_ASSERT(585, 0, idx, 3);
        setKart(idx, sAwardDebugKartIDTable[idx], sAwardDebugDriver1IDTable[idx], 0, sAwardDebugDriver2IDTable[idx], 0);
    }
}

void RaceInfo::settingForStaffRoll(bool trueEnding)
{
    setRace(STAFF_ROLL, 8, 0, 1, 1);
    mTrueEnding = trueEnding;
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
    mRaceMode = RaceMode;
    mKartNum = (s16)kartCount;
    mPlayerNum = playerCount;
    mConsoleNum = (s16)consoleCount;
    mStatusNum = (s16)p5;
    mLOD = 0;

    if (mConsoleNum >= 3)
        mLOD |= 2;

    if ((consoleCount >= 3) && (kartCount >= 5))
        mTinyProcess = true;
}

void RaceInfo::setKart(int kartNo, EKartID kartID, ECharID charID1, KartGamePad *kartPad1, ECharID charID2, KartGamePad *kartPad2)
{
    JUT_MINMAX_ASSERT(685, 0, kartNo, 8);
    mKartInfo[kartNo].setKartID(kartID);
    JUT_ASSERT(694, charID1 != cCharIDNone);
    JUT_ASSERT(695, charID2 != cCharIDNone);

    for (int idx = 0; 2 > idx; ++idx) // the most useful loop ever
    {
        if (!idx != false)
            mKartInfo[kartNo].setDriver(idx, charID1, kartPad1);
        else
            mKartInfo[kartNo].setDriver(idx, charID2, kartPad2);
    }
}

void RaceInfo::setRaceLevel(ERaceLevel raceLvl)
{
    mRaceLevel = raceLvl;
    if (raceLvl == LVL_MIRROR)
        mMirror = true;
    else
        mMirror = false;
}

void RaceInfo::shuffleRandomSeed()
{
    JMath::TRandom_<JMath::TRandom_fast_> rndm(mRandomSeed); 
    mRandomSeed = rndm.get();
    // randomSeed = JMath::TRandom_<JMath::TRandom_fast_>(randomSeed).get();
}

void RaceInfo::shuffleStartNo()
{
    JMath::TRandom_<JMath::TRandom_fast_> rndm(mRandomSeed);

    for (u32 i = 0; i < (u32)getKartNumber(); i++)
    {
        u32 dst = i + (rndm.get() % (getKartNumber() - i));
        JUT_MAX_ASSERT(751, dst, getKartNumber());
        s32 playerStartIdx = mStartPosIndex[i];
        mStartPosIndex[i] = mStartPosIndex[dst];
        mStartPosIndex[dst] = playerStartIdx;
    }
}

void RaceInfo::hideConsole(u32 viewNo)
{
    JUT_MINMAX_ASSERT(772, 1, viewNo, 5);
    mHideConsole = mHideConsole | (1 << viewNo);
}
