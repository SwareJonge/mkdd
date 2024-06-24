#include <dolphin/os.h>
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTDbg.h"

#include "Kameda/SequenceInfo.h"
#include "Kaneshige/RaceInfo.h"

// i assume this data is from some include related to audio (edit: unlikely since release doesn't seem to have it as often)
#ifdef DEBUG
#include "mathHelper.h"
#endif

RaceInfo gRaceInfo;

#include "JSystem/JAudio/JASFakeMatch2.h"

u16 RaceInfo::sWaitDemoSelector;
ERaceGpCup RaceInfo::sAwardDebugCup;

u32 RaceInfo::sForceDemoNo = 0xFFFFFFFF;
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
    mIsTinyProcess = false;
    mIsLanMode = false;
    mIsTrueEnding = false;
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
    mIsMirror = false;
    mWaitDemoResult = 0;
    mHidingConsoles = 0;

    for (int i = 0; i < 8; i++)
    {
        mKartInfo[i].reset();
        mRank[i] = 0;
    }

    for (int i = 0; i < 8; i++)
    {
        mStartPosIndex[i] = i;
        mPointTable[i] = 0;
    }

    for (int i = 0; i < 2; i++)
    {
        mRivalKarts[i] = -1;
    }

    for (int i = 0; i < 4; i++)
    {
        mTargetKarts[i] = i;
        mIsDemoKart[i] = 0;
    }

    mAwardKartNo = -1;
    mDemoNextPhase = 6;
}

void RaceInfo::setConsoleTarget(int cnsNo, int target, bool demo)
{
#line 453
    JUT_MINMAX_ASSERT(0, cnsNo, 4);
    mTargetKarts[cnsNo] = target;
    mIsDemoKart[cnsNo] = demo;
}

void RaceInfo::settingForWaitDemo(bool settingThing)
{
    bool doDemo = true;
    int demoNo = sWaitDemoSelector % 3;

    if (sWaitDemoSelector & 0x1)
    {
        doDemo = false;
    }
#if DEBUG
    if (sForceDemoNo != 0xffffffff)
    {
        demoNo = sForceDemoNo % 3;
    }
#endif
    sWaitDemoSelector++;

    switch (demoNo)
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
#if DEBUG
    if (sForceRandomSeed != 0xffffffff)
    {
        setRandomSeed(sForceRandomSeed);
    }
#endif
    setRaceLevel(LVL_150CC);
    if (!doDemo)
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
        if (!doDemo)
        {
            gSequenceInfo.getDemoKart(no, driver, partner, kartID);
        }
        setKart(no, kartID, driver, 0, partner, 0);
    }

    // this doesn't seem to have any effect on anything, probably used for testing
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
#line 585
        JUT_MINMAX_ASSERT(0, idx, 3);
        setKart(idx, sAwardDebugKartIDTable[idx], sAwardDebugDriver1IDTable[idx], 0, sAwardDebugDriver2IDTable[idx], 0);
    }
}

void RaceInfo::settingForStaffRoll(bool trueEnding)
{
    setRace(STAFF_ROLL, 8, 0, 1, 1);
    mIsTrueEnding = trueEnding;
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

void RaceInfo::setRace(ERaceMode raceMode, int kartCount, int playerCount, int consoleCount, int statusCount)
{
    reset();
    mRaceMode = raceMode;
    mKartNum = (s16)kartCount;
    mPlayerNum = playerCount;
    mConsoleNum = (s16)consoleCount;
    mStatusNum = (s16)statusCount;
    mLOD = 0;

    if (mConsoleNum >= 3)
        mLOD |= 2;

    if ((consoleCount >= 3) && (kartCount >= 5))
        mIsTinyProcess = true;
}

void RaceInfo::setKart(int kartNo, EKartID kartID, ECharID charID1, KartGamePad *kartPad1, ECharID charID2, KartGamePad *kartPad2)
{
#line 685
    JUT_MINMAX_ASSERT( 0, kartNo, 8);
    KartInfo *kartInfo = &mKartInfo[kartNo];
    kartInfo->setKartID(kartID);
#line 694
    JUT_ASSERT(charID1 != cCharIDNone);
    JUT_ASSERT(charID2 != cCharIDNone);

    // this meme must stay alive
    for (int idx = 0; 2 > idx; ++idx) // the most useful loop ever
    {
        if (!idx != false)
            kartInfo->setDriver(idx, charID1, kartPad1);
        else
            kartInfo->setDriver(idx, charID2, kartPad2);
    }
}

void RaceInfo::setRaceLevel(ERaceLevel raceLvl)
{
    mRaceLevel = raceLvl;
    if (raceLvl == LVL_MIRROR)
        mIsMirror = true;
    else
        mIsMirror = false;
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
#line 750
        u32 dst = i + (rndm.get() % (getKartNumber() - i));
        JUT_MAX_ASSERT(dst, getKartNumber());
        
        int playerStartIdx = mStartPosIndex[i];
        mStartPosIndex[i] = mStartPosIndex[dst];
        mStartPosIndex[dst] = playerStartIdx;
    }
}

void RaceInfo::hideConsole(u32 viewNo)
{
#line 772
    JUT_MINMAX_ASSERT(1, viewNo, 5);
    mHidingConsoles |= (1 << viewNo);
}
