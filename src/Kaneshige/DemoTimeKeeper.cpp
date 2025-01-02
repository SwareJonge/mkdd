#include "Kaneshige/DemoTimeKeeper.h"
#include "Inagaki/GameAudioMain.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTFader.h"
#include "JSystem/JUtility/TColor.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Kaneshige/RaceMgr.h"
#include "Osako/kartPad.h"
#include "Osako/system.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/EffectScreen.h"
#include "Sato/RivalSpeedCtrl.h"
#include "Shiraiwa/MapObjAward.h"
#include "Shiraiwa/MapObjHanabi.h"
#include "kartEnums.h"

s32 StaffRollTimeKeeper::sStartTitleCreditTime;
s32 StaffRollTimeKeeper::sStartExectiveProducerCreditTime;
s32 StaffRollTimeKeeper::sStartProducerCreditTime;
s32 StaffRollTimeKeeper::sStartChiefDirectorCreditTime;
s32 StaffRollTimeKeeper::sStartDirectorCreditTime;
s32 StaffRollTimeKeeper::sStartProgramDirectorCreditTime;
s32 StaffRollTimeKeeper::sStartProgramCreditTime;
s32 StaffRollTimeKeeper::sStartDesignManagerCreditTime;
s32 StaffRollTimeKeeper::sStartCourseDesignCreditTime;
s32 StaffRollTimeKeeper::sStartGraphicDesignCreditTime;
s32 StaffRollTimeKeeper::sStartScreenDesignCreditTime;
s32 StaffRollTimeKeeper::sStartMusicCreditTime;
s32 StaffRollTimeKeeper::sStartVoiceCreditTime;
s32 StaffRollTimeKeeper::sStartProgressCreditTime;
s32 StaffRollTimeKeeper::sStartDebugSupportCreditTime;
s32 StaffRollTimeKeeper::sStartSpecialCreditTime;
s32 StaffRollTimeKeeper::sStartLocalizationCreditTime;
s32 StaffRollTimeKeeper::sStartLastCreditTime;

s16 AwardDemoTimeKeeper::sEventTimeHanabi1 = 10;
s16 AwardDemoTimeKeeper::sEventTimeHanabi2 = 70;
s16 AwardDemoTimeKeeper::sEventTimeHanabi3 = 120;
s16 AwardDemoTimeKeeper::sEventTimeHanabi4 = 160;
s16 AwardDemoTimeKeeper::sEventTimeHanabi5 = 210;
s16 AwardDemoTimeKeeper::sEventTimeHanabi6 = 190;
s16 AwardDemoTimeKeeper::sEventTimeHanabi7 = 220;
s16 AwardDemoTimeKeeper::sEventTimeHanabi8 = 225;
s16 AwardDemoTimeKeeper::sEventTimePaper = 180;
s16 AwardDemoTimeKeeper::sEventTimeStart2D = 60;

s32 StaffRollTimeKeeper::sKartAppearancePreTime = 60;
f32 StaffRollTimeKeeper::sKartMaxSpeed = 50.0f;

JUTColor StaffRollTimeKeeper::sBlurTitleColor(0xff,0xfa,0xff,0xfa);
JUTColor StaffRollTimeKeeper::sBlurExProducerColor(0xff,0xff,0xfa,0xfa);
JUTColor StaffRollTimeKeeper::sBlurProducerColor(0xfa,0xff,0xff,0xfa);
JUTColor StaffRollTimeKeeper::sBlurChiefDirectorColor(0xff,0xfa,0xfa,0xfa);
JUTColor StaffRollTimeKeeper::sBlurDirectorColor(0xfa,0xff,0xfa,0xfa);
JUTColor StaffRollTimeKeeper::sBlurProgramDirectorColor(0xfa,0xfa,0xff,0xfa);
JUTColor StaffRollTimeKeeper::sBlurProgramColor(0xfa,0xf5,0xfa,0xfa);
JUTColor StaffRollTimeKeeper::sBlurDesignManagerColor(0xfa,0xfa,0xf5,0xfa);
JUTColor StaffRollTimeKeeper::sBlurCourseDesignColor(0xf5,0xfa,0xfa,0xfa);
JUTColor StaffRollTimeKeeper::sBlurGraphicDesignColor(0xfa,0xf5,0xf5,0xfa);
JUTColor StaffRollTimeKeeper::sBlurScreenDesignColor(0xf5,0xfa,0xf5,0xfa);
JUTColor StaffRollTimeKeeper::sBlurMusicColor(0xf5,0xf5,0xfa,0xfa);
JUTColor StaffRollTimeKeeper::sBlurVoiceColor(0xf5,0xf0,0xf5,0xfa);
JUTColor StaffRollTimeKeeper::sBlurProgressColor(0xf5,0xf5,0xf0,0xfa);
JUTColor StaffRollTimeKeeper::sBlurDebugSupportColor(0xf0,0xf5,0xf5,0xfa);
JUTColor StaffRollTimeKeeper::sBlurSpecialColor(0xf5,0xf0,0xf0,0xfa);
JUTColor StaffRollTimeKeeper::sBlurLocalizationColor(0xf0,0xf5,0xf0,0xfa);
JUTColor StaffRollTimeKeeper::sBlurLastColor(0xf5,0xf0,0xf0,0xfc);

#include "mathHelper.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

DemoTimeKeeper::DemoTimeKeeper(bool isStaffRoll) {
    mStaffRoll = isStaffRoll;
    mDispFlags = 0;
    reset();
}

void DemoTimeKeeper::reset() {
    _8 = -1;
    mCamId = -1;

    mCutTimer = 0;
    mCutName = 0;
    mCurCutTime = 0;

    for (int i = 0; i < 5; i++) {
        mName[i] = 0;
    }
    mDemoEnd = false;
}

void DemoTimeKeeper::calc() {
    if (mStaffRoll) {
        mCamId = RCMGetCamera(0)->GetDemoCam()->mStartId;
    }
    else {
        mCamId = RCMGetCamera(0)->mCurCamIdx;
    }
    if(mCamId != _8) {
        _8 = mCamId;
        mCutTimer = 0;
    }

    mCutName = 0;
    mCurCutTime = 0;

    CrsData::Camera *cameraData = RCMGetCourse()->getCrsData()->getCameraData(mCamId);
    if (cameraData) {
        mCutName = cameraData->mName;
        mCurCutTime = cameraData->getTime();
    }
    
    if (isDispInfo()) {
        if (cameraData) {
            mName[0] = (cameraData->mName >> 24) & 0xff;
            mName[1] = (cameraData->mName >> 16) & 0xff;
            mName[2] = (cameraData->mName >> 8) & 0xff;
            mName[3] = cameraData->mName;
            mName[4] = '\0';
        }
        else {
            for (int i = 0; i < 5; i++) {
                mName[i] = '\0';
            }
        }
        JUTReport(60, 120, "NAME   : %s", mName);
        JUTReport(60, 140, "TIME   : %5d", mCutTimer);
    }
}

AwardDemoTimeKeeper::AwardDemoTimeKeeper() : DemoTimeKeeper(false) { reset(); }

void AwardDemoTimeKeeper::reset() {
    DemoTimeKeeper::reset();
    mEventTime = 0;
    _22 = 0;
    _24 = 0;
    _25 = 0;
    _26 = false;
    mPaperEventStarted = false; 
}

void AwardDemoTimeKeeper::calc() {
    DemoTimeKeeper::calc();

    switch (getCurCutName()) {
    case 0x66697273: { // firs
        doOpening();
        break;
    }
    case 0x746f7070: { // topp
        if (getCutTimer() == 0) {
            GetKartCtrl()->getKartEnemy(2)->getSpeedCtrl()->set_speeddown();
        }
        break;
    }
    case 0x7061726b: { // park
        doPark();
        break;
    }
    case 0x66726f6e: { // fron
        doFront();
        break;        
    }
    case 0x656e6464: { // endd
        doEnding();
        break;    
    }
    case 0x6e657874: { // next 
        doNext();   
    }
    }

    if (_25 && !_26 && gGamePad1P.testTrigger(KartGamePad::A | KartGamePad::START)) {
        jumpEndingCut();
    }

    if (isDispInfo()) {
        const char *cup = nullptr;
        switch (RaceMgr::getManager()->getRaceGpCup()) {
        case MUSHROOM_CUP: cup = "KINOKO"; break;
        case FLOWER_CUP: cup = "FLOWER"; break;
        case STAR_CUP: cup = "STAR"; break;
        case SPECIAL_CUP: cup = "SPECIAL"; break;
        case ALL_CUP_TOUR: cup = "ALL"; break;
        default:
#line 571
            JUT_ASSERT_MSG(false, "UNKNOWN CUP");
            break;
        }

        const char *level = nullptr;
        switch (RaceMgr::getManager()->getRaceLevel()) {
        case LVL_50CC: level = "50cc"; break;
        case LVL_100CC: level = "100cc"; break;
        case LVL_150CC: level = "150cc"; break;
        case LVL_MIRROR: level = "MIRROR"; break;
        default:
#line 583
            JUT_ASSERT_MSG(false, "UNKNOWN LV");
            break;
        }

        JUTReport(60, 160, "CUP    : %s", cup);
        JUTReport(60, 180, "LEVEL  : %s", level);
        JUTReport(60, 200, "MY KART: %1d", RaceMgr::getManager()->getAwardKartNo());
    }
    updateCutTimer();
}

void AwardDemoTimeKeeper::checkEventHanabi(s16 id) {
    s16 eventTime = -1;
    switch (id) {
    case 1:
        eventTime = sEventTimeHanabi1;
        break;
    case 2:
        eventTime = sEventTimeHanabi2;
        break;
    case 3:
        eventTime = sEventTimeHanabi3;
        break;
    case 4:
        eventTime = sEventTimeHanabi4;
        break;
    case 5:
        eventTime = sEventTimeHanabi5;
        break;
    case 6:
        eventTime = sEventTimeHanabi6;
        break;
    case 7:
        eventTime = sEventTimeHanabi7;
        break;
    case 8:
        eventTime = sEventTimeHanabi8;
        break;
    }

    if (eventTime == getCutTimer()) {
        TMapObjHanabi::getSupervisor()->fire(id);
    }
}

void AwardDemoTimeKeeper::doOpening() {
    checkEventHanabi(1);
    checkEventHanabi(2);
    checkEventHanabi(3);
    checkEventHanabi(4);
    checkEventHanabi(5);
    checkEventHanabi(6);
    checkEventHanabi(7);
    checkEventHanabi(8);
    if (getCutTimer() == 60) {
        _25 = true;
    }
}

void AwardDemoTimeKeeper::startEventPaper() {
    bool isAwardKart = true;
    if (RaceMgr::getManager()->getAwardKartNo() < 0)
        isAwardKart = false;
    GetJPAMgr()->createAwardDemoEmt(isAwardKart);
    mPaperEventStarted = true;
}

void AwardDemoTimeKeeper::doPark() {
    if (sEventTimePaper == getCutTimer()) {
        startEventPaper();
    }
}

void AwardDemoTimeKeeper::doFront() {}

void AwardDemoTimeKeeper::doEnding() {
    if ((getCutTimer() == 80) && ((_22 & 2) == 0)) {
        fadeOutSE();
    }

    if (TMapObjAwardItemBox::sSupervisor->isRollState()) {
        if (mEventTime < sEventTimeStart2D) {
            mEventTime++;
        }

        if ((_22 & 1) == 0 && mEventTime >= sEventTimeStart2D) {
            RaceMgr::getManager()->getAward2D()->start();
            GetEfctScreenMgr()->startDemoScreen();
            _22 |= 1;
        }
    }

    if (getCutTimer() >= 60) {
        _24 = true;
    }

    _25 = false;
    if (_24 && RaceMgr::getManager()->getAward2D()->isFinish()) {
        setDemoEnd();
    }
}

void AwardDemoTimeKeeper::doNext() {
    if (getCutTimer() == 0) {
        startEventPaper();
        stopAllKart();
        TMapObjAwardItemBox::sSupervisor->skip();

        if ((_22 & 1) == 0) {
            RaceMgr::getManager()->getAward2D()->start();
            GetEfctScreenMgr()->startDemoScreen();
            _22 |= 1;
        }
    }

    if (getCutTimer() >= 60) {
        _24 = true;
    }

    _25 = false;
    if (_24 && RaceMgr::getManager()->getAward2D()->isFinish()) {
        setDemoEnd();
    }
}

void AwardDemoTimeKeeper::jumpEndingCut() {
    _26 = 1;
    stopAllKart();
    if (!mPaperEventStarted) {
        startEventPaper();
    }
    TMapObjAwardItemBox::sSupervisor->skip();
    fadeOutSE();
}

void AwardDemoTimeKeeper::stopAllKart() {
    for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
        GetKartCtrl()->getKartEnemy(i)->getSpeedCtrl()->stop_immediately();
    }
}

void AwardDemoTimeKeeper::fadeOutSE() {
    GetGameAudioMain()->setRaceSeVolume(0.0f, 180);
    _22 |= 2;
}

int AwardDemoTimeKeeper::requestDemoCameraID() const {
    int id = -1;
    if(_26) {
        id = RCMGetCourse()->getCrsData()->searchCameraID(0x656e6464); // endd
    }
    return id;
}

StaffRollTimeKeeper::StaffRollTimeKeeper() : DemoTimeKeeper(true) { reset(); }

void StaffRollTimeKeeper::reset() {
    DemoTimeKeeper::reset();
    _20 = 0;
    mJumpFlag = 0;
    _24 = 0;
    mBlurColor.set(0xff,0xfa,0xff,0xff);
    _2c = 0;
    _30 = 0;
}

void StaffRollTimeKeeper::calc() {
    DemoTimeKeeper::calc();

    switch (getCurCutName()) {
    case 0x6f70656e: // open
        doOpening();
        break;
    case 0x6d617269: // mari
        doMarioTitle();
        break;
    case 0x65786563: // exec
        doExecProducer();
        break;
    case 0x70726f64: // prod
        doProducer();
        break;
    case 0x63686965: // chie
        doChiefDirector();
        break;
    case 0x64697265: // dire
        doDirector();
        break;
    case 0x70726469: // prdi
        doProgramDirector();
        break;
    case 0x70726f67: // prog
        doProgram();
        break;
    case 0x64657369: // desi
        doDesignManager();
        break;
    case 0x636f7572: // cour
        doCourseDesign();
        break;
    case 0x67726170: // grap
        doGraphicDesign();
        break;
    case 0x73637265: // scre
        doScreenDesign();
        break;
    case 0x6d757369: // musi
        doMusic();
        break;
    case 0x766f6963: // voic
        doVoice();
        break;
    case 0x70726d61: // prma
        doProgress();
        break;
    case 0x64656275: // debu
        doDebugSupport();
        break;
    case 0x73706563: // spec
        doSpecial();
        break;
    case 0x6c6f6361: // loca
        doLocalization();
        break;
    case 0x6c617374: // last
        doLast();
        break;
    }

    if (getCutTimer() == 0) {
        RaceMgr::getManager()->getBlurScreen()->flush();
        RaceMgr::getManager()->setBlurColor(mBlurColor);
    }

    if (!isJumpedEnd() && isEnableJump() && gGamePad1P.testTrigger(KartGamePad::A)) {
        jump();
    }
    updateCutTimer();
}

void StaffRollTimeKeeper::jump() {
    mJumpFlag = 1;
    setLastBlanking(60);
    GetGameAudioMain()->fadeOutAll(210);
}

void StaffRollTimeKeeper::setLastBlanking(int duration) {
    _2c = 1;
    _30 = 0;

    JUTColor fadeColor(0,0,0,0xff);
    SYSTEM_GetFader()->setColor(fadeColor);
    SYSTEM_StartFadeOut(duration);
}

bool StaffRollTimeKeeper::isKartAppearanceFrame() {
    bool ret = false;
    s32 appearanceFrame = getCurCutTime() - sKartAppearancePreTime;
    if (getCutTimer() == appearanceFrame) {
        ret = true;
    }
    return ret;
}

bool StaffRollTimeKeeper::checkCreditStart(int id, s32 time) {
    bool ret = false;  
    if (getCutTimer() == time) {
        RaceMgr::getManager()->getStaffRoll2D()->start(id);
        ret = true;
    }
    return ret;
}

void StaffRollTimeKeeper::setupCreditMask(int maskNo) {
    int infoBoxNum = RaceMgr::getManager()->getStaffRoll2D()->getExitBoxNum(maskNo);
    RaceMgr::getManager()->getMaskScreen()->resetAllMask();

    for (int i = 0; i < infoBoxNum; i++) {
        ExitBoxInfo *info = RaceMgr::getManager()->getStaffRoll2D()->getExitBoxInfoPtr(maskNo, i);
        bool b = false;
        if (info->getDir() == 1)
            b = true;

        f32 x = info->getBox().i.x;
        f32 y = info->getBox().i.y;
        if (b) {
            x = info->getBox().f.x;
            y = info->getBox().i.y; // mistake?
        }

        RaceMgr::getManager()->getMaskScreen()->requestMask(x, y, b);
    }
}

void StaffRollTimeKeeper::warpKart(u16 warpId) {
    JugemPoint *point = RCMGetCourse()->searchJugemPoint(warpId);
#line 1151
    JUT_ASSERT_F(point, "NOT FOUND WARP:%d", warpId);
    int kartIdx = warpId / 100;
    RCMGetKartChecker(kartIdx)->setDemoPoint(point);
    if (isDispInfo()) {
        JUTReport(60, 180 + kartIdx * 20, 120, "KART No.%1d:WARP(%03d)", kartIdx, warpId);
    }
}

void StaffRollTimeKeeper::setBlurColor(const JUTColor &color) {
    mBlurColor.set(color);
    RaceMgr::getManager()->setBlurDecrease(color.a);
}

int StaffRollTimeKeeper::requestDemoCameraID() const {
    int id = -1;
    if(isJumpedEnd()) {
        if (SYSTEM_GetFaderStatus() == JUTFader::Status_Out) {
            id = RCMGetCourse()->getCrsData()->searchCameraID(0x6c617374); // last
        }
        
    }
    return id;
}

void StaffRollTimeKeeper::doOpening() {
    if (isFirstFrame()) {
        GetGeoObjMgr()->getJugem(0)->setEndingState();
    
        JUTColor fadeColor(0,0,0,0xff);
        SYSTEM_GetFader()->setColor(fadeColor);
        SYSTEM_StartFadeOut(1);

        for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
            GetKartCtrl()->getKartEnemy(i)->getSpeedCtrl()->setMaxSpd(sKartMaxSpeed);
        }
    } 

    if (isKartAppearanceFrame()) {
        warpKart(1);
    }
}

void StaffRollTimeKeeper::doMarioTitle() {
    if (isFirstFrame()) {
        setBlurColor(sBlurTitleColor);
        SYSTEM_StartFadeIn(15);
        setupCreditMask(0);        
    }

    if (getCutTimer() >= 120) {
        _20 = 1;
    }

    if (isKartAppearanceFrame()) {
        warpKart(101);
        warpKart(401);
    }

    checkCreditStart(0, sStartTitleCreditTime);
}

void StaffRollTimeKeeper::doExecProducer() {
    if (isFirstFrame()) {
        setBlurColor(sBlurExProducerColor);
        setupCreditMask(1);
    }
    if (isKartAppearanceFrame()) {
        warpKart(301);
        warpKart(201);
    }

    checkCreditStart(1, sStartExectiveProducerCreditTime);
}

void StaffRollTimeKeeper::doProducer() {
    if (isFirstFrame()) {
        setBlurColor(sBlurProducerColor);
        setupCreditMask(2);
    }
    if (isKartAppearanceFrame()) {
        warpKart(501);
        warpKart(601);
        warpKart(701);
        warpKart(2);
    }

    checkCreditStart(2, sStartProducerCreditTime);
}

void StaffRollTimeKeeper::doChiefDirector() {
    if (isFirstFrame()) {
        setBlurColor(sBlurChiefDirectorColor);
        setupCreditMask(3);
    }
    if (isKartAppearanceFrame()) {
        warpKart(102);
        warpKart(302);
    }

    checkCreditStart(3, sStartChiefDirectorCreditTime);
}

void StaffRollTimeKeeper::doDirector() {
    if (isFirstFrame()) {
        setBlurColor(sBlurDirectorColor);
        setupCreditMask(4);
    }
    if (isKartAppearanceFrame()) {
        warpKart(202);
        warpKart(402);
    }

    checkCreditStart(4, sStartDirectorCreditTime);
}

void StaffRollTimeKeeper::doProgramDirector() {
    if (isFirstFrame()) {
        setBlurColor(sBlurProgramDirectorColor);
        setupCreditMask(5);
    }
    if (isKartAppearanceFrame()) {
        warpKart(502);
    }

    checkCreditStart(5, sStartProgramDirectorCreditTime);
}

void StaffRollTimeKeeper::doProgram() {
    if (isFirstFrame()) {
        setBlurColor(sBlurProgramColor);
        setupCreditMask(6);
    }
    if (isKartAppearanceFrame()) {
        warpKart(702);
        warpKart(602);
        warpKart(3);
        warpKart(103);
    }

    checkCreditStart(6, sStartProgramCreditTime);
}

void StaffRollTimeKeeper::doDesignManager() {
    if (isFirstFrame()) {
        setBlurColor(sBlurDesignManagerColor);
        setupCreditMask(7);
    }
    if (isKartAppearanceFrame()) {
        warpKart(303);
        warpKart(403);
    }

    checkCreditStart(7, sStartDesignManagerCreditTime);    
}

void StaffRollTimeKeeper::doCourseDesign() {
    if (isFirstFrame()) {
        setBlurColor(sBlurCourseDesignColor);
        setupCreditMask(8);
    }
    if (isKartAppearanceFrame()) {
        warpKart(203);
    }

    checkCreditStart(8, sStartCourseDesignCreditTime); 
}

void StaffRollTimeKeeper::doGraphicDesign() {
    if (isFirstFrame()) {
        setBlurColor(sBlurGraphicDesignColor);
        setupCreditMask(9);
    }
    if (isKartAppearanceFrame()) {
        warpKart(503);
        warpKart(603);
    }

    checkCreditStart(9, sStartGraphicDesignCreditTime);     
}

void StaffRollTimeKeeper::doScreenDesign() {
    if (isFirstFrame()) {
        setBlurColor(sBlurScreenDesignColor);
        setupCreditMask(10);
    }
    if (isKartAppearanceFrame()) {
        warpKart(4);
        warpKart(104);
    }

    checkCreditStart(10, sStartScreenDesignCreditTime);     
}

void StaffRollTimeKeeper::doMusic() {
    if (isFirstFrame()) {
        setBlurColor(sBlurMusicColor);
        setupCreditMask(11);
    }
    if (isKartAppearanceFrame()) {
        warpKart(703);
    }

    checkCreditStart(11, sStartMusicCreditTime); 
}

void StaffRollTimeKeeper::doVoice() {
    if (isFirstFrame()) {
        setBlurColor(sBlurVoiceColor);
        setupCreditMask(12);
    }
    if (isKartAppearanceFrame()) {
        warpKart(5);
        warpKart(105);
        warpKart(204);
        warpKart(304);
        warpKart(404);
        warpKart(504);
        warpKart(604);
        warpKart(704);
    }

    checkCreditStart(12, sStartVoiceCreditTime); 
}

void StaffRollTimeKeeper::doProgress() {
    if (isFirstFrame()) {
        setBlurColor(sBlurProgressColor);
        setupCreditMask(13);
    }
    if (isKartAppearanceFrame()) {
        warpKart(6);
        warpKart(106);
        warpKart(205);
        warpKart(305);
        warpKart(405);
        warpKart(505);
        warpKart(605);
        warpKart(705);
    }

    checkCreditStart(13, sStartProgressCreditTime);
}

void StaffRollTimeKeeper::doDebugSupport() {
    if (isFirstFrame()) {
        setBlurColor(sBlurDebugSupportColor);
        setupCreditMask(14);
        if (GeoCar::getCarSupervisor()) {
            GeoCar::getCarSupervisor()->hideAllCar();
        }
    }
    if (isKartAppearanceFrame()) {
        warpKart(7);
        warpKart(107);
        warpKart(206);
        warpKart(306);
        warpKart(406);
        warpKart(506);
        warpKart(606);
        warpKart(706);
    }

    checkCreditStart(14, sStartDebugSupportCreditTime);
}

void StaffRollTimeKeeper::doSpecial() {
    if (isFirstFrame()) {
        setBlurColor(sBlurSpecialColor);
        setupCreditMask(15);
    }
    if (isKartAppearanceFrame()) {
        warpKart(8);
        warpKart(108);
        warpKart(207);
        warpKart(307);
        warpKart(407);
        warpKart(507);
        warpKart(607);
        warpKart(707);
    }

    checkCreditStart(15, sStartSpecialCreditTime);
}

void StaffRollTimeKeeper::doLocalization() {
    if (isFirstFrame()) {
        setBlurColor(sBlurLocalizationColor);
        setupCreditMask(16);
    }
    if (isKartAppearanceFrame()) {
        warpKart(9);
        _24 = 1;
    }

    checkCreditStart(16, sStartLocalizationCreditTime);
}

void StaffRollTimeKeeper::doLast() {
    switch (_2c) {
    case 0: {
        if (isFirstFrame()) {
            setBlurColor(sBlurLastColor);
        }

        checkCreditStart(17, sStartLastCreditTime);
        if (getCutTimer() == 205) {
            GetGameAudioMain()->fadeOutAll(420);
        }

        if (getCutTimer() >= getCurCutTime()) {
            setLastBlanking(30);
        }
        break;
    }
    case 1: {
        if (SYSTEM_GetFaderStatus() == JUTFader::Status_Out) {
            _30++;
        }
        if (_30 >= 15) {
            _2c = 2;
            _30 = 0;
            SYSTEM_StartFadeIn(15);
            RaceMgr::getManager()->getStaffRoll2D()->start(0x13);
            RaceMgr::getManager()->getMaskScreen()->resetAllMask();

            if (GeoCar::getCarSupervisor()) {
                GeoCar::getCarSupervisor()->hideAllCar();
            }

            for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); i++) {
                RaceMgr::getManager()->getKartDrawer(i)->hide();
                GetKartCtrl()->getKartEnemy(i)->getSpeedCtrl()->setMaxSpd(0.0f);
                GetKartCtrl()->getKartEnemy(i)->getSpeedCtrl()->stop_immediately();
            }
            GetJPAMgr()->getEm2DMgr()->forceDeleteAllEmitter();
        }
        break;
    }
    case 2: {
        if (gGamePad1P.testTrigger(KartGamePad::A)) {
            setDemoEnd();
        }
        break;
    }
    
    }
}
