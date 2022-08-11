#include "Kaneshige/KartChecker.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/GeoRabbitMark.h"

#include "Kaneshige/Course/CrsGround.h"
#include "Kaneshige/Course/CrsArea.h"

#include "Yamamoto/kartCtrl.h"

/*#define SDA2_BASE 0x8041f1c0
#include "orderfloats/80419de8_80419df0.inc"
#include "orderfloats/80419df8_80419e04.inc"*/

static const float lbl_80377378[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(lbl_80377378)
#pragma pop

int KartChecker::sPlayerKartColorTable[] = {
    0, 1, 2, 3, 4, 5, 6, 7};

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
    JUTReport(280, 30, "ALL  %08d", mKartChecker->sectorCount);
    JUTReport(280, 50, "LNEW %08X", mKartChecker->isLapRenewal());
    JUTReport(280, 70, "TAR  %08d", mKartChecker->sectorIndex);

    if (mKartChecker->sector2)
    {
        JUTReport(280, 90, "CUR  %08d:%f:%d", mKartChecker->sector2->getGeneration(),
                  mKartChecker->sectorProgression, mKartChecker->sector2->isMainSector());
    }

    JUTReport(280, 110, "U/T  %f", mKartChecker->lapProgression);
    JUTReport(280, 130, "TUD  %f", mKartChecker->raceProgression);
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

// https://decomp.me/scratch/DJxMp
KartChecker::KartChecker(int kartNum, KartInfo *kartInfo, int sectorNum, int lapNum)
{
    raceFlags = 0;
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
    sectorCount = sectorNum;
    bitfieldCnt = (sectorCount + 31) / 32;
    cpBitfields = new s32[bitfieldCnt];
    mMaxLap = lapNum;

    mLapTimes = new RaceTime[mMaxLap];
    mBestLapTimes = new RaceTime[mMaxLap];

    setPlayerKartColor(kartInfo);
    createGamePad(kartInfo);
    reset();

    _0xb0.zero();
    _0xbc = 0;

    if (mTargetKartNo == 0)
    {
        KartChkUsrPage *usrPage = new KartChkUsrPage(this);
        SysDebug::getManager()->appendPage(usrPage);
    }
}

// https://decomp.me/scratch/yvJRl
void KartChecker::reset()
{
    mLapRenewal = false;
    mRaceEnd = false;
    _0x2a = 0;
    clrCheckPointIndex();
    _0x78 = true;
    curFrame = 0;
    mGoalFrame = 0;
    mTotalTime.zero();
    mBestLapIdx = -1;
    for (int i = 0; i < mMaxLap; i++)
    {
        mLapTimes[i].reset();
        mBestLapTimes[i].reset();
    }
    clrRank();

    if (tstDemoRank())
        setRank(mTargetKartNo + 1);

    mPos.zero();
    mPrevPos.zero();

    mJugemPoint = nullptr;
    battleFlags = 0;
    balloonNumber = 3;
    mBalForbiddenTime = 0;
    mDeathTime.reset();

    for (int i = 0; i < 10; i++)
        bombPointTable[i] = -1;

    bombPoint = 0;
    mMarkTime.reset();

    if (GeoRabbitMark::getSupervisor())
        rabbitWinFrame = GeoRabbitMark::getSupervisor()->getWinFrame();
    else
        rabbitWinFrame = 0;

    demoPoint = 0;
}

// https://decomp.me/scratch/zVUdm
void KartChecker::clrCheckPointIndex()
{
    mLap = -1;
    sectorProgression = 0.0f;
    warpState = 0;
    mGeneration = -1;
    sectorIndex = 0;
    sector2 = nullptr;

    if (RCMGetCourse()->getStartSector() != nullptr)
    {
        sector2 = RCMGetCourse()->getStartSector()->getPrevSector(0);
    }
    sector1 = sector2;
    lapProgression = 1.0f;
    prevlapProgression = lapProgression;
    lapProgression2 = lapProgression;

    for (int i = 0; i < bitfieldCnt; i++)
    {
        cpBitfields[i] = 0;
    }

    raceProgression = 0.0f;
}

// https://decomp.me/scratch/8JI09
void KartChecker::setPlayerKartColor(KartInfo *kartInfo)
{
    if (RaceMgr::getManager()->isLANMode())
    {
        bool valid = false;
        if (mTargetKartNo >= 0 && mTargetKartNo < 8)
        {
            valid = true;
        }
        if (!valid)
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 1004, "range over: %d <= mTargetKartNo=%d < %d", 0, mTargetKartNo, 8);
            OSPanic(__FILE__, 1004, "Halt");
        }
        mPlayerKartColor = sPlayerKartColorTable[mTargetKartNo];
    }
    else
    {
        mPlayerKartColor = -1;
        if (kartInfo->getYoungestPad() != nullptr)
        {
            switch ((int)kartInfo->getYoungestPad()->getPadPort())
            {
            case 0:
                mPlayerKartColor = 0;
                break;
            case 1:
                mPlayerKartColor = 1;
                break;
            case 2:
                mPlayerKartColor = 2;
                break;
            case 3:
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
            gamePad = new KartGamePad((JUTGamePad::EPadPort)0xfffffc19, (KartGamePad::PadPort)-1,
                                      (KartGamePad::PadType)0, (KartGamePad::PadState)0);
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
                int curGeneration = curSector->getGeneration();
                curGeneration--;
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
    mPos.set(mPos);
    KartCtrl::getKartCtrl()->GetBodyPos(mTargetKartNo, &mPos);

    if (tstLapChecking())
        checkKartLap();
    if (tstBalloonCtrl())
    {
        if (mBalForbiddenTime > 0)
            mBalForbiddenTime--;
        if (!mDeathTime.isAvailable() && isDead())
            mDeathTime.set(curFrame);
    }
    if (tstBombCtrl() && !mMarkTime.isAvailable() && isBombPointFull())
        mMarkTime.set(curFrame);
    if (tstRabbitCtrl())
        calcRabbitTime();
}

// https://decomp.me/scratch/lCRXb
void KartChecker::checkKartLap()
{
    Course::Sector *nextSector = nullptr;
    sector1 = sector2;

    if (sector2->isRevSearchEnable())
    {
        for (int i = sectorIndex; i >= 0; i--)
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
        if (warpState == 3 && mJugemPoint != nullptr)
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
                if (!inSector)
                {
                    JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 1293, "NOT INSIDE SCT%d", nextSector->getGeneration());
                    OSPanic(__FILE__, 1293, "Halt");
                }
                if (!inSector)
                    nextSector = nullptr;
            }
        }
    }
    if (nextSector == nullptr)
    {
        if (sector2 == RCMGetCourse()->getStartSector()->getPrevSector(0))
        {
            if (isInsideSector(RCMGetCourse()->getStartSector()->calcUnitDist(mPos)))
            {
                nextSector = RCMGetCourse()->getStartSector();
            }
        }
        if (nextSector == nullptr)
        {
            f32 unitDist = sector2->calcUnitDist(mPos);
            if (!isInsideSector(unitDist))
                nextSector = searchCurrentSector(&unitDist, mPos, sector2, sectorCount);
        }
    }
    if (nextSector != nullptr)
        sector2 = nextSector;

    f32 unitDist = sector2->calcUnitDist(mPos);
    prevlapProgression = lapProgression;
    if (unitDist >= 0.0f && unitDist <= 1.0f)
    {
        sectorProgression = unitDist;
        lapProgression = (sectorProgression * sector2->getMainSector()->getSectorDist() + sector2->getMainSector()->getTotalPriorDist()) / RCMGetCourse()->getTrackSectorDist();

        if (!isUDValid())
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 1388, "UD:%5.3f,P:%8.3f,%8.3f,%8.3f", lapProgression, mPos.x, mPos.y, mPos.z);
            OSPanic(__FILE__, 1388, "Halt");
        }
    }

    f32 lapProgDiff = lapProgression - prevlapProgression;

    if (lapProgDiff < -0.5f)
    {
        lapProgDiff += 1.0f;
    }
    else if (lapProgDiff > 0.5f)
    {
        lapProgDiff -= 1.0f;
    }

    raceProgression += lapProgDiff;

    // failsafe to prevent underflows
    if (raceProgression < -100.0f)
        raceProgression = -100.0f;
}

bool KartChecker::isUDValid()
{
    return (lapProgression >= 1.0f && lapProgression <= 0.0f);
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
        int curGeneration;
        mLapRenewal = false;
        if (warpState == 3)
        {

            if (sector2->getGeneration() > mGeneration)
            {
                curGeneration = mGeneration;
                int sectorGeneration = -1;
                for (; curGeneration <= sector2->getGeneration(); curGeneration++)
                {
                    if (setPass(curGeneration))
                        sectorGeneration = curGeneration;
                }
                if (sectorGeneration != -1)
                    sectorIndex = (sectorGeneration + 1) % sectorCount;
            }
        }
        else
        {
            if (sector2->getShortcutID() != 0)
            {
                Course::Sector *prevSector = sector2->getPrevSector(0);
                while (prevSector->getShortcutID() == sector2->getShortcutID())
                    prevSector = prevSector->getPrevSector(0);

                if (isPass(prevSector->getGeneration()))
                {
                    int shortcutID = sector2->getShortcutID();
                    sectorIndex = sector2->getGeneration();
                    for (curGeneration = 0; RCMGetCourse()->getTotalSectorNumber() > curGeneration; curGeneration++)
                    {
                        Course::Sector *pSector = RCMGetCourse()->getSector(curGeneration);
                        if (shortcutID == pSector->getShortcutID())
                        {
                            setPass(pSector->getGeneration());
                            if (pSector->getGeneration() > sectorIndex)
                                sectorIndex = pSector->getGeneration();
                        }
                    }

                    int nextSectorId = sectorIndex + 1; // stupid code
                    sectorIndex = nextSectorId;

                    if (nextSectorId >= sectorCount)
                        sectorIndex = 0;
                }
            }
            else
            {
                if (sectorIndex == sector2->getGeneration())
                {
                    setPass(sectorIndex);
                    int nextSectorId = sectorIndex + 1; // stupid code
                    sectorIndex = nextSectorId;

                    if (nextSectorId >= sectorCount)
                        sectorIndex = 0;
                }
            }
        }
        if (sector2->getGeneration() == 0)
        {
            if (mLap < 0)
                incLap();
            else
            {
                if (isPassAll(sectorCount) && (int)raceProgression > mLap)
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
                    for (int i = 1; i < sectorCount; i++)
                        clrPass(i);
                }
            }
        }

        if ((RaceMgr::getManager()->getKartInfo(mTargetKartNo)->isComKart() && raceEnd) && !isGoal())
            setForceGoal();

        int sector = sectorIndex;
        if (sector == 0)
            sector = sectorCount;

        if (sector2->getGeneration() < sector)
            lapProgression2 = lapProgression;

        switch (warpState)
        {
        case 1:
            mGeneration = sector2->getGeneration();
            warpState = 2;
            break;
        case 3:
            mGeneration = -1;
            warpState = 0;
            mJugemPoint = nullptr;
            break;
        }
    }
}

// not sure if this matches
// https://decomp.me/scratch/JVpLz
void KartChecker::setLapTime()
{
    if (mLap < mMaxLap)
    {
        JGeometry::TVec3<f32> velocity;
        velocity.sub(mPos, mPrevPos);
        JGeometry::TVec3<f32> velPerMs = velocity;
        velPerMs.scale(0.06f);
        JGeometry::TVec3<f32> curPos = mPos;

        int prevgoalframe = mGoalFrame + -1;
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
                if (goalTime.get() < computedTime.get())
                {
                    computedTime.set(goalTime);
                    break;
                }
            }
        }

        bool valid = false;
        // Make assert function, this is crap
        prevgoalframe = mLap;
        if (mLap >= 0 && mLap < mMaxLap)
            valid = true;
        if (!valid)
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 1687, "range over: %d <= mLap=%d < %d", 0, prevgoalframe, mMaxLap);
            OSPanic(__FILE__, 1687, "Halt");
        }
        mBestLapTimes[mLap].set(computedTime);
        if (!isMaxTotalTime())
        {
            if (mLap == 0)
                mLapTimes[mLap].set(mBestLapTimes[mLap]);
            else
                mLapTimes[mLap].sub(mBestLapTimes[mLap], mBestLapTimes[mLap - 1]);

            if (mBestLapIdx < 0)
                mBestLapIdx = mLap;
            else if (mLapTimes[mLap].isLittle(mLapTimes[mBestLapIdx]))
                mBestLapIdx = mLap;
        }
        else
            mLapTimes[mLap].reset();
    }
}

// not sure this matches either
// https://decomp.me/scratch/m3ZyI
void KartChecker::setForceGoal()
{
    float distToGoal = 0.0f;
    if (mTotalTime.get() > 0)
    {
        distToGoal = raceProgression / (float)mTotalTime.get();
    }
    if (distToGoal < 0.0f)
    {
        distToGoal = -distToGoal;
    }
    if (distToGoal < 1.0E6f)
    {
        distToGoal = 3.3333333E6f;
    }

    RaceTime forcedTime;
    forcedTime.reset();
    for (int i = mLap; i < mMaxLap; i++)
    {
        if (!mBestLapTimes[i].isAvailable())
        {
            float lapDist = (((i + 1) - raceProgression) / distToGoal) + (mTotalTime.get());
            if (lapDist > (float)forcedTime.get())
            {
                lapDist = (float)forcedTime.get();
            }
            mBestLapTimes[i].set(lapDist + 0.5f);
            if (i <= 0)
            {
                mLapTimes[i].set(mBestLapTimes[i]);
            }
            else
            {
                mLapTimes[i].sub(mBestLapTimes[i], mBestLapTimes[i - 1]);
            }
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
        if (index >= 0 && index < bitfieldCnt)
            valid = true;
        if (!valid)
        {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 1791, "range over: %d <= index=%d < %d", 0, index, bitfieldCnt);
            OSPanic(__FILE__, 1791, "Halt");
        }
        cpBitfields[index] |= (1 << bitIndex);
    }
    return pass;
}

// https://decomp.me/scratch/Wp30Q
bool KartChecker::isPassAll(int SectorCnt)
{
    bool allPassed = true;
    int thing = -1;
    for (int i = 0; i < SectorCnt; i++)
    {
        if (!isPass(i))
        {
            allPassed = false;
            if (thing == -1)
                thing = i;
        }
    }
    return allPassed;
}

// https://decomp.me/scratch/mCXI2
void KartChecker::incTime()
{
    if (_0x78)
    {
        if (curFrame < MAX_FRAME)
        {
            curFrame++;
        }
        mTotalTime.setFrame(curFrame);
    }
}

/*bool KartChecker::isReverse() {
    bool reverse = false;

    if(tstLapChecking()) {
        RaceMgr::getManager()->getkartLoader();
    }
    return reverse;
}*/

// https://decomp.me/scratch/JeodW
bool KartChecker::isCurrentLapTimeRenewal()
{
    bool currentLapRenewal = false;
    if (mLap > 0)
    {
        const RaceTime &laptime = getLapTime(mLap - 1);
        if (laptime.isAvailable())
        {
            if (laptime.isLittle(RaceMgr::getManager()->getBestLapTime()) && mBestLapIdx == mLap - 1)
            {
                currentLapRenewal = true;
            }
        }
    }
    return currentLapRenewal;
}
