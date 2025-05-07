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

#ifdef DEBUG
#pragma sym on
#endif

#include "kartEnums.h"
#include "kartLocale.h"
#include "Inagaki/GameAudioMain.h"
#include "Inagaki/GameAudioCommon.h" 
#include "Kameda/J2DManager.h"
#include "Kameda/MotorManager.h"
#include "Kameda/PauseManager.h"
#include "Kaneshige/Course/Course.h"
#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/Course/CrsData.h"
#include "Kaneshige/Objects/GeoRabbitMark.h"
#include "Kaneshige/DemoTimeKeeper.h"
#include "Kaneshige/ExModel.h"
// #include "Kaneshige/LightMgr.h"
#include "Kaneshige/RaceInfo.h"
#include "Kaneshige/RaceLight.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/RaceUsrPage.h"
// #include "Kaneshige/SysDebug.h"
#include "Kaneshige/TexLODControl.h"
#include "Kawano/accessory.h"
// #include "Osako/kartPad.h"
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
// #include "Sato/RivalKart.h"
#include "Sato/stEffectMgr.h"
#include "Sato/stMath.h"

// #include "Shiraiwa/Balloon.h"
// #include "Shiraiwa/JugemRodSignal.h"
#include "Yamamoto/kartCamera.h"

RaceMgr *RaceMgr::sRaceManager;
s16 RaceMgr::sForceTotalLapNum;
s16 RaceMgr::sDispFrameCounter;

s16 RaceMgr::sMyStartPointID = -1;

#include "mathHelper.h"

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

// fabircated to generate certain data first
// just ignore it
void PrintRaceHeap(u32 free, JKRHeap *heap)
{
    if (false) {
        KartCtrl::getKartCtrl()->getKartCam(0)->GetPosv();
        KartCtrl::getKartCtrl()->getKartCam(0)->GetPosh();
        KartCtrl::getKartCtrl()->getKartCam(0)->GetWidth();
        KartCtrl::getKartCtrl()->getKartCam(0)->GetHeight();
    }

    if (heap != nullptr)
        JUT_REPORT_MSG("LINE%4d:(%d/%d)\n", 0, free, heap->getHeapSize());
}

RaceMgr::RaceMgr(RaceInfo *raceInfo) : mRaceInfo(nullptr),
                                       mCourse(nullptr),
                                       mBestLapTime()
{

    Language lang = KartLocale::getLanguage();
    VideoMode videoMode = KartLocale::getVideoMode();
    JUT_REPORT_MSG("Demo    Start-------------------------------------------------------\n");
    JUT_REPORT_MSG("Race Mode:%08X\n", raceInfo->getRaceMode());
    JUT_REPORT_MSG("Stat Num :%08X\n", raceInfo->getStatusNumber());
    JUT_REPORT_MSG("Rand Seed:%08X\n", raceInfo->mRandomSeed);

    sRaceManager = this;

    SYSDBG_SetHeapGroup("RACE MGR", nullptr);
    _0x22 = 0;
    mFrame = 0;
    mRaceHeap = JKRGetCurrentHeap();
    mReplayMode = 0;
    mRaceInfo = raceInfo;

    editRaceInfoForDebug();
    SYSDBG_AppendPage(new RaceUsrPage(mRaceInfo));

    _0x30 = 0; // might've been used for debug only, this is the only time it gets used
    mAbleStart = false;
    mAreaLight = 0;

    if (getProcLevel() <= 0)
        activeAreaLight();

    bool waitForDemo = false;
    if (!isWaitDemoMode())
    {
        switch (getRaceMode())
        {
        case GRAND_PRIX:
        case BALLOON_BATTLE:
        case ROBBERY_BATTLE:
        case BOMB_BATTLE:
        case ESCAPE_BATTLE:
            waitForDemo = true;
            break;
        }
    }

    DemoTimeKeeper *timeKeeper = nullptr;
    if (isAwardDemoMode())
        timeKeeper = new AwardDemoTimeKeeper();
    else if (isStaffRoll())
        timeKeeper = new StaffRollTimeKeeper();

    mRaceDirector = new RaceDirector(waitForDemo, timeKeeper, getConsoleNumber());

    u16 numLevels = 1;
    if (mRaceInfo->isDriverLODOn())
        numLevels = 2;

    SYSDBG_SetHeapGroup("COURSE MGR", nullptr);

    CrsData::SColHeader *bco = (CrsData::SColHeader *)ResMgr::getPtr(ResMgr::mcCourseBco);
    CrsData::SOblHeader *bol = (CrsData::SOblHeader *)ResMgr::getPtr(ResMgr::mcCourseBol);
    CrsData *crsData = new CrsData(bco, bol);

    if (isStaffRoll())
        crsData->patchCamDataForStaffRoll(lang, videoMode);

    JUT_REPORT_MSG("Course ID:%08X\n", crsData->getCourseID());

#ifdef DEBUG
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
#endif

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

    int level = 0;
    if (!isRaceModeMiniGame() && getConsoleNumber() >= 3)
        level = 1;

    TexLODControl *texLod = new TexLODControl(level);
    texLod->setGeographyLODBiasOn(crsData->isTexLODBiasOn());
    bool staffRoll = isStaffRoll();

    SYSDBG_SetHeapGroup("RACE DRAWER", nullptr);
    mRaceDrawer = new RaceDrawer(numLevels, staffRoll);
    SYSDBG_SetHeapGroup("RACE BGM", nullptr);
    mRaceBGMPlayer = new RaceBGMPlayer();

    createConsole();
    stRandom::createAllRandom();
    setRandomSeed();

    ExMdlDataMgr *mdlData = new ExMdlDataMgr(isMirror());
    SYSDBG_SetHeapGroup("SHADOW SCR", nullptr);
    JUtility::TColor color;
    crsData->getShadowColor(&color);
    ShadowManager::ptr()->setMirror(isMirror());
    ShadowManager::ptr()->setShadowColor(color);
    ShadowManager::ptr()->setShadowDepth(crsData->getShadowDepth());

    SYSDBG_SetHeapGroup("COURSE MGR", nullptr);
    bool credits = false; // Kaneshige moment
    if (isStaffRoll())
        credits = true;

    mCourse = new Course(crsData, getCameraNumber(), credits);
    mAwardArc = nullptr;
    if (isAwardDemoMode())
    {
#line 2184
        mAwardArc = ResMgr::getArchive(ResMgr::mcArcAward);
        JUT_ASSERT_MSG(mAwardArc, "NOT LOAD AWARD ARC");
    }
    SYSDBG_SetHeapGroup("OBJECT MGR", nullptr);
    CreateGeoObjMgr(mCourse->getCrsData());
    CreateCharIKParamMgr();

    bool isTT = false;
    if (getRaceMode() == TIME_ATTACK)
        isTT = true;

    KartChecker::sBombPointFull = KartChecker::getWinBombPointForMenu(getKartNumber());

    for (int i = 0; i < 8; i++)
    {
        SYSDBG_SetHeapGroup("KART WORK", nullptr);
        KartChecker *kartChker = nullptr;
        KartLoader *kartLdr = nullptr;
        if (i < getKartNumber())
        {
            kartChker = new KartChecker(i, getKartInfo(i), getCourse()->getTrackSectorNumber(), getTotalLapNumber());
            kartLdr = new KartLoader(i, getKartInfo(i), numLevels, isTT, isAwardDemoMode());
        }
        mKartChecker[i] = kartChker;
        mKartLoader[i] = kartLdr;
    }

    if (isAwardDemoMode())
        mKartLoader[0]->setDemoBodyBmd(ObjUtility::getPtrCourseArc("/Objects/demo_k_body.bmd"));

    SYSDBG_SetHeapGroup("PARTICLE MGR", nullptr);
    CreateJPAMgr(mRaceHeap, false);
    CreateStEfctMgr();
    GetStEfctMgr()->init();
    CreateJ3DEfctMgr();
    CreateJ3DEfctKarAnmMgr();

    SYSDBG_SetHeapGroup("ITEM MGR", nullptr);
    CreateItemObjMgr();

    SYSDBG_SetHeapGroup("KART MGR", nullptr);
    KartCtrl::makeKartCtrl();
    KartCtrl::getKartCtrl()->DynamicsInit(ExModel::isMtxCombinationOn());
    createModel();
    CreateEfctScreenMgr();
    createLight();

    SYSDBG_SetHeapGroup("MOTOR MGR", nullptr);
    new MotorManager(mRaceHeap);

    SYSDBG_SetHeapGroup("2D DATA", nullptr);
    J2DManager *j2dMgr = new J2DManager(mRaceHeap);
    PauseManager *pauseMgr = new PauseManager(mRaceHeap);

    if (isAwardDemoMode())
        mAward2D = new Award2D(mRaceHeap);
    else
        mAward2D = nullptr;

    if (isStaffRoll())
    {
        SYSDBG_SetHeapGroup("STAFF ROLL2D", nullptr);
        bool ending = mRaceInfo->isTrueEnding();
        mStaffRoll2D = new StaffRoll2D(mRaceHeap, lang, videoMode, ending);
        SYSDBG_SetHeapGroup("2D DATA", nullptr);
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

    SYSDBG_SetHeapGroup("RACE MGR", nullptr);
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

    SYSDBG_SetHeapGroup("RESTART", nullptr);
    SYSDBG_SetDefaultHeapGroup(nullptr);
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
        RaceInfo *raceInfo = mRaceInfo;
        s16 origStartPosIdx = raceInfo->mStartPosIndex[0];
        for (int i = 0; i < 8; i++)
        {
            if (raceInfo->mStartPosIndex[i] == startpointID)
            {
                raceInfo->mStartPosIndex[i] = origStartPosIdx;
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
#line 3969
    JUT_MINMAX_ASSERT(0, viewNo, mRaceInfo->getConsoleNumber());
    return mConsole[viewNo].isDraw();
}

void RaceMgr::createModel()
{
    Mtx viewMtx;
    PSMTXIdentity(viewMtx);
    j3dSys.setViewMtx(viewMtx);
    SYSDBG_SetHeapGroup("KART MODEL", nullptr);
    createKartModel();
    SYSDBG_SetHeapGroup("COURSE MODEL", nullptr);
    createCourseModel();
    SYSDBG_SetHeapGroup("OBJECT MODEL", nullptr);
    createObjectModel();
    SYSDBG_SetHeapGroup("ITEM MODEL", nullptr);
    createItemModel();
    SYSDBG_SetHeapGroup("EFFECT MODEL", nullptr);
    createEffectModel();
}

void RaceMgr::createKartModel()
{
    ShadowManager::ptr();
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
    SYSDBG_CreateDebugHeapInfo(solidHeap, "KART MDL");
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
    SYSDBG_CreateDebugHeapInfo(solidHeap, "CRS  MDL");
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    mCourse->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createObjectModel()
{
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
    SYSDBG_CreateDebugHeapInfo(solidHeap, "OBJ  MDL");
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    GetGeoObjMgr()->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createItemModel()
{
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
    SYSDBG_CreateDebugHeapInfo(solidHeap, "ITEM MDL");
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    GetItemObjMgr()->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createEffectModel()
{
    size_t freeSize = mRaceHeap->getFreeSize();
    JKRSolidHeap *solidHeap = JKRCreateSolidHeap(freeSize, mRaceHeap, false);
    SYSDBG_CreateDebugHeapInfo(solidHeap, "EFCT MDL");
    JKRHeap *curHeap = solidHeap->becomeCurrentHeap();
    GetStEfctMgr()->createModel(solidHeap, getCameraNumber());
    GetJ3DEfctMgr()->createModel(solidHeap, getCameraNumber());
    solidHeap->adjustSize();
    curHeap->becomeCurrentHeap();
}

void RaceMgr::createLight()
{
    SYSDBG_SetHeapGroup("LIGHT MGR", nullptr);

    LightMgr::createManager();
    bool balloonExists = false;

    if (TBalloonString::getSupervisor())
        balloonExists = true;

    JUtility::TColor ambientColor;
    mCourse->getAmbientColor(&ambientColor);
    LtObjAmbient *lightObjAmb = new LtObjAmbient("シーンアンビエント", ambientColor); // scene ambient?

    lightObjAmb->setTagName('AMBI');
    LightMgr::getManager()->appendLight(lightObjAmb);

    JUtility::TColor lightColor;
    mCourse->getLightColor(&lightColor);
    JGeometry::TVec3f lightPos;
    mCourse->getLightOffsetPosition(&lightPos);

    for (u32 i = 0; i < getCameraNumber(); i++)
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
            LightMgr::getManager()->appendLight(new RaceBalloonLight(i));
        }
        if (isAwardDemoMode())
        {
            LightMgr::getManager()->appendLight(new RaceCupLight(sceneLight));
        }
    }
}

void RaceMgr::resetRace()
{
    bool needHardReset = false;
    ERacePhase racePhase = mRaceDirector->getRacePhase();
    switch (racePhase)
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
    JUT_REPORT_MSG("Race    Start-------------------------------------------------------\n");
    updateBestTime();
}

void RaceMgr::resetRaceForRestartEvent()
{
    JUT_REPORT_MSG("Restart Start-------------------------------------------------------\n");
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
    JUT_REPORT_MSG("Replay  Start-------------------------------------------------------\n");
    mReplayMode |= 1;
}

void RaceMgr::resetRaceCommon()
{
    JUT_REPORT_MSG("Begin Reset.............................................\n");
    JUT_REPORT_MSG("Rand Seed:%08X\n", mRaceInfo->mRandomSeed);

    GetGameAudioMain()->initRaceSound();
    mFrame = 0;
    mRaceDirector->reset();
    setRandomSeed();
    mRaceBGMPlayer->reset();
    for (u32 i = 0; i < getCameraNumber(); i++)
    {
        setJugemZClr(i, true);
    }
    resetConsole();
    SYSDBG_SetHeapGroup("RESTART", nullptr);
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

    SYSDBG_SetDefaultHeapGroup(nullptr);
    JUT_REPORT_MSG("End   Reset.............................................\n");
}

// Unused
int RaceMgr::getDrawingConsoleNumber()
{
    int ret = 0;
    for (int i = 0; i < getKartNumber(); i++)
    {
        if (mConsole[i].isDraw())
            ret++;
    }
    return ret;
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
            bool finalLap = kartChecker->isFinalLap();
            if (false) {
                // ~RaceUsrPage() needs to be emitted right here, between isFinalLap() and isPlayerKart()
                RaceUsrPage page(mRaceInfo);
            }
            if (finalLap &&
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
    mRaceInfo->mWaitDemoResult = mRaceDirector->getWaitDemoResult();
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
#ifdef DEBUG
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

// fabricated to call all JSUTree and JSUTreeIterator functions before ~RaceMgr, which is needed to order JKRHeap::getHeapTree() correctly
// if any one of these functions is first emitted in the destructor (including ++iterator and iterator.getObject()),
// it "pulls" the getHeapTree() call into the section with all the JSUTree* functions. don't ask me why
void DestroyHeapTree(const JSUTree<JKRHeap>& heapTree)
{
    for (JSUTreeIterator<JKRHeap> iterator = heapTree.getFirstChild(); iterator != heapTree.getEndChild(); ++iterator)
    {
        SYSDBG_DestroyHeapInfo(iterator.getObject());
    }
}

RaceMgr::~RaceMgr()
{
    if (GameAudioParameters::getDemoMode())
        GameAudioParameters::setDemoMode(0);

    for (JSUTreeIterator<JKRHeap> iterator = mRaceHeap->getHeapTree().getFirstChild(); iterator != mRaceHeap->getHeapTree().getEndChild(); ++iterator)
    {
        SYSDBG_DestroyHeapInfo(iterator.getObject());
    }
    SYSDBG_ClearAllLabels();
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
#line 4353
    JUT_MINMAX_ASSERT(0, viewNo, mRaceInfo->getConsoleNumber());
    Console *console = &mConsole[viewNo];
    if (clear)
        console->setJugemZClr();
    else
        console->clrJugemZClr();
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

#line 4396
    JUT_MINMAX_ASSERT(0, kartNo, getKartNumber());
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
#line 4436
        CrsData::StartPoint *startPoint = getCourse()->getCrsData()->getStartPoint(startID);
        JUT_ASSERT_F(startPoint, "NOT FOUND START:%d", startID);
        isRight = startPoint->isRight();
        startPoint->getPosition(position);
        startPoint->getFrDirection(direction);
        if (tindex >= 0)
        {
            const Vec startPosTable[18] = {
                {0.0f, 0.0f, 250.0f},
                {600.0f, 0.0f, 250.0f},
                {267.0f, 0.0f, 0.0f},
                {-67.0f, 0.0f, -250.0f},
                {-400.0f, 0.0f, -500.0f},
                {450.0f, 0.0f, -600.0f},
                {117.0f, 0.0f, -850.0f},
                {-217.0f, 0.0f, -1100.0f},
                {-550.0f, 0.0f, -1350.0f},
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
#line 4480
            JUT_MINMAX_ASSERT(0, tindex, 18);
            const f32 startPosX = startPosTable[tindex].x;
            const f32 startPosZ = startPosTable[tindex].z;
            startPos.set(startPosX, 0.0f, startPosZ);

            if (startPoint->isRight())
            {
                startPos.x = -startPos.x;
            }

            JGeometry::TVec3f scaledDirection = (*direction);
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
    JUT_MINMAX_ASSERT(0, kartNo, getKartNumber());
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

void RaceMgr::beginProcTime(s16 id)
{
#ifdef DEBUG
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

void RaceMgr::endProcTime(s16 id)
{
#ifdef DEBUG
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

const RaceMgr::EventInfo *RaceMgr::searchEventInfo(s16 searchId)
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
    if (!mAbleStart)
        ret = GetGeoObjMgr()->getJugem(0)->isCallThree();
    else
        ret = true;

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
    mIsDemo = false;
    mFlags = 0;
    mFlags |= 8;
}

void RaceMgr::Console::changeTargetNo(int targetNo, bool p2)
{
    if (isValid())
    {
        mTargetNo = targetNo;
        mIsDemo = p2;
        RCMGetCamera(mCnsNo)->SetTargetNum(mTargetNo);
        if (!isNoStat())
            J2DManager::getManager()->setStatus2Kart(mCnsNo, mTargetNo);
    }
}

// these functions are unused so i'm just putting functions in here that need to get generated
void RaceMgr::Console::clearZBuffer()
{
    if (!System::getDisplay()->getFBAlpha())
        System::getDisplay()->setFBAlpha(true);
    System::getDisplay()->clearEfb(System::getDisplay()->getClearColor());
}

bool RaceMgr::Console::isZoom()
{
    RCMGetKartChecker(0);
}

bool RaceMgr::robRivalOfBalloon(int playerIdx, int rivalIdx)
{
    bool robbed = false;
    TBalloonManager *balloonMgr = GetGeoObjMgr()->getBalloonMgr(playerIdx);
#ifdef DEBUG
    if (getKartChecker(playerIdx)->getBalloonNumber() <= 0)
    {
        return false;
    }
    else
    {
#endif
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
#ifdef DEBUG
    }
#endif
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
        const char *kind = nullptr;
        switch (kartInfo->getPlayerKind(0))
        {
        case 0:
            kind = "NONE ";
            break;
        case 1:
            kind = "MAN  ";
            break;
        case 2:
            kind = "COM  ";
            break;
        case 3:
            kind = "NET  ";
            break;
        case 4:
            kind = "GHOST";
            break;
        }
        KartGamePad *kartPad = kartInfo->getPad(0);
        const char *type = "NULL  ";
        const char *port = "NULL  ";

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

#include "JSystem/JAudio/JASFakeMatch2.h"
