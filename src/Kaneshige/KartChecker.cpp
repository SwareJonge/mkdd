#include "Kaneshige/KartChecker.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceMgr.h"

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