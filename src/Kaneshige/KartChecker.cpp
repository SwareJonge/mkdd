#include <dolphin/mtx.h>
#include "Inagaki/GameAudioMain.h"
#include "Kaneshige/JugemPoint.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/Course/CrsGround.h"
#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/GeoRabbitMark.h"
#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/SysDebug.h"
#include "Osako/kartPad.h"
#include "Sato/ItemObjMgr.h"
#include "Yamamoto/kartCtrl.h"

#ifdef DEBUG
#include "Kaneshige/KartChkUsrPage.h"
#endif
#include "Kaneshige/KartChecker.h"

#include <JSystem/JAudio/JASFakeMatch2.h>

#ifdef DEBUG // I assume because Kartchecker doesn't put anything in rodata this gets deadstripped?
static const f32 lbl_80377378[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(lbl_80377378)
#pragma pop
#endif

int KartChecker::sPlayerKartColorTable[] = {
    0, 1, 2, 3, 4, 5, 6, 7};

#if DEBUG
KartChkUsrPage::KartChkUsrPage(KartChecker *kartChecker)
{
    mKartChecker = kartChecker;
}

// https://decomp.me/scratch/9s4i7
void KartChkUsrPage::draw()
{
    CrsGround ground;
    ground.search(mKartChecker->mPos);
    JGeometry::TVec3<f32> vel;
    ground.getVelocity(&vel);

    CrsArea shadow;
    shadow.searchShadow(mKartChecker->mPos);
    CrsArea roof;
    roof.searchRoof(mKartChecker->mPos);
    CrsArea camera;
    camera.searchCamera(mKartChecker->mPos);
    CrsArea light;
    light.searchLight(mKartChecker->mPos);

    JUTReport(40, 80, "CRS: %f", RCMGetCourse()->getTrackSectorDist());
    mKartChecker->printPass(80, 100);
    JUTReport(280, 30, "ALL  %08d", mKartChecker->mNumSectors);
    JUTReport(280, 50, "LNEW %08X", mKartChecker->isLapRenewal());
    JUTReport(280, 70, "TAR  %08d", mKartChecker->mSectorIdx);

    if (mKartChecker->mSector2)
    {
        JUTReport(280, 90, "CUR  %08d:%f:%d", mKartChecker->mSector2->getGeneration(),
                  mKartChecker->mSectorProgression, (u8)mKartChecker->mSector2->isMainSector());
    }

    JUTReport(280, 110, "U/T  %f", mKartChecker->mLapProgression);
    JUTReport(280, 130, "TUD  %f", mKartChecker->mRaceProgression);
    JUTReport(280, 150, "LAP  %4d", mKartChecker->mLap);

    JUTReport(180, 170, "POS  %8.3f,%8.3f,%8.3f", mKartChecker->mPos.x, mKartChecker->mPos.y, mKartChecker->mPos.z);

    JUTReport(280, 190, "GND  %04d:%3d:%02x", ground.getAttribute(), ground.getAttrIndex(), ground.getMaterial());
    JUTReport(280, 210, "SPL%03d:SPI%03d", ground.getSplashCode(), ground.getSpiralCode());
    JUTReport(280, 230, "SPHT %8.3f", ground.getSplashHeight());
    JUTReport(280, 250, "ECHO %8.3f", roof.getEchoRate());
    JUTReport(280, 270, "CD:C%3d:S%3d", ground.getCameraCode(), ground.getStaggerCode());
    JUTReport(280, 290, "CAM  %3d", camera.getCameraID());
    JUTReport(280, 310, "VEL  %8.3f", vel.length());
    JUTReport(280, 330, "LIGHT%8.3f:%d", light.getRate(), light.getLightParamID());
    JUTReport(280, 350, "SHD  %8.3f", shadow.getShadowRate());
    JUTReport(280, 370, "ADD  %2d", ground.getAddThickness());

    JUTReport(280, 390, "LAP%1d:TOTAL%1d", mKartChecker->isBestLapTimeRenewal(), mKartChecker->isBestTotalTimeRenewal(0));
    JUTReport(280, 410, "RANK %1d", mKartChecker->getRank());
}
#endif

short KartChecker::sBalForbiddenTime = 180;
short KartChecker::sBombPointDirect = 1;    // 1
short KartChecker::sBombPointSpin = 1;      // 1
short KartChecker::sBombPointIndirect = 1;  // 1
short KartChecker::sBombPointAttacked = -1; // -1
short KartChecker::sBombPointFull = 4;      // 4
short KartChecker::sBombPointFullS = 3;     // 3
short KartChecker::sBombPointFullL = 4;     // 4
short KartChecker::sBombPointCrushOneself = 0;

// https://decomp.me/scratch/DJxMp
KartChecker::KartChecker(int kartNum, KartInfo *kartInfo, int sectorNum, int lapNum)
{
    mRaceFlags = 0;
    switch (RaceMgr::getManager()->getRaceMode())
    {
    case BALLOON_BATTLE:
        setBalloonCtrl();
        break;
    case BOMB_BATTLE:
        setBombCtrl();
        break;
    case ROBBERY_BATTLE:
        break;
    case ESCAPE_BATTLE:
        setRabbitCtrl();
        break;
    case AWARD_DEMO:
        setDemoRank();
        break;
    case STAFF_ROLL:
        break;
    default:
        setLapChecking();
        break;
    }

    mTargetKartNo = kartNum;
    mNumSectors = sectorNum;
    mNumBitfields = (mNumSectors + 31) / 32;
    mPassedSectors = new u32[mNumBitfields];
    mMaxLap = lapNum;

    mLapTimes = new RaceTime[mMaxLap];
    mLapSplits = new RaceTime[mMaxLap];

    setPlayerKartColor(kartInfo);
    createGamePad(kartInfo);
    reset();

    _0xb0.zero();
    _0xbc = 0;

    if (mTargetKartNo == 0)
    {
        SYSDBG_AppendPage(new KartChkUsrPage(this));
    }
}

// https://decomp.me/scratch/yvJRl
void KartChecker::reset()
{
    mLapRenewal = false;
    mRaceEnd = false;
    _0x2a = 0;
    clrCheckPointIndex();
    mIsInRace = true;
    mCurFrame = 0;
    mGoalFrame = 0;
    mTotalTime.zero();
    mBestLapIdx = -1;
    for (int i = 0; i < mMaxLap; i++)
    {
        mLapTimes[i].reset();
        mLapSplits[i].reset();
    }
    clrRank();

    if (tstDemoRank())
        setRank(mTargetKartNo + 1);

    mPos.zero();
    mPrevPos.zero();

    mJugemPoint = nullptr;
    mBattleFlags = 0;
    mBalloonNum = 3;
    mBalForbiddenTime = 0;
    mDeathTime.reset();

    for (int i = 0; i < 10; i++)
        mBombPointTable[i] = -1;

    mBombPoint = 0;
    mMarkTime.reset();

    if (GeoRabbitMark::getSupervisor())
        mRabbitWinFrame = GeoRabbitMark::getSupervisor()->getWinFrame();
    else
        mRabbitWinFrame = 0;

    mDemoPoint = 0;
}

// https://decomp.me/scratch/zVUdm
void KartChecker::clrCheckPointIndex()
{
    mLap = -1;
    mSectorProgression = 0.0f;
    mWarpState = 0;
    mGeneration = -1;
    mSectorIdx = 0;
    mSector2 = nullptr;

    if (RCMGetCourse()->getStartSector() != nullptr)
    {
        mSector2 = RCMGetCourse()->getStartSector()->getPrevSector(0);
    }
    mSector1 = mSector2;
    mLapProgression = 1.0f;
    mPrevLapProgression = mLapProgression;
    mLapProgression2 = mLapProgression;

    for (int i = 0; i < mNumBitfields; i++)
    {
        mPassedSectors[i] = 0;
    }

    mRaceProgression = 0.0f;
}

// https://decomp.me/scratch/8JI09
void KartChecker::setPlayerKartColor(KartInfo *kartInfo)
{
    if (RaceMgr::getManager()->isLANMode())
    {
#line 1004
        JUT_MINMAX_ASSERT(0, mTargetKartNo, 8);
        mPlayerKartColor = sPlayerKartColorTable[mTargetKartNo];
    }
    else
    {
        mPlayerKartColor = -1;
        if (kartInfo->getYoungestPad() != nullptr)
        {
            switch (kartInfo->getYoungestPad()->getPadPort())
            {
            case KartGamePad::PORT_1:
                mPlayerKartColor = 0;
                break;
            case KartGamePad::PORT_2:
                mPlayerKartColor = 1;
                break;
            case KartGamePad::PORT_3:
                mPlayerKartColor = 2;
                break;
            case KartGamePad::PORT_4:
                mPlayerKartColor = 3;
                break;
            }
        }
    }
}

// https://decomp.me/scratch/gfNGd doesn't match on decomp.me because i'm too lazy to add classes
void KartChecker::createGamePad(KartInfo *kartInfo)
{
    for (int i = 0; i < 2; i++)
    {
        KartGamePad *gamePad = kartInfo->getPad(i);
        if (gamePad == nullptr)
        {
            gamePad = new KartGamePad(KartGamePad::Port_unknown, KartGamePad::PORT_INV, KartGamePad::NORMAL, KartGamePad::STATE_0);
        }
        mKartGamePads[i] = gamePad;
    }
}

// https://decomp.me/scratch/ijLhU
Course::Sector *KartChecker::searchCurrentSector(f32 *unitDist, JGeometry::TVec3<f32> const &pos, Course::Sector *curSector, int sectorCnt)
{
    Course::Sector *ret = nullptr;
    if (curSector->isDiv())
    {
        for (int i = 0; i < RCMGetCourse()->getTotalSectorNumber(); i++)
        {
            if (curSector == RCMGetCourse()->getSector(i))
                continue;

            if (curSector->getGeneration() != RCMGetCourse()->getSector(i)->getGeneration())
                continue;

            *unitDist = RCMGetCourse()->getSector(i)->calcUnitDist(pos);
            if (!isInsideSector(*unitDist))
                continue;

            ret = RCMGetCourse()->getSector(i);
            break;
        }
    }

    if (ret == nullptr)
    {
        if (curSector->isDiv())
        {
            for (int i = 0; i < RCMGetCourse()->getTotalSectorNumber(); i++)
            {
                int curGeneration = (curSector->getGeneration() + 1) % sectorCnt;
                if (curGeneration != RCMGetCourse()->getSector(i)->getGeneration())
                    continue;

                *unitDist = RCMGetCourse()->getSector(i)->calcUnitDist(pos);
                if (!isInsideSector(*unitDist))
                    continue;

                ret = RCMGetCourse()->getSector(i);
                break;
            }
        }
        else
        {
            for (int i = 0; i < curSector->getNextNum(); i++)
            {
                *unitDist = curSector->getNextSector(i)->calcUnitDist(pos);
                if (!isInsideSector(*unitDist))
                    continue;

                ret = curSector->getNextSector(i);
                break;
            }
        }
    }

    if (ret == nullptr)
    {
        if (curSector->isDiv())
        {
            for (int i = 0; i < RCMGetCourse()->getTotalSectorNumber(); i++)
            {
                int curGeneration = curSector->getGeneration() - 1;
                if (curGeneration < 0)
                    curGeneration += sectorCnt;

                if (curGeneration != RCMGetCourse()->getSector(i)->getGeneration())
                    continue;

                *unitDist = RCMGetCourse()->getSector(i)->calcUnitDist(pos);
                if (!isInsideSector(*unitDist))
                    continue;

                ret = RCMGetCourse()->getSector(i);
                break;
            }
        }
        else
        {
            for (int i = 0; i < curSector->getPrevNum(); i++)
            {
                *unitDist = curSector->getPrevSector(i)->calcUnitDist(pos);
                if (!isInsideSector(*unitDist))
                    continue;

                ret = curSector->getPrevSector(i);
                break;
            }
        }
    }

    if (ret == nullptr)
    {
        for (int i = 0; i < RCMGetCourse()->getTotalSectorNumber(); i++)
        {
            *unitDist = RCMGetCourse()->getSector(i)->calcUnitDist(pos);
            if (RCMGetCourse()->getSector(i)->isInvalid())
                continue;
            if (!isInsideSector(*unitDist))
                continue;

            ret = RCMGetCourse()->getSector(i);
            break;
        }
    }

    return ret;
}

// https://decomp.me/scratch/BQ18K
void KartChecker::checkKart()
{
    mPrevPos.set(mPos);
    KartCtrl::getKartCtrl()->GetBodyPos(mTargetKartNo, &mPos);

    if (tstLapChecking())
        checkKartLap();
    if (tstBalloonCtrl())
    {
        if (mBalForbiddenTime > 0)
            mBalForbiddenTime--;
        if (!mDeathTime.isAvailable() && isDead())
            mDeathTime.set(mCurFrame);
    }
    if (tstBombCtrl() && !mMarkTime.isAvailable() && isBombPointFull())
        mMarkTime.set(mCurFrame);
    if (tstRabbitCtrl())
        calcRabbitTime();
}

// https://decomp.me/scratch/lCRXb
void KartChecker::checkKartLap()
{
    Course::Sector *nextSector = nullptr;
    mSector1 = mSector2;

    if (mSector2->isRevSearchEnable())
    {
        for (int i = mSectorIdx; i >= 0; i--)
        {
            if (isInsideSector(RCMGetCourse()->getMainSector(i)->calcUnitDist(mPos)))
            {
                nextSector = RCMGetCourse()->getMainSector(i);
                break;
            }
        }
    }
    else
    {
        if (mWarpState == 3 && mJugemPoint != nullptr)
        {
            s16 respawnSectorID = mJugemPoint->getSectorID();
            if (respawnSectorID >= 0)
            {
                nextSector = RCMGetCourse()->searchSector(respawnSectorID);
                bool inSector = isInsideSector(nextSector->calcUnitDist(mPos));
                if (!inSector)
                {
                    for (int i = 0; i < nextSector->getPrevNum(); i++)
                    {
                        inSector = isInsideSector(nextSector->getPrevSector(i)->calcUnitDist(mPos));
                        if (inSector)
                            break;
                    }
                }
#line 1293
                JUT_ASSERT_F(inSector, "NOT INSIDE SCT%d", nextSector->getGeneration());
                if (!inSector)
                    nextSector = nullptr;
            }
        }
    }
    if (nextSector == nullptr)
    {
        if (mSector2 == RCMGetCourse()->getStartSector()->getPrevSector(0))
        {
            if (isInsideSector(RCMGetCourse()->getStartSector()->calcUnitDist(mPos)))
            {
                nextSector = RCMGetCourse()->getStartSector();
            }
        }
        if (nextSector == nullptr)
        {
            f32 unitDist = mSector2->calcUnitDist(mPos);
            if (!isInsideSector(unitDist))
                nextSector = searchCurrentSector(&unitDist, mPos, mSector2, mNumSectors);
        }
    }
    if (nextSector != nullptr)
        mSector2 = nextSector;

    f32 unitDist = mSector2->calcUnitDist(mPos);
    mPrevLapProgression = mLapProgression;
    if (unitDist >= 0.0f && unitDist <= 1.0f)
    {
        mSectorProgression = unitDist;
        mLapProgression = (mSectorProgression * mSector2->getMainSector()->getSectorDist() + mSector2->getMainSector()->getTotalPriorDist()) / RCMGetCourse()->getTrackSectorDist();
#line 1388
        JUT_ASSERT_F(isUDValid(), "UD:%5.3f,P:%8.3f,%8.3f,%8.3f", mLapProgression, mPos.x, mPos.y, mPos.z);
    }

    f32 lapProgDiff = mLapProgression - mPrevLapProgression;

    if (lapProgDiff < -0.5f)
    {
        lapProgDiff += 1.0f;
    }
    else if (lapProgDiff > 0.5f)
    {
        lapProgDiff -= 1.0f;
    }

    mRaceProgression += lapProgDiff;

    // failsafe to prevent underflows
    if (mRaceProgression < -100.0f)
        mRaceProgression = -100.0f;
}

bool KartChecker::isUDValid()
{
    return validUD(mLapProgression);
}

// https://decomp.me/scratch/7mQAF
RaceTime *KartChecker::getBestLapTime()
{
    RaceTime *bestLapTime = nullptr;
    for (int i = 0; i < mMaxLap; i++)
    {
        RaceTime *curLapTime = &mLapTimes[i];
        if (bestLapTime == nullptr || curLapTime->isLittle(*bestLapTime))
            bestLapTime = curLapTime;
    }
    return bestLapTime;
}

// https://decomp.me/scratch/0lnEb also includes a lot of inlines
void KartChecker::checkLap(bool raceEnd)
{
    if (tstLapChecking())
    {
        mLapRenewal = false;
        if (mWarpState == 3)
        {
            if (mSector2->getGeneration() > mGeneration)
            {
                int sectorGeneration = -1;
                for (int curGeneration = mGeneration; curGeneration <= mSector2->getGeneration(); curGeneration++)
                {
                    if (setPass(curGeneration))
                        sectorGeneration = curGeneration;
                }
                if (sectorGeneration != -1)
                    mSectorIdx = (sectorGeneration + 1) % mNumSectors;
            }
        }
        else
        {
            if (mSector2->getShortcutID() != 0)
            {
                Course::Sector *prevSector = mSector2->getPrevSector(0);
                while (prevSector->getShortcutID() == mSector2->getShortcutID())
                    prevSector = prevSector->getPrevSector(0);

                if (isPass(prevSector->getGeneration()))
                {
                    int shortcutID = mSector2->getShortcutID();
                    mSectorIdx = mSector2->getGeneration();
                    for (int curGeneration = 0; RCMGetCourse()->getTotalSectorNumber() > curGeneration; curGeneration++)
                    {
                        Course::Sector *pSector = RCMGetCourse()->getSector(curGeneration);
                        if (shortcutID == pSector->getShortcutID())
                        {
                            setPass(pSector->getGeneration());
                            if (pSector->getGeneration() > mSectorIdx)
                                mSectorIdx = pSector->getGeneration();
                        }
                    }

                    int nextSectorId = mSectorIdx + 1; // stupid code
                    mSectorIdx = nextSectorId;

                    if (nextSectorId >= mNumSectors)
                        mSectorIdx = 0;
                }
            }
            else
            {
                if (mSectorIdx == mSector2->getGeneration())
                {
                    setPass(mSectorIdx);
                    int nextSectorId = mSectorIdx + 1; // stupid code
                    mSectorIdx = nextSectorId;

                    if (nextSectorId >= mNumSectors)
                        mSectorIdx = 0;
                }
            }
        }
        if (mSector2->getGeneration() == 0)
        {
            if (mLap < 0)
                incLap();
            else
            {
                if (isPassAll(mNumSectors) && (int)mRaceProgression > mLap)
                {
                    if (!isGoal())
                        setLapTime();

                    mLapRenewal = true;
                    incLap();
                    if (!isGoal() && mLap >= mMaxLap)
                    {
                        setGoal();
                        setGoalTime();
                    }
                    for (int i = 1; i < mNumSectors; i++)
                        clrPass(i);
                }
            }
        }

        if ((RaceMgr::getManager()->getKartInfo(mTargetKartNo)->isComKart() && raceEnd) && !isGoal())
            setForceGoal();

        int sector = mSectorIdx;
        if (sector == 0)
            sector = mNumSectors;

        if (mSector2->getGeneration() < sector)
            mLapProgression2 = mLapProgression;

        switch (mWarpState)
        {
        case 1:
            mGeneration = mSector2->getGeneration();
            mWarpState = 2;
            break;
        case 3:
            mGeneration = -1;
            mWarpState = 0;
            mJugemPoint = nullptr;
            break;
        }
    }
}

// https://decomp.me/scratch/JVpLz
void KartChecker::setLapTime()
{
    if (mLap < mMaxLap)
    {
        JGeometry::TVec3<f32> velocity;
        velocity.sub(mPos, mPrevPos);
        JGeometry::TVec3<f32> velPerMs = velocity;
        f32 scale = 0.06f;
        velPerMs.scale(scale);
        JGeometry::TVec3<f32> curPos = mPos;

        int prevgoalframe = mCurFrame - 1;
        if (prevgoalframe < 0)
            prevgoalframe = 0;

        RaceTime goalTime;
        goalTime.setFrame(prevgoalframe);
        RaceTime finalTime = mTotalTime;
        RaceTime computedTime = finalTime;
        if (!isMaxTotalTime())
        {
            while (RCMGetCourse()->getStartSector()->calcUnitDist(curPos) >= 0.0f)
            {
                curPos.sub(velPerMs);
                computedTime.set(finalTime);
                finalTime.sub(1);
                if (computedTime.get() < goalTime.get())
                {
                    computedTime.set(goalTime);
                    break;
                }
            }
        }
#line 1687
        JUT_MINMAX_ASSERT(0, mLap, mMaxLap);

        mLapSplits[mLap].set(computedTime);
        if (!isMaxTotalTime())
        {
            // take delta of current lap split with previous lap split, this gives us the lap time
            // this of course doesn't work for lap 1 so just take the split time
            if (mLap == 0)
                mLapTimes[mLap].set(mLapSplits[mLap]);
            else
                mLapTimes[mLap].sub(mLapSplits[mLap], mLapSplits[mLap - 1]);

            if (mBestLapIdx < 0)
                mBestLapIdx = mLap;
            else if (mLapTimes[mLap].isLittle(mLapTimes[mBestLapIdx]))
                mBestLapIdx = mLap;
            return;
        }

        mLapTimes[mLap].reset();
    }
}

// https://decomp.me/scratch/m3ZyI
void KartChecker::setForceGoal()
{
    f32 distToGoal = 0.0f;
    if (mTotalTime.get() > 0)
    {
        distToGoal = mRaceProgression / (f32)mTotalTime.get();
    }
    if (distToGoal < 0.0f)
    {
        distToGoal = -distToGoal;
    }
    if (distToGoal < 0.000001f)
    {
        distToGoal = 0.0000033333333f;
    }

    // approximate the time it would've taken to get the finish
    RaceTime forcedTime;
    forcedTime.reset();
    for (int i = mLap; i < mMaxLap; i++)
    {
        if (!mLapSplits[i].isAvailable())
        {
            f32 lapDist = (((i + 1) - mRaceProgression) / distToGoal) + (mTotalTime.get());
            if (lapDist > (f32)forcedTime.get())
                lapDist = (f32)forcedTime.get();
            
            mLapSplits[i].set(lapDist + 0.5f);

            if (i <= 0)
                mLapTimes[i].set(mLapSplits[i]);
            else
                mLapTimes[i].sub(mLapSplits[i], mLapSplits[i - 1]);
        }
    }

    mLap = mMaxLap;
    setGoal();
    setGoalTime();
}

// https://decomp.me/scratch/DJhtT also includes isPass()
bool KartChecker::setPass(int sectorIdx)
{
    bool pass = false;
    bool valid = true;
    if (sectorIdx != 0)
    {
        for (int i = 0; i < sectorIdx; i++)
        {
            if (!isPass(i))
            {
                valid = false;
                break;
            }
        }
    }

    if (valid)
    {
        int index = sectorIdx / 32;
        pass = true;
        int bitIndex = sectorIdx % 32;
        valid = false;
#line 1791
        JUT_MINMAX_ASSERT(0, index, mNumBitfields);
        mPassedSectors[index] |= (1 << bitIndex);
    }
    return pass;
}

// https://decomp.me/scratch/Wp30Q
bool KartChecker::isPassAll(int SectorCnt)
{
    bool allPassed = true;
    int lastSectorIdx = -1; // stores the index of the first sector that has not been passed
    for (int i = 0; i < SectorCnt; i++)
    {
        if (!isPass(i))
        {
            allPassed = false;
            if (lastSectorIdx == -1)
                lastSectorIdx = i;
        }
    }
    return allPassed;
}

// https://decomp.me/scratch/mCXI2
void KartChecker::incTime()
{
    if (mIsInRace)
    {
        if (mCurFrame < MAX_FRAME)
        {
            mCurFrame++;
        }
        mTotalTime.setFrame(mCurFrame);
    }
}

// https://decomp.me/scratch/74FW0
bool KartChecker::isReverse()
{
    bool reverse = false;

    if (tstLapChecking())
    {
        const Mtx &m = RaceMgr::getManager()->getKartLoader(mTargetKartNo)->getExModelBody()->getBaseTRMtx();
        JGeometry::TVec3<f32> yaw; // or is this pitch? y = z in math made me confused
        yaw.set(m[0][2], m[1][2], m[2][2]);
        yaw.normalize();

        JGeometry::TVec3<f32> cpDir;
        mSector2->getBNormal(&cpDir);
        if (yaw.dot(cpDir) <= -0.5f)
            reverse = true;
    }
    return reverse;
}

bool KartChecker::isFinalLapRenewal() const
{
    bool renewal = false;
    if (isLapRenewal() && isFinalLap())
        renewal = true;

    return renewal;
}

// https://decomp.me/scratch/JeodW
bool KartChecker::isCurrentLapTimeRenewal()
{
    bool currentLapRenewal = false;
    if (mLap > 0)
    {
        const RaceTime &laptime = getLapTime(mLap - 1);
        if (laptime.isAvailable() && laptime.isLittle(RaceMgr::getManager()->getBestLapTime()) && mBestLapIdx == mLap - 1)
            currentLapRenewal = true;
    }
    return currentLapRenewal;
}

// https://decomp.me/scratch/kIxFJ
bool KartChecker::isBestLapTimeRenewal(void)
{
    bool renewal = false;
    for (int i = 0; i < mMaxLap; i++)
    {
        if (getLapTime(i).isAvailable() && (getLapTime(i).isLittle(RaceMgr::getManager()->getBestLapTime())))
        {
            renewal = true;
            break;
        }
    }
    return renewal;
}

bool KartChecker::isBestTotalTimeRenewal(int recID)
{
    bool renewal = false;
    if (getTotalTime().isAvailable() && (getTotalTime().isLittle(RaceMgr::getManager()->getBestTotalTime(recID))))
        renewal = true;

    return renewal;
}

bool KartChecker::incBalloon()
{
    bool incremented = false;
    if (tstBalloonCtrl() && !tstFixMiniPoint() && mBalloonNum < 3)
    {
        mBalloonNum++;
        incremented = true;
        mBalForbiddenTime = 0;
    }
    return incremented;
}

// Dumb code incoming
// https://decomp.me/scratch/5J3WZ
bool KartChecker::decBalloon()
{
    bool decreased = false;
    if (!tstBalloonCtrl())
        return false;
    if (mBalForbiddenTime > 0)
        return false;
    if (tstFixMiniPoint())
        return false;
    if (RaceMgr::getManager()->checkRaceEnd())
        return false;
    
    if (mBalloonNum > 0)
    {
        mBalloonNum--;
        mBalForbiddenTime = sBalForbiddenTime;
        if (mBalloonNum <= 0)
            setDead();
        decreased = true;
    }

    return decreased;
}

// https://decomp.me/scratch/bDxAr
bool KartChecker::incMyBombPoint(int pnt, int increment)
{
    bool increased = false;
    int bombPoint = mBombPoint;

    if (!isBombPointFull() && !tstFixMiniPoint())
    {
        mBombPoint += increment;
        if (mBombPoint > sBombPointFull)
            mBombPoint = sBombPointFull;
        else if (mBombPoint < 0)
            mBombPoint = 0;

        if (mBombPoint < bombPoint)
        {
            for (int pntNo = mBombPoint; pntNo < bombPoint; pntNo++)
            {
#line 2071
                JUT_MINMAX_ASSERT(0, pntNo, 10);
                mBombPointTable[pntNo] = -1;
            }
            if (bombPoint == sBombPointFull - 1)
                GetKartCtrl()->GetKartSoundMgr(mTargetKartNo)->setSe(0x1009a);
        }
        else if (mBombPoint > bombPoint)
        {
            for (int pntNo = bombPoint; pntNo < mBombPoint; pntNo++)
            {
#line 2097
                JUT_MINMAX_ASSERT(0, pntNo, 10);
                mBombPointTable[pntNo] = pnt;
            }
        }
        increased = true;
    }

    return increased;
}

// https://decomp.me/scratch/fBBv9
bool KartChecker::incYourBombPoint(int idx, int pnt, int increment)
{
    return RaceMgr::getManager()->getKartChecker(idx)->incMyBombPoint(pnt, increment);
}

// https://decomp.me/scratch/Pl4Wz
void KartChecker::setBombEvent(KartChecker::EBombEvent bombEvent, ItemObj *itemObj)
{
    if (tstBombCtrl() && !isBombPointFull())
    {
        int ownerNum = -1;
        if (itemObj != nullptr)
            ownerNum = itemObj->getOwnerNum();
        switch (bombEvent)
        {
        case EVENT_1:
            int increment = sBombPointCrushOneself;
            if (ownerNum != mTargetKartNo)
                increment = sBombPointAttacked;
            if (increment != 0)
                incMyBombPoint(-1, increment);
            if (ownerNum != mTargetKartNo)
            {
                if (itemObj->getKartReaction() != 8)
                    increment = sBombPointSpin;
                else
                {
                    if (mTargetKartNo != itemObj->getDirectHitKartNo())
                        increment = sBombPointIndirect;
                    else
                        increment = sBombPointDirect;
                }

                incYourBombPoint(ownerNum, mTargetKartNo, increment);
            }
            break;
        case EVENT_3:
            incMyBombPoint(-1, -1);
            break;
        }
    }
}

// https://decomp.me/scratch/uXEO9
int KartChecker::getRobberyItemNumber(void)
{
    int num = 0;
    for (u8 i = 0; i < 2; i++)
        num += GetItemObjMgr()->getRobberyItemNum(mTargetKartNo, (int)i);
    return num;
}

// https://decomp.me/scratch/0guno
bool KartChecker::releaseRabbitMark()
{
    bool released = false;
    if (tstRabbitCtrl() && !tstFixMiniPoint())
    {
        GeoRabbitMarkSupervisor *supervisor = GeoRabbitMark::getSupervisor();
        if (supervisor != nullptr)
        {
            if (mTargetKartNo == supervisor->getRabbitKartNo() && !isRabbitWinner())
            {
                supervisor->release();
                released = true;
            }
        }
    }
    return released;
}

// https://decomp.me/scratch/9G0Hj
bool KartChecker::isRabbit() const
{
    bool rabbit = false;
    if (GeoRabbitMark::getSupervisor())
    {
        rabbit = mRabbitWinFrame < GeoRabbitMark::getSupervisor()->getWinFrame();
    }
    return rabbit;
}

void KartChecker::calcRabbitTime()
{
    GeoRabbitMarkSupervisor *supervisor = GeoRabbitMark::getSupervisor();
    if (supervisor == nullptr)
        return;
    if (mRabbitWinFrame > 0)
    {
        if (mTargetKartNo == supervisor->getRabbitKartNo())
        {
            if (!tstStillRabbitTimer())
            {
                mRabbitWinFrame--;
                RaceTime rabbitTime;
                rabbitTime.setFrame(mRabbitWinFrame);
                int ms = rabbitTime.getUpwardMSec() / 1000;
                if (!(rabbitTime.get() % 1000))
                {
                    if (ms == 0)
                        GetGameAudioMain()->startSystemSe(0x20027);
                    else if (ms <= 10)
                    {
                        GetGameAudioMain()->startSystemSe(0x20026);
                    }
                    else if ((ms > 10) && ms <= GeoRabbitMark::getSupervisor()->getWinTime() - 2)
                    {
                        GetGameAudioMain()->startSystemSe(0x20025);
                    }
                }
            }
        }
        else
        {
            mRabbitWinFrame = GeoRabbitMark::getSupervisor()->getWinFrame();
            resumeRabbitTimer();
        }
    }        
    else
    {
        supervisor->startWinnerEffect();
    }    
}

LapChecker::LapChecker()
{
    reset();
}

void LapChecker::reset()
{
    mSector = nullptr;
    mSectorDist = 0.0f;
    mLapUnitDist = 0.0f;
}

void LapChecker::start(Course::Sector *sector)
{
    mSector = sector;
}

// https://decomp.me/scratch/VlU2K
void LapChecker::calc(const JGeometry::TVec3<f32> &pos)
{
    if (mSector != nullptr)
    {
        Course::Sector *nextSector = nullptr;
        f32 dist = mSector->calcUnitDist(pos);
        if (!KartChecker::isInsideSector(dist))
            nextSector = KartChecker::searchCurrentSector(&dist, pos, mSector, RCMGetCourse()->getTrackSectorNumber());
        if (nextSector != nullptr)
            mSector = nextSector;

        f32 unitDist = mSector->calcUnitDist(pos);
        if (unitDist >= 0.0f && unitDist <= 1.0f)
        {
            mSectorDist = unitDist;
            mLapUnitDist = (mSectorDist * mSector->getMainSector()->getSectorDist() + mSector->getMainSector()->getTotalPriorDist()) / RCMGetCourse()->getTrackSectorDist();
#line 2430
            JUT_ASSERT_F(isUDValid(), "LAP UD:%5.3f,P:%8.3f,%8.3f,%8.3f", mLapUnitDist, pos.x, pos.y, pos.z);
        }
    }
}

bool LapChecker::isUDValid()
{
    return validUD(mLapUnitDist);
}
