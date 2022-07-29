#include "Kaneshige/KartChecker.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/GeoRabbitMark.h"

/*KartChkUsrPage::KartChkUsrPage(KartChecker *kartChecker)
{
    mKartChecker = kartChecker;
}*/

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

    kartIndex = kartNum;
    sectorCount = sectorNum;
    bitfieldCnt = (sectorCount + 31) / 32;
    cpBitfields = new s32[bitfieldCnt];
    trackLapCount = lapNum;

    laptimes1 = new RaceTime[trackLapCount];
    laptimes2 = new RaceTime[trackLapCount];
    
    setPlayerKartColor(kartInfo);
    createGamePad(kartInfo);
    reset();

    _0xb0.zero();
    _0xbc = 0;

    if (kartIndex == 0)
    {
        KartChkUsrPage *usrPage = new KartChkUsrPage(this);
        SysDebug::getManager()->appendPage(usrPage);
    }
}

void KartChecker::reset()
{
    mLapRenewal = false;
    mRaceEnd = false;
    _0x2a = 0;
    clrCheckPointIndex();
    _0x78 = true;
    curFrame = 0;
    goalFrame = 0;
    mTotalTime.zero();
    _0x10 = -1;
    for (int i = 0; i < trackLapCount; i++)
    {
        laptimes1[i].reset();
        laptimes2[i].reset();
    }
    clrRank();

    if (tstDemoRank())
        setRank(kartIndex + 1);

    curPos.zero();
    prevPos.zero();

    jugemPoint = 0;
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