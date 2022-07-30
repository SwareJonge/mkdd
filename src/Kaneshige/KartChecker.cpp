#include "Kaneshige/KartChecker.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/GeoRabbitMark.h"

#include "Kaneshige/Course/CrsGround.h"
#include "Kaneshige/Course/CrsArea.h"

/*KartChkUsrPage::KartChkUsrPage(KartChecker *kartChecker)
{
    mKartChecker = kartChecker;
}

void KartChkUsrPage::draw() {
    CrsGround ground;
    ground.search(mKartChecker->curPos);
    JGeometry::TVec3<f32> vel;
    ground.getVelocity(&vel);
    
    CrsArea shadow;
    shadow.searchShadow(mKartChecker->curPos);
    CrsArea roof;
    roof.searchRoof(mKartChecker->curPos);
    CrsArea camera;
    camera.searchCamera(mKartChecker->curPos);
    CrsArea light;
    light.searchLight(mKartChecker->curPos);
    
    JUTReport(40, 80, "CRS: %f", RCMGetCourse()->getTrackSectorDist());    
    mKartChecker->printPass(80, 100);
    JUTReport(280, 30, "ALL  %08d", mKartChecker->sectorCount);
    JUTReport(280,50, "LNEW %08X", mKartChecker->isLapRenewal());
    JUTReport(280,70, "TAR  %08d", mKartChecker->sectorIndex);

    if(mKartChecker->sector2 ) {
        JUTReport(280, 90, "CUR  %08d:%f:%d", mKartChecker->sector2->isMainSector(), mKartChecker->sectorProgression, 
            mKartChecker->sector2->getGeneration());
    }

    JUTReport(280, 110, "U/T  %f", mKartChecker->lapProgression);
    JUTReport(280, 130, "TUD  %f", mKartChecker->raceProgression);
    JUTReport(280, 150, "LAP  %4d", mKartChecker->mLap);

    JUTReport(180, 170, "POS  %8.3f,%8.3f,%8.3f", mKartChecker->curPos.x, mKartChecker->curPos.y, mKartChecker->curPos.z);

    JUTReport(280,190, "GND  %04d:%3d:%02x", ground.getAttribute(), ground.getAttrIndex(), ground.getMaterial());
    JUTReport(280,210, "SPL%03d:SPI%03d", ground.getSplashCode(), ground.getSpiralCode());
    JUTReport(280,230, "SPHT %8.3f", ground.getSplashHeight());
    JUTReport(280,250, "ECHO %8.3f", roof.getEchoRate());
    JUTReport(280,270, "CD:C%3d:S%3d", ground.getCameraCode(), ground.getStaggerCode());
    JUTReport(280,290, "CAM  %3d", camera.getCameraID());    
    JUTReport(280,310, "VEL  %8.3f", vel.length());
    JUTReport(280,330, "LIGHT%8.3f:%d", light.getRate() , light.getLightParamID());
    JUTReport(280,350, "SHD  %8.3f", shadow.getShadowRate());
    JUTReport(280,370, "ADD  %2d", ground.getAddThickness());
    
    JUTReport(280,390, "LAP%1d:TOTAL%1d", mKartChecker->isBestLapTimeRenewal(), mKartChecker->isBestTotalTimeRenewal(0));
    JUTReport(280,410, "RANK %1d", mKartChecker->getRank());
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