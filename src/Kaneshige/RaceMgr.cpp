/* Todo list:
Class Sizes?
text ordering
*/
/*
I guess the idea here was to not create seperate sections for JSystem? didnt work that well
Also, make headers for all libraries itself?
*/
#include <dolphin/os.h>
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTGamePad.h"
#if DEBUG
#pragma sym on
// couldve been a default log function
#define RCMLogMsg(...) OSReport(__VA_ARGS__)
#else
#define RCMLogMsg(...)
#endif

#include "kartEnums.h"
#include "kartLocale.h"
#include "Inagaki/GameAudioMain.h"
#include "Kameda/J2DManager.h"
#include "Kameda/MotorManager.h"
#include "Kameda/PauseManager.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/Course/CrsData.h"
#include "Kaneshige/DemoTimeKeeper.h"
#include "Kaneshige/GeoRabbitMark.h"
#include "Kaneshige/ExModel.h"
#include "Kaneshige/LightMgr.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceLight.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/RaceUsrPage.h"
#include "Kaneshige/SysDebug.h"
#include "Kaneshige/TexLODControl.h"
#include "Kawano/accessory.h"
#include "Osako/kartPad.h"
#include "Osako/RaceApp.h"
#include "Osako/ResMgr.h"
#include "Osako/shadowMgr.h"
#include "Osako/system.h"
#include "Osako/SystemRecord.h"
#include "Sato/EffectScreen.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/J3DEffectMgr.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/NeckCtrl.h"
#include "Sato/ObjUtility.h"
#include "Sato/RivalKart.h"
#include "Sato/stEffectMgr.h"
#include "Sato/stMath.h"
#include "Shiraiwa/Balloon.h"
#include "Shiraiwa/JugemRodSignal.h"
#include "Yamamoto/kartCamera.h"

RaceMgr *RaceMgr::sRaceManager;
short RaceMgr::sForceTotalLapNum;
short RaceMgr::sDispFrameCounter;

short RaceMgr::sMyStartPointID = -1;

#if PAL // not sure if needed
static const float lbl_8033ce58[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(lbl_8033ce58)
#pragma pop
#else
static const float lbl_80377640[4] = {0.0f, 0.0f, 0.0f, 1.0f};
#pragma push
#pragma force_active on
DUMMY_POINTER(lbl_80377640)
#pragma pop
#endif

const RaceMgr::EventInfo RaceMgr::sEventTable[] = {
    {0, "空", " SKY"},
    {1, "コース", " CRS"},
    {2, "地形ＯＢＪ", " GEOOBJ"},
    {3, "カート影", " KART SHADOW"},
    {4, "タイヤ跡", " WHEEL TRACKS"},
    {5, "カート", " KART"},
    {6, "アイテム影", " ITEM SHADOW"},
    {7, "アイテム", " ITEM"},
    {8, "エフェクト", " EFFECT"},
    {9, "パーティクル", " PARTICLE"},
    {0xa, "シャドウスクリーン", " SHADOW SCR"},
    {0xb, "スクリーンエフェクト", " SCR EFFECT"},
    {0xc, "２Ｄ", " 2D"},
    {0xd, "Ｚキャプチャ", " ZCAPTURE"},
    {0x100, "２Ｄ", " 2D"},
    {0x101, "カートダイナミクス１", " KART DYN1"},
    {0x102, "カートダイナミクス２", " KART DYN2"},
    {0x103, "コース", " CRS"},
    {0x104, "エフェクト", " EFFECT"},
    {0x105, "地形ＯＢＪ", "\tGEOOBJ"},
    {0x106, "アイテム", " ITEM"},
    {0x107, "パーティクル", " PARTICLE"},
    {0x108, "スクリーンエフェクト", " SCR EFFECT"},
    {0x109, "周回チェック", " LOOP CHECK"},
    {0x10a, "実験", " FOR TEST"},
    {0x110, "デバッグ１", " DEBUG1"},
    {0x111, "デバッグ２", " DEBUG2"},
    {0x112, "デバッグ３", " DEBUG3"},
    {0x113, "デバッグ４", " DEBUG4"},
    {0x114, "デバッグ５", " DEBUG5"},
    {0x115, "デバッグ６", " DEBUG6"},
    {0x116, "デバッグ７", " DEBUG7"},
    {0x117, "デバッグ８", " DEBUG8"},
    {0x118, "デバッグ９", " DEBUG9"},
    {0x119, "デバッグ１０", " DEBUG10"},
    {0x200, "空", " SKY"},
    {0x201, "コースとＯＢＪ", " CRS&OBJ"},
    {0x202, "アイテム", " ITEM"},
    {0x203, "ドライバー", " DRIVER"},
    {0x204, "影", " SHADOW"},
    {0x205, "エフェクト", " EFFECT"},
    {0x300, "空", " SKY"},
    {0x301, "コース", " CRS"},
    {0x302, "アイテム", " ITEM"},
    {0x303, "カート", " KART"},
    {0x304, "影", " SHADOW"},
    {0x305, "エフェクト", " EFFECT"},
    {-1, "なし", nullptr}};

// half fabircated to generate certain data first
// Note: Kartcam might be part of KartCtrl.h?
void PrintRaceHeap(u32 free, JKRHeap *heap)
{
    JUTReport(KartCtrl::getKartCtrl()->getKartCam(0)->GetPosh(), KartCtrl::getKartCtrl()->getKartCam(0)->GetPosv(), "LINE%4d:(%d/%d)\n", 0, free, heap->getHeapSize());
}

RaceMgr::RaceMgr(RaceInfo *raceInfo) : mRaceInfo(nullptr),
                                       mCourse(nullptr),
                                       mBestLapTime()
{

    Language lang = KartLocale::getLanguage();
    VideoMode videoMode = KartLocale::getVideoMode();
    RCMLogMsg("Demo    Start-------------------------------------------------------\n");
    RCMLogMsg("Race Mode:%08X\n", raceInfo->getRaceMode());
    RCMLogMsg("Stat Num :%08X\n", raceInfo->getStatusNumber());
    RCMLogMsg("Rand Seed:%08X\n", raceInfo->mRandomSeed);

    sRaceManager = this;

    SysDebug::getManager()->setHeapGroup("RACE MGR", nullptr);
    _0x22 = 0;
    mFrame = 0;
    mRaceHeap = JKRGetCurrentHeap();
    mReplayMode = 0;
    mRaceInfo = raceInfo;

    editRaceInfoForDebug();
#if DEBUG
    RaceUsrPage *racePage = new RaceUsrPage(mRaceInfo);
    SysDebug::getManager()->appendPage(racePage);
#endif

    _0x30 = 0; // might've been used for debug only, this is the only time it gets used
    mAbleStart = false;
    mAreaLight = 0;

    if (getProcLevel() <= 0)
        activeAreaLight();

    bool bVar17 = false;
    if (!isWaitDemoMode())
    {
        switch (getRaceMode())
        {
        case GRAND_PRIX:
        case BALLOON_BATTLE:
        case ROBBERY_BATTLE:
        case BOMB_BATTLE:
        case ESCAPE_BATTLE:
            bVar17 = true;
            break;
        }
    }

    DemoTimeKeeper *timeKeeper = nullptr;
    if (isAwardDemoMode())
        timeKeeper = new AwardDemoTimeKeeper();
    else if (isStaffRoll())
        timeKeeper = new StaffRollTimeKeeper();

    mRaceDirector = new RaceDirector(bVar17, timeKeeper, getConsoleNumber());

    u16 uVar20 = 1;
    if (mRaceInfo->isDriverLODOn())
        uVar20 = 2;

    SysDebug::getManager()->setHeapGroup("COURSE MGR", nullptr);

    CrsData::SColHeader *bco = (CrsData::SColHeader *)ResMgr::getPtr(ResMgr::COURSE_BCO);
    CrsData::SOblHeader *bol = (CrsData::SOblHeader *)ResMgr::getPtr(ResMgr::COURSE_BOL);
    CrsData *crsData = new CrsData(bco, bol);

    if (isStaffRoll())
        crsData->patchCamDataForStaffRoll(lang, videoMode);

    RCMLogMsg("Course ID:%08X\n", crsData->getCourseID());

    for (int i = 0; i < 16; i++)
    {
        KartGamePad *gamepad = gpaKartPad[i];
        if (gamepad->testButton(0x10))
        {
            if (gamepad->testButton(0x800))
                sForceTotalLapNum = 1;
            else if (gamepad->testButton(0x400))
                sForceTotalLapNum = 2;
        }
        if (sForceTotalLapNum != 0)
            break;
    }

    if (sForceTotalLapNum == 0)
    {
        if (crsData->getTotalLapNumber() != 0)
            mTotalLapNumber = crsData->getTotalLapNumber();
        else
            mTotalLapNumber = 3;

        if (mRaceInfo->getLANLapNumber() > 0)
            mTotalLapNumber = mRaceInfo->getLANLapNumber();
        else if (getRaceMode() == VERSUS_RACE && mRaceInfo->getVSLapNumber() > 0)
            mTotalLapNumber = mRaceInfo->getVSLapNumber();
    }
    else
        mTotalLapNumber = sForceTotalLapNum;

    int iVar4 = 0;
    if (!isRaceModeMiniGame() && getConsoleNumber() >= 3)
        iVar4 = 1;

    TexLODControl *texLod = new TexLODControl(iVar4);
    texLod->setGeographyLODBiasOn(crsData->isTexLODBiasOn());
    bool staffRoll = isStaffRoll();

    SysDebug::getManager()->setHeapGroup("RACE DRAWER", nullptr);
    mRaceDrawer = new RaceDrawer(uVar20, staffRoll);
    SysDebug::getManager()->setHeapGroup("RACE BGM", nullptr);
    mRaceBGMPlayer = new RaceBGMPlayer();

    createConsole();
    stRandom::createAllRandom();
    setRandomSeed();

    ExMdlDataMgr *mdlData = new ExMdlDataMgr(isMirror());
    SysDebug::getManager()->setHeapGroup("SHADOW SCR", nullptr);
    JUtility::TColor color;
    crsData->getShadowColor(&color);
    ShadowManager::ptr()->setMirror(isMirror());
    ShadowManager::ptr()->setShadowColor(color);
    ShadowManager::ptr()->setShadowDepth(crsData->getShadowDepth());

    SysDebug::getManager()->setHeapGroup("COURSE MGR", nullptr);
    bool credits = false; // Kaneshige moment
    if (isStaffRoll())
        credits = true;

    mCourse = new Course(crsData, getCameraNumber(), credits);
    mAwardArc = nullptr;
    if (isAwardDemoMode())
    {
        mAwardArc = ResMgr::getArchive(ResMgr::mcArcAward);
        JUT_ASSERT_MSG(2185, mAwardArc, "NOT LOAD AWARD ARC");
    }
    SysDebug::getManager()->setHeapGroup("OBJECT MGR", nullptr);
    CreateGeoObjMgr(mCourse->getCrsData());
    CreateCharIKParamMgr();

    bool isTT = false;
    if (getRaceMode() == TIME_ATTACK)
        isTT = true;

    KartChecker::sBombPointFull = KartChecker::getWinBombPointForMenu(getKartNumber());

    for (int i = 0; i < 8; i++)
    {
        SysDebug::getManager()->setHeapGroup("KART WORK", nullptr);
        KartChecker *kartChker = nullptr;
        KartLoader *kartLdr = nullptr;
        if (i < getKartNumber())
        {
            kartChker = new KartChecker(i, getKartInfo(i), getCourse()->getTrackSectorNumber(), getTotalLapNumber());
            kartLdr = new KartLoader(i, getKartInfo(i), uVar20, isTT, isAwardDemoMode());
        }
        mKartChecker[i] = kartChker;
        mKartLoader[i] = kartLdr;
    }

    if (isAwardDemoMode())
        mKartLoader[0]->setDemoBodyBmd(ObjUtility::getPtrCourseArc("/Objects/demo_k_body.bmd"));

    SysDebug::getManager()->setHeapGroup("PARTICLE MGR", nullptr);
    CreateJPAMgr(mRaceHeap, false);
    CreateStEfctMgr();
    GetStEfctMgr()->init();
    CreateJ3DEfctMgr();
    CreateJ3DEfctKarAnmMgr();

    SysDebug::getManager()->setHeapGroup("ITEM MGR", nullptr);
    CreateItemObjMgr();

    SysDebug::getManager()->setHeapGroup("KART MGR", nullptr);
    KartCtrl::makeKartCtrl();
    KartCtrl::getKartCtrl()->DynamicsInit(ExModel::isMtxCombinationOn());
    createModel();
    CreateEfctScreenMgr();
    createLight();

    SysDebug::getManager()->setHeapGroup("MOTOR MGR", nullptr);
    new MotorManager(mRaceHeap);

    SysDebug::getManager()->setHeapGroup("2D DATA", nullptr);
    J2DManager *j2dMgr = new J2DManager(mRaceHeap);
    PauseManager *pauseMgr = new PauseManager(mRaceHeap);

    if (isAwardDemoMode())
        mAward2D = new Award2D(mRaceHeap);
    else
        mAward2D = nullptr;

    if (isStaffRoll())
    {
        SysDebug::getManager()->setHeapGroup("STAFF ROLL2D", nullptr);
        bool ending = mRaceInfo->isTrueEnding();
        mStaffRoll2D = new StaffRoll2D(mRaceHeap, lang, videoMode, ending);
        SysDebug::getManager()->setHeapGroup("2D DATA", nullptr);
    }
    else
        mStaffRoll2D = nullptr;

    if (isWaitDemoMode() || isAwardDemoMode() || isStaffRoll())
    {
        pauseMgr->setExecFlag(false);
        j2dMgr->setDrawFlag(false);
        if (isAwardDemoMode() || isStaffRoll())
        {
            GetGeoObjMgr()->getJugem(0)->signalGo();
            mAbleStart = true;
        }
    }

    SysDebug::getManager()->setHeapGroup("RACE MGR", nullptr);
    for (int i = 0; i < mRaceInfo->getConsoleNumber(); i++)
    {
        Console *cns = &mConsole[i];
        int cnsTgt = mRaceInfo->getConsoleTarget(i);
        bool isDemo = mRaceInfo->isDemoConsole(i);
        cns->changeTargetNo(cnsTgt, isDemo);
    }

    updateBestTime();
    mEvents = 0;
    for (const EventInfo *eventTable = sEventTable; eventTable->id != -1; eventTable++)
        mEvents++;

    mProctime1 = -1;
    mProctime2 = -1;
    mProctime3 = -1;
    mProctime4 = -1;

    SysDebug::getManager()->setHeapGroup("RESTART", nullptr); // these also might be a define, who would write SysDebug::getManager a million times?
    SysDebug::getManager()->setDefaultHeapGroup(nullptr);
    SYSDBG_SetUserTimeLabel(0, "GX ISSUE");
    SYSDBG_SetUserTimeLabel(1, nullptr);
    SYSDBG_SetUserTimeLabel(2, "CALC");
    SYSDBG_SetUserTimeLabel(3, nullptr);
    SYSDBG_SetUserTimeLabel(4, "UPDATE");
    SYSDBG_SetUserTimeLabel(5, nullptr);
    SYSDBG_SetUserTimeLabel(6, "VIEWCALC");
    SYSDBG_SetUserTimeLabel(7, nullptr);
}

void RaceMgr::updateBestTime()
{
    ECourseID courseID = RCMGetCourse()->getCourseID();
    if (gSystemRecord.getBestLap(courseID)->isValid())
        mBestLapTime.set(gSystemRecord.getBestLap(courseID)->getRaceTime());
    else
        mBestLapTime.reset();

    for (int i = 0; i < 5; i++)
    {
        if (gSystemRecord.getTARecord(courseID, i)->isValid())
            mBestTotalTimes[i].set(gSystemRecord.getTARecord(courseID, i)->getRaceTime());
        else
            mBestTotalTimes[i].reset();
    }
}

void RaceMgr::setRandomSeed()
{
    for (u32 i = 0; i < 6; i++)
    {
        stGetRnd(i)->setSeed(mRaceInfo->mRandomSeed);
    }
    GameAudio::Random::setSeed(mRaceInfo->mRandomSeed);
}

void RaceMgr::editRaceInfoForDebug()
{
    s16 startpointID = sMyStartPointID;
    if (startpointID >= 0)
    {
        RaceInfo *rInfo = mRaceInfo;
        s16 origStartPosIdx = rInfo->mStartPosIndex[0];
        for (int i = 0; i < 8; i++)
        {
            if (rInfo->mStartPosIndex[i] == startpointID)
            {
                rInfo->mStartPosIndex[i] = origStartPosIdx;
                break;
            }
        }
        mRaceInfo->mStartPosIndex[0] = sMyStartPointID;
    }
}

void RaceMgr::createConsole()
{
    int cnsCnt = mRaceInfo->getConsoleNumber();
    mConsole = new Console[cnsCnt];
    for (int i = 0; i < cnsCnt; i++)
    {
        Console *cns = &mConsole[i];
        cns->setConsoleNo(i);
        if (i == 0)
            cns->setDraw();
    }
    if (!isWaitDemoMode() && !isAwardDemoMode() && !isStaffRoll())
        return;
    resetConsole();
}

void RaceMgr::resetConsole()
{
    int cnsCnt = mRaceInfo->getConsoleNumber();
    for (int i = 0; i < cnsCnt; i++)
    {
        Console *cns = &mConsole[i];
        if (!mRaceInfo->isHiddingConsole(i))
            cns->setDraw();
        else
            cns->clrDraw();
    }
}

bool RaceMgr::hideConsole(u32 cnsNo)
{
    bool ret = false;
    int cnsCnt = mRaceInfo->getConsoleNumber();
    if (cnsNo < cnsCnt)
    {
        mConsole[cnsNo].clrDraw();
        ret = true;
    }
    return ret;
}

// even though UNUSED, its needed for data alignment
// its probably something close to this, currently it has a small size mismatch(not that it matters)
bool RaceMgr::isHiddingConsole(u32 viewNo)
{
    JUT_MINMAX_ASSERT(3969, 0, viewNo, mRaceInfo->getConsoleNumber());
    return mRaceInfo->isHiddingConsole(viewNo);
}

void RaceMgr::createModel()
{
    Mtx viewMtx;
    PSMTXIdentity(viewMtx);
    j3dSys.setViewMtx(viewMtx);
    SysDebug::getManager()->setHeapGroup("KART MODEL", nullptr);
    createKartModel();
    SysDebug::getManager()->setHeapGroup("COURSE MODEL", nullptr);
    createCourseModel();
    SysDebug::getManager()->setHeapGroup("OBJECT MODEL", nullptr);
    createObjectModel();
    SysDebug::getManager()->setHeapGroup("ITEM MODEL", nullptr);
    createItemModel();
    SysDebug::getManager()->setHeapGroup("EFFECT MODEL", nullptr);
    createEffectModel();
}

void RaceMgr::createKartModel()
{
    ShadowManager::ptr();
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
#if DEBUG
    SysDebug::getManager()->createHeapInfo(solidHeap, "KART MDL");
#endif
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();

    for (int i = 0; i < getKartNumber(); i++)
    {
        mKartLoader[i]->load();
        mKartLoader[i]->createModel(solidHeap, getCameraNumber());
        mRaceDrawer->getKartDrawer(i)->setAnimation();
    }

    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createCourseModel()
{
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
#if DEBUG
    SysDebug::getManager()->createHeapInfo(solidHeap, "CRS  MDL");
#endif
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    mCourse->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createObjectModel()
{
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
#if DEBUG
    SysDebug::getManager()->createHeapInfo(solidHeap, "OBJ  MDL");
#endif
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    GetGeoObjMgr()->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createItemModel()
{
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
#if DEBUG
    SysDebug::getManager()->createHeapInfo(solidHeap, "ITEM MDL");
#endif
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    GetItemObjMgr()->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createEffectModel()
{
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
#if DEBUG
    SysDebug::getManager()->createHeapInfo(solidHeap, "EFCT MDL");
#endif
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    GetStEfctMgr()->createModel(solidHeap, getCameraNumber());
    GetJ3DEfctMgr()->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createLight()
{
    SysDebug::getManager()->setHeapGroup("LIGHT MGR", nullptr);

    LightMgr::createManager();
    bool balloonExists = false;
    TBalloonString *balloonString = TBalloonString::getSupervisor();
    if (balloonString)
        balloonExists = 1;

    JUtility::TColor ambientColor;
    mCourse->getAmbientColor(&ambientColor);
    LtObjAmbient *lghtObjAmb = new LtObjAmbient("シーンアンビエント", ambientColor); // scene ambient?

    lghtObjAmb->setTagName('AMBI');
    LightMgr::getManager()->appendLight(lghtObjAmb);

    JUtility::TColor lightColor;
    mCourse->getLightColor(&lightColor);
    JGeometry::TVec3f lightPos;
    mCourse->getLightOffsetPosition(&lightPos);

    for (u32 i = 0; i < getConsoleNumber(); i++)
    {
        RaceSceneLight *sceneLight = new RaceSceneLight("シーンライト", i, lightColor, lightPos);
        LightMgr::getManager()->appendLight(sceneLight);
        for (int j = 0; j < getKartNumber(); j++)
        {
            RaceKartLight *kartLight = new RaceKartLight(sceneLight, j);
            LightMgr::getManager()->appendLight(kartLight);
            getKartDrawer(j)->setLight(i, kartLight);
        }
        if (balloonExists)
        {
            RaceBalloonLight *balloonLight = new RaceBalloonLight(i);
            LightMgr::getManager()->appendLight(balloonLight);
        }
        if (isAwardDemoMode())
        {
            RaceCupLight *cupLight = new RaceCupLight(sceneLight);
            LightMgr::getManager()->appendLight(cupLight);
        }
    }
}

void RaceMgr::resetRace()
{
    bool needHardReset = false;

    switch (mRaceDirector->getRacePhase())
    {
    case PHASE_3:
        resetRaceForResetEvent();
        needHardReset = true;
        break;
    case PHASE_4:
        resetRaceForRestartEvent();
        needHardReset = true;
        break;
    case PHASE_5:
        resetRaceForReplayEvent();
        needHardReset = true;
        break;
    }
    if (needHardReset)
        resetRaceCommon();
}

void RaceMgr::resetRaceForResetEvent()
{
    RCMLogMsg("Race    Start-------------------------------------------------------\n");
    updateBestTime();
}

void RaceMgr::resetRaceForRestartEvent()
{
    RCMLogMsg("Restart Start-------------------------------------------------------\n");
    if (isRaceModeMiniGame())
    {
        mRaceInfo->shuffleRandomSeed();
        mRaceInfo->shuffleStartNo();
    }

    mReplayMode &= 0xfffe;
    updateBestTime();
}

void RaceMgr::resetRaceForReplayEvent()
{
    RCMLogMsg("Replay  Start-------------------------------------------------------\n");
    mReplayMode |= 1;
}

void RaceMgr::resetRaceCommon()
{
    RCMLogMsg("Begin Reset.............................................\n");
    RCMLogMsg("Rand Seed:%08X\n", mRaceInfo->mRandomSeed);

    GameAudio::Main::getAudio()->initRaceSound();
    mFrame = 0;
    mRaceDirector->reset();
    setRandomSeed();
    mRaceBGMPlayer->reset();
    for (u32 i = 0; i < getCameraNumber(); i++)
    {
        setJugemZClr(i, true);
    }
    resetConsole();
    SysDebug::getManager()->setHeapGroup("RESTART", nullptr);
    mCourse->reset();
    GetGeoObjMgr()->reset(*mCourse->getCrsData());
    GetItemObjMgr()->reset();

    for (int i = 0; i < getKartNumber(); i++)
    {
        mKartChecker[i]->reset();
        mKartLoader[i]->reset();
    }

    KartCtrl::getKartCtrl()->DynamicsReset();
    GetStEfctMgr()->reset();
    GetJ3DEfctMgr()->reset();
    GetEfctScreenMgr()->reset();
    MotorManager::getManager()->reset();
    PauseManager::getManager()->reset();

    if (!isWaitDemoMode() && !isAwardDemoMode() && !isStaffRoll())
    {
        PauseManager::getManager()->enablePause();
    }

    J2DManager::getManager()->reset();
    J2DManager::getManager()->startLANNumAnm();
    if (mAward2D != nullptr)
        mAward2D->still();
    if (mStaffRoll2D != nullptr)
        mStaffRoll2D->reset();

    GetJPAMgr()->reset();
    mRaceDrawer->reset();
    if (!isRaceModeMiniGame())
        RivalKart::Reset();

    SysDebug::getManager()->setDefaultHeapGroup(nullptr);
    RCMLogMsg("End   Reset.............................................\n");
}

int RaceMgr::getCurrentConsoleNumber()
{
    int ret = getConsoleNumber();
    if (isCrsDemoMode())
        ret = 1;

    return ret;
}

void RaceMgr::drawRace()
{
    SYSDBG_BeginUserTime(0);
    mRaceDrawer->drawPreScene();
    for (u32 i = 0; i < getCameraNumber(); i++)
    {
        if (mConsole[i].isDraw())
            mRaceDrawer->drawSceneFirstStage(i);
    }
    mRaceDrawer->drawMidScene();

    for (u32 i = 0; i < getCameraNumber(); i++)
    {
        if (mConsole[i].isDraw())
            mRaceDrawer->drawSceneSecondStage(i);
    }

    mRaceDrawer->drawPostScene();
    SYSDBG_EndUserTime(0);
}

void RaceMgr::checkKart()
{

    int goalKartNo = -1;
    int finalLapKartNo = -1;
    int lowestKartNo = -1;

    mRaceDirector->isFrameRenewal();
    for (int i = 0; i < getKartNumber(); i++)
    {
        KartChecker *kartChecker = mKartChecker[i];
        kartChecker->checkKart();
        if (isAbleStart())
            kartChecker->incTime();

        kartChecker->checkLap(isRaceEnd());

        if (!kartChecker->isGoal())
        {
            if (kartChecker->isFinalLap() &&
                getKartInfo(i)->isPlayerKart() && !getKartInfo(i)->isGhostKart())
            {
                finalLapKartNo = i;
            }
            if (isRaceEnd() && getKartInfo(i)->isRealPlayerKart() &&
                !getKartInfo(i)->isGhostKart())
            {
                lowestKartNo = i;
            }
        }
        else
        {
            if (getKartInfo(i)->isRealPlayerKart() && !getKartInfo(i)->isGhostKart())
            {
                goalKartNo = i;
            }
        }
    }
    if (finalLapKartNo >= 0)
        mRaceBGMPlayer->setFinalLapKartNo(finalLapKartNo);
    if (goalKartNo >= 0)
        mRaceBGMPlayer->setGoalKartNo(goalKartNo);
    if (lowestKartNo >= 0)
        mRaceBGMPlayer->setLowestKartNo(lowestKartNo);
}

void RaceMgr::checkRank()
{
    for (int i = 0; i < getKartNumber(); i++)
        mKartChecker[i]->clrRank();

    switch (getRaceMode())
    {
    case BALLOON_BATTLE:
        checkRankForBalloonBattle();
        break;
    case ROBBERY_BATTLE:
        checkRankForRobberyBattle();
        break;
    case BOMB_BATTLE:
        checkRankForBombBattle();
        break;
    case ESCAPE_BATTLE:
        checkRankForEscapeBattle();
        break;
    case AWARD_DEMO:
        checkRankForAwardDemo();
        break;
    default:
        checkRankForRace();
        break;
    }
}

// TODO: rename
void RaceMgr::checkRankForBalloonBattle()
{
    int i = 1;
    int lowhalf = getKartNumber();
    do
    {
        KartChecker *aliveKart = nullptr;
        int balloonNum = -1;
        KartChecker *deadKart = nullptr;
        RaceTime lastDeathTime;

        for (int j = 0; j < getKartNumber(); j++)
        {
            KartChecker *kartChecker3 = getKartChecker(j);
            if (!kartChecker3->isRankAvailable())
            {
                if (kartChecker3->getBalloonNumber() != 0)
                {
                    if (kartChecker3->getBalloonNumber() > balloonNum)
                    {
                        aliveKart = kartChecker3;
                        balloonNum = kartChecker3->getBalloonNumber();
                    }
                }
                else
                {
                    bool isDead = false;
                    if (kartChecker3->getDeathTime().isLittle(lastDeathTime))
                    {
                        isDead = true;
                    }
                    else
                    {
                        if (kartChecker3->getDeathTime().get() == lastDeathTime.get() &&
                            kartChecker3 > deadKart) // kaneshige moment?
                            isDead = true;
                    }
                    if (isDead)
                    {
                        deadKart = kartChecker3;
                        lastDeathTime.set(kartChecker3->getDeathTime());
                    }
                }
            }
        }

        if (aliveKart)
        {
            aliveKart->setRank(i);
            i++;
        }

        if (deadKart)
        {
            deadKart->setRank(lowhalf);
            lowhalf--;
        }

    } while (i <= lowhalf);
}

// this is a robbery dun dun dun dun
void RaceMgr::checkRankForRobberyBattle()
{
    int rank = 1;
    int kartNo = getKartNumber();
    do
    {
        int itemNum = -1;
        for (int i = 0; i < getKartNumber(); i++)
        {
            KartChecker *kartChecker = getKartChecker(i);
            if (!kartChecker->isRankAvailable())
            {
                int robberyItemNum = kartChecker->getRobberyItemNumber();
                if (robberyItemNum > itemNum)
                {
                    itemNum = robberyItemNum;
                }
            }
        }
        int increment = 0;
        for (int i = 0; i < getKartNumber(); i++)
        {
            KartChecker *kartChecker = getKartChecker(i);
            if (!kartChecker->isRankAvailable() && kartChecker->getRobberyItemNumber() == itemNum)
            {
                kartChecker->setRank(rank);
                increment++;
            }
        }
        rank += increment;
    } while (rank <= kartNo);
}

void RaceMgr::checkRankForBombBattle()
{
    int rank = 1;
    int kartNo = getKartNumber();
    do
    {
        KartChecker *kartChecker1 = nullptr;
        int lastPoint = 1000;
        KartChecker *kartChecker2 = nullptr;
        RaceTime lastMarkTime;
        for (int i = 0; i < getKartNumber(); i++)
        {
            KartChecker *kartChecker3 = getKartChecker(i);
            if (!kartChecker3->isRankAvailable())
            {
                if (kartChecker3->isBombPointFull())
                {
                    bool setMarkTime = false;
                    if (kartChecker3->getMarkTime().get() < lastMarkTime.get())
                    {
                        setMarkTime = true;
                    }
                    else
                    {
                        if ((kartChecker3->getMarkTime().get() == lastMarkTime.get()) && (kartChecker3 < kartChecker2))
                        {
                            setMarkTime = true;
                        }
                    }
                    if (setMarkTime)
                    {
                        kartChecker2 = kartChecker3;
                        lastMarkTime.set(kartChecker3->getMarkTime());
                    }
                }
                else
                {
                    bool setPoint = false;
                    if (kartChecker3->getBombPoint() < lastPoint)
                    {
                        setPoint = true;
                    }
                    else
                    {
                        if (kartChecker3->getBombPoint() == lastPoint && kartChecker3 > kartChecker2)
                        {
                            setPoint = true;
                        }
                    }
                    if (setPoint)
                    {
                        kartChecker1 = kartChecker3;
                        lastPoint = kartChecker3->getBombPoint();
                    }
                }
            }
        }

        if (kartChecker2)
        {
            kartChecker2->setRank(rank);
            rank++;
        }
        if (kartChecker1)
        {
            kartChecker1->setRank(kartNo);
            kartNo--;
        }
    } while (rank <= kartNo);
}

void RaceMgr::checkRankForEscapeBattle()
{
    int rabbitKartNo = GeoRabbitMark::getSupervisor()->getRabbitKartNo(); // rabbit? Gamecube uses PowerPC
    int rank = 1;
    int kartNo = getKartNumber();

    do
    {
        KartChecker *kartChecker1 = nullptr;
        KartChecker *kartChecker2 = nullptr;
        int idk = -1;
        for (int i = 0; i < getKartNumber(); i++)
        {
            KartChecker *kartChecker3 = getKartChecker(i);
            if (!kartChecker3->isRankAvailable())
            {
                if (i == rabbitKartNo)
                {
                    kartChecker1 = kartChecker3;
                }
                else
                {
                    bool higher = false;
                    if (i > idk)
                    {
                        higher = true;
                    }
                    if (higher)
                    {
                        kartChecker2 = kartChecker3;
                        idk = i;
                    }
                }
            }
        }

        if (kartChecker1)
        {
            kartChecker1->setRank(rank);
            rank++;
        }

        if (kartChecker2)
        {
            kartChecker2->setRank(kartNo);
            kartNo--;
        }
    } while (rank <= kartNo);
}

void RaceMgr::checkRankForAwardDemo()
{
    for (int i = 0; i < getKartNumber(); i++)
        getKartChecker(i)->setRank(i + 1);
}

void RaceMgr::checkRankForRace()
{
    int i = 1;
    int lowhalf = getKartNumber();
    do
    {
        KartChecker *kartChecker1 = nullptr;
        RaceTime lastRaceTime;
        f32 lastDist = 10000.0f;
        KartChecker *kartChecker2 = nullptr;

        for (int j = 0; j < getKartNumber(); j++)
        {
            KartChecker *kartChecker3 = getKartChecker(j);
            if (!kartChecker3->isRankAvailable())
            {
                if (kartChecker3->isGoal())
                {
                    bool isFaster = false;
                    if (kartChecker3->getTotalTime().isAvailable())
                    {
                        if (kartChecker3->getTotalTime().isLittle(lastRaceTime))
                        {
                            isFaster = true;
                        }
                    }
                    else
                    {
                        if (!kartChecker1 || (kartChecker3->getGoalFrame() < kartChecker1->getGoalFrame()))
                        {
                            isFaster = true;
                        }
                    }
                    if (isFaster)
                    {
                        kartChecker1 = kartChecker3;
                        lastRaceTime.set(kartChecker3->getTotalTime());
                    }
                }
                else
                {
                    f32 totalDist = kartChecker3->getTotalUnitDist();
                    if (totalDist < lastDist)
                    {
                        kartChecker2 = kartChecker3;
                        lastDist = totalDist;
                    }
                }
            }
        }

        if (kartChecker1)
        {
            kartChecker1->setRank(i);
            i++;
        }

        if (kartChecker2)
        {
            kartChecker2->setRank(lowhalf);
            lowhalf--;
        }

    } while (i <= lowhalf);
}

void RaceMgr::setRaceResult()
{
    for (int i = 0; i < getKartNumber(); i++)
    {
        mRaceInfo->mRank[i] = getKartChecker(i)->getRank();
        mRaceInfo->mFinishTime[i] = getKartChecker(i)->getTotalTime();
        // Store Lap times
        for (int j = 0; j < getTotalLapNumber(); j++)
            mRaceInfo->mLapTimes[i][j] = getKartChecker(i)->getLapTime(j);
    }
    mRaceInfo->_0x298 = mRaceDirector->getWaitDemoResult();
}

void RaceMgr::calcRace()
{
    SYSDBG_BeginUserTime(2); // start timer to calculate duration
    mRaceDirector->calc();
    MotorManager::getManager()->exec();

    beginProcTime(0x100);
    J2DManager::getManager()->calc();
    GetItemObjMgr()->clrKartItemUseTriggerList();
    if (mAward2D)
        mAward2D->calc();
    if (mStaffRoll2D)
        mStaffRoll2D->calc();
    endProcTime(0x100);

    if (mRaceDirector->isFrameRenewal())
    {
        frameWork();
        mFrame++;
    }
    checkRank();
    _0x22++;
    SYSDBG_EndUserTime(2);
#if DEBUG
    if (gGamePad1P.testButton(JUTGamePad::Z) && gGamePad1P.testTrigger(JUTGamePad::DPAD_DOWN))
    {
        sDispFrameCounter ^= 1;
    }

    if (sDispFrameCounter)
    {
        u32 frame = getPadRecorderFrame();
        if (RaceApp::ptr())
        {
            if (RaceApp::ptr()->getPadRecorder() == nullptr)
                frame = mFrame;
        }
        JUTReport(300, 430, "FRAME:%8d", frame);
    }
#endif
}

void RaceMgr::frameWork()
{ // Maybe there were some sort of wrappers for proctime?
    mCourse->calcShaking();

    beginProcTime(0x101);
    KartCtrl::getKartCtrl()->DynamicsStage1();
    endProcTime(0x101);

    beginProcTime(0x102);
    KartCtrl::getKartCtrl()->DynamicsStage2();
    endProcTime(0x102);

    J2DManager::getManager()->calcParticle();
    LightMgr::getManager()->calc();

    beginProcTime(0x103);
    mCourse->calc();
    endProcTime(0x103);

    beginProcTime(0x105);
    GetGeoObjMgr()->calc();
    endProcTime(0x105);

    beginProcTime(0x109);
    checkKart();
    endProcTime(0x109);

    mRaceBGMPlayer->calc();
    GetJPAMgr()->calc_forMenu();
}

void RaceMgr::updateRace()
{
    SYSDBG_BeginUserTime(4);
    if (mRaceDirector->isFrameRenewal())
    {
        beginProcTime(0x201);
        getCourse()->update();
        getCourse()->updateSky();
        mRaceDrawer->getShadowGeoDrawBuffer()->update();
        mRaceDrawer->getGeoDrawBuffer()->update();
        mRaceDrawer->getJugemDrawBuffer()->update();
        endProcTime(0x201);

        beginProcTime(0x203);
        for (int i = 0; i < getKartNumber(); i++)
            mRaceDrawer->getKartDrawer(i)->update();
        mRaceDrawer->getFeelDrawBuffer()->update();
        AccessoryMgr::ptr()->update();
        endProcTime(0x203);

        beginProcTime(0x106);
        GetItemObjMgr()->calc();
        endProcTime(0x106);

        beginProcTime(0x202);
        mRaceDrawer->getItemDrawBuffer(-1)->update();
        if (isActiveAreaLight())
        {
            for (int j = 0; j < getKartNumber(); j++)
            {
                mRaceDrawer->getItemDrawBuffer(j)->update();
            }
        }
        endProcTime(0x202);

        beginProcTime(0x204);
        ShadowManager::ptr()->calc();
        endProcTime(0x204);

        beginProcTime(0x107);
        GetStEfctMgr()->calc();
        GetJPAMgr()->calc();
        GetJ3DEfctMgr()->calc();
        endProcTime(0x107);

        beginProcTime(0x108);
        GetEfctScreenMgr()->calc();
        endProcTime(0x108);

        beginProcTime(0x205);
        mRaceDrawer->getEffectDrawBuffer()->update();
        endProcTime(0x205);
    }
    SYSDBG_EndUserTime(4);

    SYSDBG_BeginUserTime(6);
    ;
    for (u32 i = 0; i < getCameraNumber(); i++)
    {
        j3dSys.setViewMtx(getCamera(i)->GetMtx());

        beginProcTime(0x301);
        mRaceDrawer->getShadowGeoDrawBuffer()->viewCalc(i);
        mRaceDrawer->getGeoDrawBuffer()->viewCalc(i);
        mRaceDrawer->getJugemDrawBuffer()->viewCalc(i);
        endProcTime(0x301);

        beginProcTime(0x302);
        mRaceDrawer->getItemDrawBuffer(-1)->viewCalc(i);
        for (int j = 0; j < getKartNumber(); j++)
        {
            mRaceDrawer->getItemDrawBuffer(j)->viewCalc(i);
        }
        endProcTime(0x302);

        beginProcTime(0x304);
        ShadowManager::ptr()->viewCalc(i);
        endProcTime(0x304);

        beginProcTime(0x305);
        mRaceDrawer->getEffectDrawBuffer()->viewCalc(i);
        endProcTime(0x305);
    }
    SYSDBG_EndUserTime(6);
}

RaceMgr::~RaceMgr()
{
    if (GameAudio::Parameters::getDemoMode())
        GameAudio::Parameters::setDemoMode(0);

    for (JSUTreeIterator<JKRHeap> iterator = mRaceHeap->getHeapTree().getFirstChild(); iterator != mRaceHeap->getHeapTree().getEndChild(); ++iterator)
    {
        SysDebug::getManager()->destroyHeapInfo(iterator.getObject());
    }
    SysDebug::getManager()->clrAllUserTimeLabel();
    sRaceManager = nullptr;
}

bool RaceMgr::isRaceModeMiniGame()
{
    bool isMiniGame = false;
    switch (getRaceMode())
    {
    case BALLOON_BATTLE:
    case ROBBERY_BATTLE:
    case BOMB_BATTLE:
    case ESCAPE_BATTLE:
        isMiniGame = true;
    }
    return isMiniGame;
}

ERacePhase RaceMgr::getRacePhase()
{
    return mRaceDirector->getRacePhase();
}

const RaceTime &RaceMgr::getMiniGameTime()
{
    return mRaceDirector->getMiniGameTime();
}

int RaceMgr::searchNRankKart(int rank)
{
    int ret = -1;
    for (int i = 0; i < getKartNumber(); i++)
    {
        if (getKartChecker(i)->getRank() == rank)
        {
            ret = i;
            break;
        }
    }
    return ret;
}

bool RaceMgr::isAbleStart() const
{
    bool ret;
    if (mAbleStart)
        ret = true;
    else
        ret = GetGeoObjMgr()->getJugem(0)->isAbleStart();

    return ret;
}

void RaceMgr::setJugemZClr(u32 viewNo, bool clear)
{
    JUT_MINMAX_ASSERT(4353, 0, viewNo, mRaceInfo->getConsoleNumber());
    Console *console = &mConsole[viewNo];
    if (clear)
        console->clrJugemZClr();
    else
        console->setJugemZClr();
}

u8 RaceMgr::getStartID(int startIndex)
{
    u8 id = 0xff;
    switch (getRaceMode())
    {
    case BALLOON_BATTLE:
    case ROBBERY_BATTLE:
    case BOMB_BATTLE:
    case ESCAPE_BATTLE:
    case AWARD_DEMO:
    case STAFF_ROLL:
        id = startIndex;
        break;
    }
    return id;
}

bool RaceMgr::getStartPoint(JGeometry::TVec3f *position, JGeometry::TVec3f *direction, int kartNo)
{
    bool isRight = false;
    position->set(0.0f, 0.0f, 0.0f); // .zero exists
    direction->set(0.0f, 0.0f, 1.0f);

    JUT_MINMAX_ASSERT(4396, 0, kartNo, getKartNumber());
    int startPosIdx = mRaceInfo->mStartPosIndex[kartNo];
    u8 startID = getStartID(startPosIdx);
    int tindex = -1;
    switch (getRaceMode())
    {
    case TIME_ATTACK:
        tindex = 0;
        break;
    case VERSUS_RACE:
        int tmp;
        if (getKartNumber() == 2)
            tmp = 9;
        else if (getKartNumber() == 3)
            tmp = 11;
        else if (getKartNumber() == 4)
            tmp = 14;
        else
            tmp = 1;
        tindex = tmp + startPosIdx;
        break;
    case GRAND_PRIX:
        tindex = startPosIdx + 1;
        break;
    case BALLOON_BATTLE:
    case ROBBERY_BATTLE:
    case BOMB_BATTLE:
    case ESCAPE_BATTLE:
        break;
    }
    if (getCourse()->getCrsData())
    {
        CrsData::StartPoint *startPoint = getCourse()->getCrsData()->getStartPoint(startID);
        JUT_ASSERT_F(4437, startPoint, "NOT FOUND START:%d", startID);
        isRight = startPoint->isRight();
        startPoint->getPosition(position);
        startPoint->getFrDirection(direction);
        if (tindex >= 0)
        {

            float startPosTable[][3] = {// might be TVec3, however that didn't work
                {0.0f, 0.0f, 250.0f},
                {600.0f, 0.0f, 250.0f},
                {267.0f, 0.0f, 0.0f},
                {-67.0f, 0.0f, -250.0f},
                {-400.0f, 0.0f, -500.0f},
                {450.0f, 0.0f, -600.0f},
                {117.0f, 0.0f, -850.0f},
                {-217.0f, 0.0f, -1100.0f},
                {-550.0f, 0.0f, 1350.0f},
                {250.0f, 0.0f, 250.0f},
                {-250.0f, 0.0f, 250.0f},
                {500.0f, 0.0f, 250.0f},
                {0.0f, 0.0f, 250.0f},
                {-500.0f, 0.0f, 250.0f},
                {500.0f, 0.0f, 250.0f},
                {167.0f, 0.0f, 250.0f},
                {-167.0f, 0.0f, 250.0f},
                {-500.0f, 0.0f, 250.0f}};
            JGeometry::TVec3f startPos;
            JUT_MINMAX_ASSERT(4480, 0, tindex, 18);
            startPos.set(startPosTable[tindex][0], 0.0f, startPosTable[tindex][2]);

            if (startPoint->isRight())
            {
                startPos.x = -startPos.x;
            }

            JGeometry::TVec3f scaledDirection(*direction);
            scaledDirection.scale(startPos.z);
            JGeometry::TVec3f yScale(0.0f, 1.0f, 0.0f);
            JGeometry::TVec3f scaledPosition;
            scaledPosition.cross(yScale, *direction);
            scaledPosition.normalize();
            scaledPosition.scale(startPos.x);
            position->add(scaledDirection);
            position->add(scaledPosition);
        }
    }
    return isRight;
}

f32 RaceMgr::getStartJugemOffsetY(int kartNo)
{
    f32 y = 0.0f;
    JUT_MINMAX_ASSERT(4507, 0, kartNo, getKartNumber())
    u8 startID = getStartID(mRaceInfo->mStartPosIndex[kartNo]);
    if (getCourse()->getCrsData())
        y = getCourse()->getCrsData()->getStartPoint(startID)->getJugemOffsetY();

    return y;
}

int RaceMgr::getProcLevel()
{
    int procLevel = 0;
    if (getConsoleNumber() >= 2 || isLANMode())
        procLevel = 1;
    return procLevel;
}

bool RaceMgr::isItemBoxValid()
{
    bool valid = true;
    switch (getRaceMode())
    {
    case TIME_ATTACK:
        valid = false;
        break;
    case GRAND_PRIX:
        break;
    case VERSUS_RACE:
    {
        if (mRaceInfo->getItemSlotType() == 1)
            valid = false;
        break;
    }
    }
    return valid;
}

void RaceMgr::beginProcTime(short id)
{
#if DEBUG
    s16 idx = -1; // not sure how to name this, one day i'll open up dolphin and use the debugger
    s16 i = (id >> 8) & 0xff;

    switch (i)
    {
    case 0:
        idx = mProctime1;
        break;
    case 1:
        idx = mProctime2;
        break;
    case 2:
        idx = mProctime3;
        break;
    case 3:
        idx = mProctime4;
        break;
    }

    if (id == idx)
    {
        SYSDBG_SetUserTimeLabel((s16)((i << 1) + 1), searchEventInfo(id)->engName);
        SYSDBG_BeginUserTime((s16)((i << 1) + 1));
    }
#endif
}

void RaceMgr::endProcTime(short id)
{
#if DEBUG
    s16 idx = -1; // not sure how to name this, one day i'll open up dolphin and use the debugger
    s16 i = (id >> 8) & 0xff;

    switch (i)
    {
    case 0:
        idx = mProctime1;
        break;
    case 1:
        idx = mProctime2;
        break;
    case 2:
        idx = mProctime3;
        break;
    case 3:
        idx = mProctime4;
        break;
    }
    if (id == idx)
        SYSDBG_EndUserTime((s16)((i << 1) + 1));
#endif
}

const RaceMgr::EventInfo *RaceMgr::searchEventInfo(short searchId)
{
    const EventInfo *ret = nullptr;
    for (const EventInfo *eventTable = sEventTable; eventTable->id != -1; eventTable++)
    {
        if (eventTable->id == searchId)
        {
            ret = eventTable;
            break;
        }
    }
    return ret;
}

bool RaceMgr::isJugemCountStart()
{
    bool ret;
    if (mAbleStart)
        ret = true;
    else
        ret = GetGeoObjMgr()->getJugem(0)->isCallThree();

    return ret;
}

bool RaceMgr::isKartGoal(int idx)
{
    return getKartChecker(idx)->isGoal();
}

int RaceMgr::getGoalKartNumber()
{
    int goalKartNum = 0;
    for (int i = 0; i < getKartNumber(); i++)
    {
        if (getKartChecker(i)->isGoal())
            goalKartNum++;
    }
    return goalKartNum;
}

u32 RaceMgr::getPadRecorderFrame()
{
    u32 frame = 0;
    if (RaceApp::ptr())
    {
        if (RaceApp::ptr()->getPadRecorder())
            frame = RaceApp::ptr()->getPadRecorder()->getFrame();
    }

    return frame;
}

int RaceMgr::getTotalLapNumberForDisplay() const
{
    return getTotalLapNumber();
}

RaceMgr::Console::Console()
{
    mCnsNo = -1;
    mTargetNo = -1;
    _04 = false;
    mFlags = 0;
    mFlags |= 8;
}

void RaceMgr::Console::changeTargetNo(int targetNo, bool p2)
{
    if (isValid())
    {
        mTargetNo = targetNo;
        _04 = p2;
        RCMGetCamera(mCnsNo)->SetTargetNum(mTargetNo);
        if (!isNoStat())
            J2DManager::getManager()->setStatus2Kart(mCnsNo, mTargetNo);
    }
}

// these functions are unused so i'm just putting functions in here that need to get generated
void RaceMgr::Console::clearZBuffer()
{
    System::getDisplay();
}

bool RaceMgr::Console::isZoom()
{
    RCMGetKartChecker(0);
    f32 wdth = KartCtrl::getKartCtrl()->getKartCam(0)->GetWidth(); // these 2 are most likely used in a different function however i'm lazy
    f32 ht = KartCtrl::getKartCtrl()->getKartCam(0)->GetHeight();
}

bool RaceMgr::robRivalOfBalloon(int playerIdx, int rivalIdx)
{
    bool robbed = false;
    TBalloonManager *balloonMgr = GetGeoObjMgr()->getBalloonMgr(playerIdx);
    if (getKartChecker(playerIdx)->getBalloonNumber() <= 0)
    {
        return false;
    }
    else
    {
        bool rivalDecreased = getKartChecker(rivalIdx)->decBalloon();
        bool increased = false;
        if (rivalDecreased)
        {
            increased = getKartChecker(playerIdx)->incBalloon();
        }
        if (balloonMgr && increased && rivalDecreased)
        {
            balloonMgr->robRivalOfBalloon(rivalIdx);
            robbed = true;
        }
    }
    return robbed;
}

bool RaceMgr::robRivalOfRabbitMark(int playerIdx, int rivalIdx)
{
    bool robbed = false;
    GeoRabbitMarkSupervisor *rabbitMark = GeoRabbitMark::getSupervisor();
    if (rabbitMark && !isRaceEnd() && rabbitMark->getRabbitKartNo() == rivalIdx)
    {
        rabbitMark->changeOwner(playerIdx);
        robbed = true;
    }

    return robbed;
}

void RaceUsrPage::draw()
{
    for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++)
    {
        KartInfo *kartInfo = RaceMgr::getManager()->getKartInfo(i);
        char *kind = nullptr;
        switch (kartInfo->getPlayerKind(0))
        {
        case 0:
            kind = "NONE ";
            break;
        case 1:
            kind = "COM  ";
            break;
        case 2:
            kind = "MAN  ";
            break;
        case 3:
            kind = "NET  ";
            break;
        case 4:
            kind = "GHOST";
            break;
        }
        KartGamePad *kartPad = kartInfo->getPad(0);
        char *type = "NULL  ";
        char *port = "NULL  ";

        if (kartPad)
        {
            switch (kartPad->getPadType())
            {
            case KartGamePad::NORMAL:
                type = "NORMAL ";
                break;
            case KartGamePad::NETWORK:
                type = "NETWORK";
                break;
            case KartGamePad::RECORD:
                type = "RECORD ";
                break;
            }
            switch (kartPad->getPadPort())
            {
            case KartGamePad::PORT_1:
                port = "PORT1";
                break;
            case KartGamePad::PORT_2:
                port = "PORT2";
                break;
            case KartGamePad::PORT_3:
                port = "PORT3";
                break;
            case KartGamePad::PORT_4:
                port = "PORT4";
                break;
            case KartGamePad::PORT_INV:
                port = "NO   ";
                break;
            case KartGamePad::PORT_NET:
                port = "NET  ";
                break;
            }
        }
        JUTReport(40, (i * 40) + 60, "No.%1d:KIND %s", i, kind);
        JUTReport(40, (i * 40) + 80, "PAD :%08x:TYPE %s: PORT %s", kartPad, type, port);
    }
}
