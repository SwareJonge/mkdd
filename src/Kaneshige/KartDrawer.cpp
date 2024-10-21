#include <dolphin/mtx.h>

#include "Kaneshige/KartDrawer.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/DarkAnmMgr.h"
#include "Kaneshige/DrawBuffer.h"
#include "Kaneshige/ExModel.h"
#include "Kaneshige/KartInfo.h"
#include "Kaneshige/KartLoader.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/EffectScreen.h"
#include "Yamamoto/kartCtrl.h"

#include "kartEnums.h"
#include "mathHelper.h"
#include "JSystem/JAudio/JASFakeMatch2.h"


s16 KartDrawer::sFlashInterval = 6;
u8 KartDrawer::sGhostA = 150;
s16 KartDrawer::sGhostFadeOutTime = 60;

void KartDrawer::DriverDB::drawIn(u32 viewNo) {
    if (mDriverNo == 0)
        KartCtrl::getKartCtrl()->SetDriverCurrentViewNo(mKartNo, viewNo);
    else
        KartCtrl::getKartCtrl()->SetCoDriverCurrentViewNo(mKartNo, viewNo);

    ExModel *modelDriver = RCMGetKartLoader(mKartNo)->getExModelDriver(mDriverNo);
    if (modelDriver->isAllShapePacketHidding(mLevelNo, viewNo))
        clrDraw();
}

void KartDrawer::DriverDB::update() {
    DrawBuffer::update();
    if (mDriverNo == 0)
        KartCtrl::getKartCtrl()->DrwaDriver(mKartNo, mLevelNo);
    else
        KartCtrl::getKartCtrl()->DrwaCoDriver(mKartNo, mLevelNo);

    DrawBuffer::lock();
}

KartDrawer::KartDrawer() {
    mKartNo = -1;
    mMaxViewNo = 0;
    mEnableLOD = true;
    mStartStage = STAGE_1;
    mStage = mStartStage;
    mGhostAlpha = sGhostA;
    mGhostFadeTimer = -1;
    mHiddenDrivers = 0;
    mMaxLevel = 1;
    mpLOD = nullptr;
    _20 = nullptr;
    _24 = nullptr;
    mOsageDB = nullptr;
    mFlashState = 0;
    mFlashTime = 0;

    for(int i = 0; i < 4; i++)
        mKartLight[i] = nullptr;

    for(int i = 0; i < 2; i++)
        _40[i] = nullptr;
    
    _48 = nullptr;
    mAnmNo = 2;
}

void KartDrawer::reset() {
    mEnableLOD = true;
    mStage = mStartStage;
    mGhostAlpha = sGhostA;
    mGhostFadeTimer = -1;
    mHiddenDrivers = 0;

    _20->reset();
    _24->reset();
    mOsageDB->reset();

    mFlashState = 0;
    mFlashTime = 0;

    for(int i = 0; i < 2; i++)
        _40[i]->reset();
    
    _48->reset();
    resetAnimation();
}

void KartDrawer::create(int kartNo, int numCams, u16 numLevels, EGhostKind ghostKind) {
    mKartNo = kartNo;
    mMaxViewNo = numCams;
    switch (ghostKind) {
    case KIND_0:
        mStartStage = STAGE_1;
        break;
    case KIND_NORMAL_GHOST:
        mStartStage = STAGE_GHOST;
        break;
    case KIND_STAFF_GHOST:
        mStartStage = STAGE_GHOST;
        break;
    default:
#line 443
        JUT_ASSERT_MSG(0, "UNKNOWN GHOST KIND");
        break;
    }
    
    mStage = mStartStage;
    mMaxLevel = numLevels;

    mpLOD = new u16[numCams];
    for (int i = 0; i < mMaxViewNo; i++)
        mpLOD[i] = nullptr;

    _20 = new DriverDB[mMaxLevel];
    _24 = new DriverDB[mMaxLevel];

    for (int i = 0; i < mMaxLevel; i++) {
        _20[i].create(8);
        _20[i].setKartNo(mKartNo);
        _20[i].setDriverNo(0);
        _20[i].setLevelNo(i);

        _24[i].create(8);
        _24[i].setKartNo(mKartNo);
        _24[i].setDriverNo(1);
        _24[i].setLevelNo(i);
    }

    mOsageDB = new OsageDrawBuffer(4);
    mOsageDB->setTargetKart(kartNo);

    for (int i = 0; i < 4; i++)
        mKartLight[i] = nullptr;

    KartInfo *kartInfo = RaceMgr::getManager()->getKartInfo(mKartNo);

    for (int i = 0; i < 2; i++) {
        _40[i] = new DarkAnmPlayer(kartInfo->getDriverCharID(i), ghostKind);
    }

    _48 = new DarkAnmPlayer(kartInfo->getKartID(), ghostKind);

}

//void OsageDrawBuffer::setTargetKart(short) {}

//void OsageDrawBuffer::OsageDrawBuffer(u32) {}


//void DrawBuffer::create(u32) {}

//void KartDrawer::DriverDB::~DriverDB() {}

//void KartDrawer::DriverDB::DriverDB() {}

void KartDrawer::setAnimation() {
    KartLoader *kartLoader = RaceMgr::getManager()->getKartLoader(mKartNo);
    
    for (int i = 0; i < 2; i++) {
        mDriverAnm[i].setExModel(kartLoader->getExModelDriver(i));
        
        if (kartLoader->isOsageExist(i)) {
            mAccessoryAnm[i].setExModel(kartLoader->getExModelOsage(i));
        }
    }
    
    mBodyAnm.setExModel(kartLoader->getExModelBody());
    mArmAnm.setExModel(kartLoader->getExModelArm(0));
    mWheelLAnm.setExModel(kartLoader->getExModelWheel(0));
    mWheelRAnm.setExModel(kartLoader->getExModelWheel(1));

    if (kartLoader->isShockExist())
        mShockAnm.setExModel(kartLoader->getExModelShock(0));

    resetAnimation();
}

void KartDrawer::resetAnimation() {
    mShadowID = 0;
    mShadowRate = 0.0f;
    setTevAnm(-1);
}

void KartDrawer::setTevAnm(int anmNo) {
    KartLoader *kartLoader = RaceMgr::getManager()->getKartLoader(mKartNo);
    bool diffAnmNo = false;
    if (mAnmNo != anmNo)
        diffAnmNo = true;

    if (diffAnmNo) {
        if (mAnmNo < 2) 
            kartLoader->removeTevRegAnimator(mAnmNo);
        
        kartLoader->entryTevRegAnimator(anmNo);
        mAnmNo = anmNo;
    }
    
    attach(mAnmNo);
    switch (mAnmNo) {
    case 0:
        setAttribute(J3DAA_UNKNOWN_4);
        resetFrame();
        setRate(1.0f);
        break;
    default:
        resetFrame();
        setRate(1.0f);
        break;
    }
}

void KartDrawer::attach(int tevanmID) {
    KartLoader *kartLoader = RaceMgr::getManager()->getKartLoader(mKartNo);
    if (kartLoader->isDriverTevAnmEnable(tevanmID)) {
        for (int i = 0; i < 2; i++) {
            if (kartLoader->getDriverTevRegKey(i, tevanmID) != nullptr)
                mDriverAnm[i].attach(kartLoader->getDriverTevRegKey(i, tevanmID));
            if (kartLoader->getAccessoryTevRegKey(i, tevanmID) != nullptr)
                mAccessoryAnm[i].attach(kartLoader->getAccessoryTevRegKey(i, tevanmID));
        }
    }

    if (kartLoader->isKartTevAnmEnable(tevanmID)) {
        mBodyAnm.attach(kartLoader->getBodyTevRegKey(tevanmID));
        mArmAnm.attach(kartLoader->getArmTevRegKey(tevanmID));
        mWheelLAnm.attach(kartLoader->getWheelLTevRegKey(tevanmID));
        mWheelRAnm.attach(kartLoader->getWheelRTevRegKey(tevanmID));

        if (kartLoader->isShockExist())
            mShockAnm.attach(kartLoader->getShockTevRegKey(tevanmID));
    }
}

void KartDrawer::setAttribute(J3DAnmAttr attr) {
    for (int i = 0; i < 2; i++) {
        mDriverAnm[i].getFrameCtrl()->setAttribute(attr);
        mAccessoryAnm[i].getFrameCtrl()->setAttribute(attr);
    }

    mBodyAnm.getFrameCtrl()->setAttribute(attr);
    mArmAnm.getFrameCtrl()->setAttribute(attr);
    mWheelLAnm.getFrameCtrl()->setAttribute(attr);
    mWheelRAnm.getFrameCtrl()->setAttribute(attr);

    if (isAvailableTevAnmShock())
        mShockAnm.getFrameCtrl()->setAttribute(attr);
}

void KartDrawer::resetFrame() {
    for (int i = 0; i < 2; i++) {
        mDriverAnm[i].resetFrame();
        mAccessoryAnm[i].resetFrame();
    }

    mBodyAnm.resetFrame();
    mArmAnm.resetFrame();
    mWheelLAnm.resetFrame();
    mWheelRAnm.resetFrame();

    if (isAvailableTevAnmShock())
        mShockAnm.resetFrame();
}

void KartDrawer::setRate(f32 rate) {
    for (int i = 0; i < 2; i++) {
        mDriverAnm[i].setRate(rate);
        mAccessoryAnm[i].setRate(rate);
    }

    mBodyAnm.setRate(rate);
    mArmAnm.setRate(rate);
    mWheelLAnm.setRate(rate);
    mWheelRAnm.setRate(rate);;

    if (isAvailableTevAnmShock())
        mShockAnm.setRate(rate);
}

void KartDrawer::kartAnmFrameProc() {
    mBodyAnm.anmFrameProc();
    mArmAnm.anmFrameProc();
    mWheelLAnm.anmFrameProc();
    mWheelRAnm.anmFrameProc();;

    if (isAvailableTevAnmShock())
        mShockAnm.anmFrameProc();
}

void KartDrawer::setFrame(f32 rate) { // UNUSED
    for (int i = 0; i < 2; i++) {
        mDriverAnm[i].setFrame(rate);
        mAccessoryAnm[i].setFrame(rate);
    }

    mBodyAnm.setFrame(rate);
    mArmAnm.setFrame(rate);
    mWheelLAnm.setFrame(rate);
    mWheelRAnm.setFrame(rate);;

    if (isAvailableTevAnmShock())
        mShockAnm.setFrame(rate);
}

void KartDrawer::frameProc() {
    for (int i = 0; i < 2; i++) {
        mDriverAnm[i].frameProc();
        mAccessoryAnm[i].frameProc();
    }

    mBodyAnm.frameProc();
    mArmAnm.frameProc();
    mWheelLAnm.frameProc();
    mWheelRAnm.frameProc();

    if (isAvailableTevAnmShock())
        mShockAnm.frameProc();
}

void KartDrawer::setShadowInfo(const CrsArea &area) {
    mShadowID = area.getShadowID();
    mShadowRate = area.getShadowRate();
}

bool KartDrawer::isFlashHidding() { return isFlashing() && ((mFlashTime) / (u16)sFlashInterval & 1); }

bool KartDrawer::enableDrawing(u32 viewNo, EDrawStage drawStage) {
    KartInfo *kartInfo = RaceMgr::getManager()->getKartInfo(mKartNo);
    if (!kartInfo->isAvailableKart())
        return false;

    bool doDrawStage = mStage == drawStage;
    if (drawStage == STAGE_0)
        doDrawStage = true;
    if (!doDrawStage)
        return false;

    if (isHide(viewNo))
        return false;

    return isFlashHidding() == false;
}

void KartDrawer::drawKart(u32 viewNo, EDrawStage drawStage) {
    if (!enableDrawing(viewNo, drawStage))
        return;

    RaceKartLight *kartLight = mKartLight[viewNo];
    kartLight->draw();

    MtxPtr effectMtx = nullptr;
    if (kartLight) // hm, if you're concerned about undefined behaviour, then also put the draw function here?
        effectMtx = kartLight->getEffectMtx();
    
    if (mBodyAnm.getAnmBase())
        kartAnmFrameProc();

    KartCtrl::getKartCtrl()->DrwaBody(mKartNo, viewNo, effectMtx);
    KartCtrl::getKartCtrl()->DrwaParts(mKartNo, viewNo, effectMtx);
}

void KartDrawer::drawDriver(u32 viewNo, EDrawStage drawStage) {
    if (!enableDrawing(viewNo, drawStage))
        return;

    mKartLight[viewNo]->draw();

    for (int i = 0; i < 2; i++) {
        if (mDriverAnm[i].getAnmBase())
            mDriverAnm[i].anmFrameProc();

        if (mAccessoryAnm[i].getAnmBase())
            mAccessoryAnm[i].anmFrameProc();
    }

#line 823
    JUT_MINMAX_ASSERT(0, viewNo, mMaxViewNo);
    u16 level = mpLOD[viewNo];
    if (level >= mMaxLevel)
        level = mMaxLevel - 1;

#line 834
    JUT_MINMAX_ASSERT(0, level, mMaxLevel);
    _20[level].draw(viewNo);
    _24[level].draw(viewNo);
    mOsageDB->draw(viewNo);
}

void KartDrawer::update() {
    KartInfo *kartInfo = RaceMgr::getManager()->getKartInfo(mKartNo);
    if (!kartInfo->isAvailableKart())
        return;

    if (mStage != STAGE_GHOST) {
        if (GetEfctScreenMgr()->IsContinue(EffectScreen::Kind_1))
            mStage = STAGE_2;
        else
            mStage = STAGE_1;
    }

    if (isFlashing())
        mFlashTime++;

    if (mGhostFadeTimer >= 0 && mGhostFadeTimer < sGhostFadeOutTime) {
        mGhostFadeTimer++;
        if (mGhostFadeTimer < sGhostFadeOutTime) {
            mGhostAlpha = sGhostA - sGhostA * mGhostFadeTimer / sGhostFadeOutTime;
        }
        else {
            mGhostAlpha = 0;
            hide();
        }
    }

    KartLoader *kartLoader = RCMGetKartLoader(mKartNo);
    for (int i = 0; i < 2; i++) {
        _40[i]->calc(mShadowID, mShadowRate);
        _40[i]->setTevColor(kartLoader->getExModelDriver(i));

        if (kartLoader->isOsageExist(i))
            _40[i]->setTevColor(kartLoader->getExModelOsage(i));
    }

    _48->calc(mShadowID, mShadowRate);
    _48->setTevColor(kartLoader->getExModelBody());

    for (int i = 0; i < kartLoader->getWheelNumber(); i++) {
        _48->setTevColor(kartLoader->getExModelWheel(i));
        _48->setTevColor(kartLoader->getExModelArm(i));
        if (kartLoader->isShockExist())
            _48->setTevColor(kartLoader->getExModelShock(i));
    }

    for (u16 i = 0; i < mMaxLevel; i++) {
        _20[i].update();
        _24[i].update();
    }
    mOsageDB->update();

    if (mBodyAnm.getAnmBase())
        frameProc();
}

void KartDrawer::setLODLevel(u32 viewNo, u16 level) {
    if (viewNo >= mMaxViewNo)
        return;

    if (mEnableLOD)
        mpLOD[viewNo] = level;
    else
        mpLOD[viewNo] = 0;
    // mpLOD[viewNo] = mEnableLOD ? level : 0;
}

void KartDrawer::playTevAnm(int anmNo) { setTevAnm(anmNo); }

void KartDrawer::stopTevAnm() { setTevAnm(-1); }
