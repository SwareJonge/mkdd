#include "kartEnums.h"
#include "kartLocale.h"
#include "dolphin/OS.h"
#include "JSystem/J3D/J3DSys.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "Inagaki/GameAudioMain.h"
#include "Kameda/J2DManager.h"
#include "Kameda/MotorManager.h"
#include "Kameda/PauseManager.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/Course/CrsData.h"
#include "Kaneshige/DemoTimeKeeper.h"
#include "Kaneshige/ExModel.h"
#include "Kaneshige/LightMgr.h"
#include "Kaneshige/RaceLight.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/SysDebug.h"
#include "Kaneshige/TexLODControl.h"
#include "Osako/kartPad.h"
#include "Osako/ResMgr.h"
#include "Osako/shadowMgr.h"
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

#pragma push
#pragma sym on

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

// UNUSED, my "guess" at how it would've looked like
void PrintRaceHeap(u32 free, JKRHeap * heap) {
    JUTReport(0, 0, "LINE%4d:(%d/%d)\n", 0, free, heap->getHeapSize());
}

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

void RaceMgr::editRaceInfoForDebug() {
    s16 startpointID = sMyStartPointID;
    if (startpointID >= 0) {
        RaceInfo *rInfo = mRaceInfo;
        s16 origStartPosIdx = rInfo->mStartPosIndex[0];
        for (int i = 0; i < 8; i++) {
            if (rInfo->mStartPosIndex[i] == startpointID) {
                rInfo->mStartPosIndex[i] = origStartPosIdx;
                break;
            }
        }
        mRaceInfo->mStartPosIndex[0] = sMyStartPointID;
    }
}

void RaceMgr::createConsole() {
    int cnsCnt = mRaceInfo->getConsoleNumber();
    mConsole = new Console[cnsCnt];
    for (int i = 0; i < cnsCnt; i++) {
        Console *cns = &mConsole[i];
        cns->setConsoleNo(i);
        if (i == 0)
            cns->setDraw();
    }
    if (!isWaitDemoMode() && !isAwardDemoMode() && !isStaffRoll())
        return;
    resetConsole();
}

void RaceMgr::resetConsole() {
    int cnsCnt = mRaceInfo->getConsoleNumber();
    for (int i = 0; i < cnsCnt; i++) {
        Console *cns = &mConsole[i];
        if (!mRaceInfo->isHiddingConsole(i))
            cns->setDraw();
        else 
            cns->clrDraw();
    }
}

bool RaceMgr::hideConsole(u32 cnsNo) {
    bool ret = false;
    int cnsCnt = mRaceInfo->getConsoleNumber();
    if (cnsNo < cnsCnt) {
        mConsole[cnsNo].clrDraw();
        ret = true;
    }
    return ret;
}

void RaceMgr::createModel() {
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

void RaceMgr::createKartModel() {
    ShadowManager::ptr();
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
    SysDebug::getManager()->createHeapInfo(nullptr, "KART MDL");
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();

    for(int i = 0; i < getKartNumber(); i++) {
        mKartLoader[i]->load();
        mKartLoader[i]->createModel(solidHeap, getCameraNumber());
        mRaceDrawer->getKartDrawer(i)->setAnimation();
    }

    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createCourseModel() {
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
    SysDebug::getManager()->createHeapInfo(nullptr, "CRS MDL");
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    mCourse->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createObjectModel() {
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
    SysDebug::getManager()->createHeapInfo(nullptr, "OBJ MDL");
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    GetGeoObjMgr()->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createItemModel() {
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
    SysDebug::getManager()->createHeapInfo(nullptr, "ITEM MDL");
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    GetItemObjMgr()->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createEffectModel() {
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
    SysDebug::getManager()->createHeapInfo(nullptr, "EFCT MDL");
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    GetStEfctMgr()->createModel(solidHeap, getCameraNumber());
    GetJ3DEfctMgr()->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createLight() {
    SysDebug::getManager()->setHeapGroup("LIGHT MGR", nullptr);

    LightMgr::createManager();
    TBalloonString * balloonString = TBalloonString::getSupervisor();

    JUtility::TColor ambientColor;
    mCourse->getAmbientColor(&ambientColor);
    LtObjAmbient *lghtObjAmb = new LtObjAmbient("シーンアンビエント", ambientColor); // scene ambient?

    lghtObjAmb->setTagName('AMBI');
    LightMgr::getManager()->appendLight(lghtObjAmb);

    JUtility::TColor lightColor;
    mCourse->getLightColor(&lightColor);
    JGeometry::TVec3f lightPos;
    mCourse->getLightOffsetPosition(&lightPos);

    for(int i = 0; i < getConsoleNumber(); i++) {
        RaceSceneLight *sceneLight = new RaceSceneLight("シーンライト", i, lightColor, lightPos);
        LightMgr::getManager()->appendLight(sceneLight);
        for(int j = 0; j < getKartNumber(); j++) {
            RaceKartLight *kartLight = new RaceKartLight(sceneLight, j);
            LightMgr::getManager()->appendLight(kartLight);
            getKartDrawer(j)->setLight(i, kartLight);
        }
        if (balloonString != nullptr) {
            RaceBalloonLight *balloonLight = new RaceBalloonLight(i);
            LightMgr::getManager()->appendLight(balloonLight);
        }
        if(isAwardDemoMode()) {
            RaceCupLight *cupLight = new RaceCupLight(sceneLight);
            LightMgr::getManager()->appendLight(cupLight);
        }
    }
}

void RaceMgr::resetRace() {
    bool needHardReset = false;

    switch (mRaceDirector->getRacePhase()) {
    case PHASE_4:
        resetRaceForRestartEvent();
        needHardReset = true;
        break;
    case PHASE_3:
        resetRaceForResetEvent();
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

void RaceMgr::resetRaceForResetEvent() {
    OSReport("Race    Start-------------------------------------------------------\n");
    updateBestTime();
}

void RaceMgr::resetRaceForRestartEvent() {
    OSReport("Restart Start-------------------------------------------------------\n");
    if(isRaceModeMiniGame()) {
        mRaceInfo->shuffleRandomSeed();
        mRaceInfo->shuffleStartNo();
    }

    mReplayMode &= 0xfffe;
    updateBestTime();
}

void RaceMgr::resetRaceForReplayEvent() {
    OSReport("Replay  Start-------------------------------------------------------\n");
    mReplayMode |= 1;
}

void RaceMgr::resetRaceCommon() {
    OSReport("Begin Reset.............................................\n");
    OSReport("Rand Seed:%08X\n", mRaceInfo->mRandomSeed);

    GameAudio::Main::getAudio()->initRaceSound();
    mFrame = 0;
    mRaceDirector->reset();
    setRandomSeed();
    mRaceBGMPlayer->reset();
    for(int i = 0; i < getCameraNumber(); i++) {
        setJugemZClr(i, true);
    }
    resetConsole();
    SysDebug::getManager()->setHeapGroup("RESTART", nullptr);
    mCourse->reset();
    GetGeoObjMgr()->reset(mCourse->getCrsData());
    GetItemObjMgr()->reset();

// while true loop
    for(int i = 0; i < getKartNumber(); i++) {
        mKartChecker[i]->reset();
        mKartLoader[i]->reset();
    }

    KartCtrl::getKartCtrl()->DynamicsReset();
    GetStEfctMgr()->reset();
    GetJ3DEfctMgr()->reset();
    GetEfctScreenMgr()->reset();
    MotorManager::getManager()->reset();
    PauseManager::getManager()->reset();

    if(!isWaitDemoMode() && !isAwardDemoMode() && !isStaffRoll()) {
        PauseManager::getManager()->enablePause();
    }

    J2DManager::getManager()->reset();
    J2DManager::getManager()->startLANNumAnm();
    if(mAward2D != nullptr)
        mAward2D->still();
    if (mStaffRoll2D != nullptr)
        mStaffRoll2D->reset();
    
    GetJPAMgr()->reset();
    mRaceDrawer->reset();
    if(!isRaceModeMiniGame())
        RivalKart::Reset();

    SysDebug::getManager()->setDefaultHeapGroup(nullptr);
    OSReport("End   Reset.............................................\n");
}

int RaceMgr::getCurrentConsoleNumber() {
    int ret = getConsoleNumber();
    if(isCrsDemoMode())
        ret = 1;

    return ret;
}

void RaceMgr::drawRace() {
    SysDebug::getManager()->beginUserTime(0);
    mRaceDrawer->drawPreScene();
    for(int i = 0; i < getCameraNumber(); i++) {
        if(mConsole[i].isDraw())
            mRaceDrawer->drawSceneFirstStage(i);
    }
    mRaceDrawer->drawMidScene();

    for (int i = 0; i < getCameraNumber(); i++){
        if (mConsole[i].isDraw())
            mRaceDrawer->drawSceneSecondStage(i);
    }

    mRaceDrawer->drawPostScene();
    SysDebug::getManager()->endUserTime(0);
}

void RaceMgr::checkKart() {
    mRaceDirector->isFrameRenewal();
    for(int i = 0; i < getKartNumber(); i++) {
        KartChecker * kartChecker = mKartChecker[i];
        kartChecker->checkKart();
        if(isAbleStart())
            kartChecker->incTime();
    }
}

#pragma pop