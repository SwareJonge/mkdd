#include "Shiraiwa/AnmPlayer.h"
#include "JSystem/J3D/J3DAnmLoader.h"
#include "JSystem/J3D/J3DFrameCtrl.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Osako/ResMgr.h"
#include "Sato/AnmController.h"
#include "Sato/J3DAnmObject.h"

TAnmPlayer::TAnmPlayer() {
    mController = nullptr;
    mAnmInfo = nullptr;
    _e = 0;
    _10 = 0;
}

TAnmPlayer::~TAnmPlayer() {}

void TAnmPlayer::resetAnimations(TAnmInfo *anmInfo, u8 n) {
    for (u8 i = 0; i < n; i++) {
        anmInfo[i].mTransAnm = nullptr;
        anmInfo[i].mCalcAnm  = nullptr;
    }
}

void TAnmPlayer::loadAnimations(TAnmInfo *anmInfo, u8 n, J3DModelData *mdlData, ResMgr::ArchiveId archiveID) {
    u8 i;
    for (i = 0; i < n; i++) {
#line 67
        void *bck = ResMgr::getPtr(archiveID, anmInfo[i].mBckName);
        JUT_ASSERT_F(bck, "load failed: %s from archive id %d", anmInfo[i].mBckName, archiveID);
        J3DAnmObjTrans::loadTransAnm(&anmInfo[i].mTransAnm, bck);
    }
    
    for (i = 0; i < n; i++) {
        s8 i2 = anmInfo[i]._12;
        if (anmInfo[i].mBlendFrameCnt && i2 < n) {
            anmInfo[i].mCalcAnm = J3DUNewMtxCalcAnm(mdlData->getMtxCalcType(), anmInfo[i].mTransAnm, anmInfo[i2].mTransAnm, nullptr, nullptr, MTXCalc_Blend);
        }
        else {
            anmInfo[i].mCalcAnm = J3DNewMtxCalcAnm(mdlData->getMtxCalcType(), anmInfo[i].mTransAnm);
        }
    }
}

void TAnmPlayer::registAnimations(AnmController *ctrl, ExModel *model, TAnmInfo *anmInfo, u8 n) {
    ctrl->InitRegistration(n, model);

    for (u8 i = 0; i < n; i++) {
        if (anmInfo[i].mBlendFrameCnt) {
            ctrl->RegisterTransBlend(i, anmInfo[i].mTransAnm, anmInfo[i].mCalcAnm);
        }
        else {
            ctrl->RegisterTrans(i, anmInfo[i].mTransAnm, anmInfo[i].mCalcAnm);
        }
        ctrl->getFrameCtrl(i)->setAttribute(anmInfo[i].mAnmCnt);
    }
}

void TAnmPlayer::init(AnmController *ctrl, TAnmInfo *info, u8 n) {
    mController = ctrl;
    mAnmInfo = info;
    mMaxAnm = n;
    reset();
}

void TAnmPlayer::reset() {
    _d = 1;
    _e = 0;
    _11 = -1;
    _14 = -1.0f;

    mController->ChangeTransAnm(0);
    mController->Reset();

    _10 = 1;
    _e = 0;
    firstEndCheck(0);
    
}

void TAnmPlayer::update() {
    if (mAnmInfo == nullptr || !_10) {
        return;
    }

    u8 anmNumber = getCurAnmNumber();
    if (!mController->IsTransAnm2()) {        
        if (_e & 1 || _e & 2 && isCurAnmReachEnd()) {
            s8 anmNo = (_11 != -1) ? _11 : mAnmInfo[getCurAnmNumber()].get_12();

            _11 = -1;
            
            if (anmNo != -1 && anmNo < mMaxAnm) {
                _d = 1;
                
                if (mAnmInfo[anmNumber].mBlendFrameCnt) {
                    mController->ChangeBlendTransAnm(anmNo, mAnmInfo[anmNumber].mBlendFrameCnt, 1);
                }
                else {
                    mController->ChangeTransAnm(anmNo);
                }
                _e = 0;
                firstEndCheck(anmNo);
            }
            else {
                _10 = 0;
            }
        }
        else {
#line 190
            JUT_MINMAX_ASSERT(0, anmNumber, mMaxAnm);

            u8 anmNo = mAnmInfo[anmNumber]._11;
            if (anmNo != 0 && isGoArroundLoop()) {
                ++_d;
                if (_d >= anmNo - 1) {
                    _10 = 1;
                    _e |= 2;
                }
            }
        }
    }

    _14 = mController->getFrameCtrl(getCurAnmNumber())->getFrame();
}

void TAnmPlayer::firstEndCheck(u8 anmNumber) {
#line 210
    JUT_MINMAX_ASSERT(0, anmNumber, mMaxAnm);
    if (mAnmInfo[anmNumber]._11 == 1 || mAnmInfo[anmNumber].mAnmCnt == 0) {
        _10 = 1;
        _e |= 2;
    }
}

bool TAnmPlayer::isCurAnmReachEnd() {
    bool ret = false;
    if (mController->getCurFrameCtrl()->checkState(1) || checkFrameEnd() || isFitBlendFrame()) {
        ret = true;
    }
    return ret;
}

bool TAnmPlayer::checkFrameEnd() {
    bool ret = false;

    if (_14 == -1.0f) {
        return false;
    }

    J3DFrameCtrl *ctrl = mController->getFrameCtrl(getCurAnmNumber());

    switch (mAnmInfo[getCurAnmNumber()].mAnmCnt) {
    case 0: {
        if (ctrl->getFrame() > ctrl->getEnd() - ctrl->getRate()) {
            ret = true;
        }
        break;
    }
    case 1: {
        break;
    }
    case 2: {
        if ((ctrl->getFrame() - _14) * ctrl->getRate() < 0.0f) {
            ret = true;
        }
        break;
    }
    }
    return ret;
}

bool TAnmPlayer::isFitBlendFrame() {
    bool ret = false;
    u8 anmNumber = getCurAnmNumber();
#line 268
    JUT_MINMAX_ASSERT(0, anmNumber, mMaxAnm);

    if (mAnmInfo[anmNumber].mBlendFrameCnt) {
        f32 frame = mController->getFrameCtrl(getCurAnmNumber())->getFrame();
        f32 end = mController->getFrameCtrl(getCurAnmNumber())->getEnd();
        if (end - frame <= mAnmInfo[anmNumber].mBlendFrameCnt) {
            ret = true;
        }
    }

    return ret;
}

bool TAnmPlayer::isGoArroundLoop() {
    return mController->getFrameCtrl(getCurAnmNumber())->checkState(2);
}
