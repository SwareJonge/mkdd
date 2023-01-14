
#include "kartLocale.h"
#include "Dolphin/OS.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "Kameda/J2DManager.h"
#include "Kameda/PauseManager.h"
#include "Kameda/MotorManager.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/Course/CrsData.h"
#include "Kaneshige/ExModel.h"
#include "Kaneshige/DemoTimeKeeper.h"
#include "Kaneshige/SysDebug.h"
#include "Kaneshige/TexLODControl.h"
#include "Kaneshige/RaceMgr.h"
#include "Osako/kartPad.h"
#include "Osako/ResMgr.h"
#include "Osako/SystemRecord.h"
#include "Osako/shadowMgr.h"
#include "Sato/EffectScreen.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/J3DEffectMgr.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/NeckCtrl.h"
#include "Sato/ObjUtility.h"
#include "Sato/stEffectMgr.h"
#include "Sato/stMath.h"

RaceMgr *RaceMgr::sRaceManager;

short RaceMgr::sForceTotalLapNum;
short RaceMgr::sDispFrameCounter;

RaceMgr::EventInfo RaceMgr::sEventTable[] = {
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
    {-1, "なし", nullptr}
};

RaceMgr::RaceMgr(RaceInfo *raceInfo) : 
    mRaceInfo(nullptr),                                       
    mCourse(nullptr),
    mBestLapTime() {

    Language lang = KartLocale::getLanguage();
    VideoMode videoMode = KartLocale::getVideoMode();
    OSReport("Demo    Start-------------------------------------------------------\n");
    OSReport("Race Mode:%08X\n", raceInfo->getRaceMode());
    OSReport("Stat Num :%08X\n", raceInfo->getStatusNumber());
    OSReport("Rand Seed: \n", raceInfo->mRandomSeed);

    sRaceManager = this;

    SysDebug::getManager()->setHeapGroup("RACE MGR", nullptr);
    _0x22 = 0;
    mFrame = 0;
    mRaceHeap = JKRGetCurrentHeap();
    mReplayMode = 0;
    mRaceInfo = raceInfo;

    editRaceInfoForDebug();
    RaceUsrPage *racePage = new RaceUsrPage(mRaceInfo);
    SysDebug::getManager()->appendPage(racePage);

    _0x30 = 0; // might've been used for debug only, this is the only time it gets used
    mAbleStart = false;
    mAreaLight = 0;

    if (getProcLevel() <= 0)
        activeAreaLight();

    bool bVar17 = false;
    if (!isWaitDemoMode()) {
        switch (getRaceMode()) {
        case GRAND_PRIX:
        case BALLOON_BATTLE:
        case ROBBERY_BATTLE:
        case BOMB_BATTLE:
        case ESCAPE_BATTLE:
            bVar17 = true;
            break;
        }
    }

    DemoTimeKeeper *demoKeeper = nullptr;
    if (isAwardDemoMode())
        demoKeeper = new AwardDemoTimeKeeper();
    else if (isStaffRoll())
        demoKeeper = new StaffRollTimeKeeper();

    mRaceDirector = new RaceDirector(bVar17, demoKeeper, getConsoleNumber());

    u16 uVar20 = 1;
    if (mRaceInfo->isDriverLODOn())
        uVar20 = 2;

    SysDebug::getManager()->setHeapGroup("COURSE MGR", nullptr);
    
    // this doesn't seem correct
    CrsData::SColHeader *bco = reinterpret_cast<CrsData::SColHeader *>(ResMgr::getPtr(ResMgr::COURSE_BCO)); 
    CrsData::SOblHeader *bol = reinterpret_cast<CrsData::SOblHeader *>(ResMgr::getPtr(ResMgr::COURSE_BOL));
    CrsData *crsData = new CrsData(bco, bol);

    if (isStaffRoll())
        crsData->patchCamDataForStaffRoll(lang, videoMode);

    OSReport("Course ID:%08X\n", crsData->getCourseID());

    for (int i = 0; i < 16; i++) {
        KartGamePad *gamepad = gpaKartPad[i];
        if (gamepad->testButton(0x10)) {
            if (gamepad->testButton(0x800))
                sForceTotalLapNum = 1;
            else if (gamepad->testButton(0x400))
                sForceTotalLapNum = 2;
        }
        if (sForceTotalLapNum != 0)
            break;
    }

    if (sForceTotalLapNum == 0) {
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
    if (isAwardDemoMode()) {
        mAwardArc = ResMgr::getArchive(ResMgr::mcArcAward);
        if (mAwardArc == nullptr)
            JUT_PANIC(2185, "NOT LOAD AWARD ARC"); // if there's a regswap, here's the issue
    }
    SysDebug::getManager()->setHeapGroup("OBJECT MGR", nullptr);
    CreateGeoObjMgr(mCourse->getCrsData());
    CreateCharIKParamMgr();

    bool isTT = false;
    if (getRaceMode() == TIME_ATTACK)
        isTT = true;

    KartChecker::sBombPointFull = KartChecker::getWinBombPointForMenu(getKartNumber());

    for (int i = 0; i < 8; i++) {
        SysDebug::getManager()->setHeapGroup("KART WORK", nullptr);
        KartChecker *kartChker = nullptr;
        KartLoader *kartLdr = nullptr;
        if (i < getKartNumber()) {
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

    if (isStaffRoll()) {
        SysDebug::getManager()->setHeapGroup("STAFF ROLL2D", nullptr);
        bool ending = mRaceInfo->isTrueEnding();
        mStaffRoll2D = new StaffRoll2D(mRaceHeap, lang, videoMode, ending);
        SysDebug::getManager()->setHeapGroup("2D DATA", nullptr);
    }
    else
        mStaffRoll2D = nullptr;

    if (isWaitDemoMode() || isAwardDemoMode() || isStaffRoll()) {
        pauseMgr->setExecFlag(false);
        j2dMgr->setDrawFlag(false);
        if (isAwardDemoMode() || isStaffRoll()) {
            GetGeoObjMgr()->getJugem(0)->signalGo();
            mAbleStart = true;
        }
    }

    SysDebug::getManager()->setHeapGroup("RACE MGR", nullptr);
    for (int i = 0; i < mRaceInfo->getConsoleNumber(); i++) {
        Console *cns = &mConsole[i];
        int cnsTgt = mRaceInfo->getConsoleTarget(i);
        bool isDemo = mRaceInfo->isDemoConsole(i);
        cns->changeTargetNo(cnsTgt, isDemo);
    }

    updateBestTime();
    mEvents = 0;
    for (EventInfo *eventTable = sEventTable; eventTable->id != -1; eventTable++)
        mEvents++;

    mProctime1 = -1;
    mProctime2 = -1;
    mProctime3 = -1;
    mProctime4 = -1;

    SysDebug::getManager()->setHeapGroup("RESTART", nullptr);
    SysDebug::getManager()->setDefaultHeapGroup(nullptr);
    SysDebug::getManager()->setUserTimeLabel(0, "GX ISSUE");
    SysDebug::getManager()->setUserTimeLabel(1, nullptr);
    SysDebug::getManager()->setUserTimeLabel(2, "CALC");
    SysDebug::getManager()->setUserTimeLabel(3, nullptr);
    SysDebug::getManager()->setUserTimeLabel(4, "UPDATE");
    SysDebug::getManager()->setUserTimeLabel(5, nullptr);
    SysDebug::getManager()->setUserTimeLabel(6, "VIEWCALC");
    SysDebug::getManager()->setUserTimeLabel(7, nullptr);
}

void RaceMgr::updateBestTime() {
    ECourseID courseID = RCMGetCourse()->getCourseID();
    if (gSystemRecord.getBestLap(courseID)->isValid())
        mBestLapTime.set(gSystemRecord.getBestLap(courseID)->getRaceTime());
    else
        mBestLapTime.reset();

    for (int i = 0; i < 5; i++) {
        if (gSystemRecord.getTARecord(courseID, i)->isValid())
            mBestTotalTimes[i].set(gSystemRecord.getTARecord(courseID, i)->getRaceTime());
        else
            mBestTotalTimes[i].reset();
    }
}

void RaceMgr::setRandomSeed() {
    for(int i = 0; i < 6; i++) {
        stGetRnd(i)->setSeed(mRaceInfo->mRandomSeed); // i have no clue why this file has different options for inlining
    }
    GameAudio::Random::setSeed(mRaceInfo->mRandomSeed);
}