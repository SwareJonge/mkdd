#include "Sato/AnmController.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Sato/J3DAnmObject.h"
#include "mathHelper.h"
#include "types.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

void AnmControlTrans::registration(u8 anm_no, J3DAnmTransform *pTrans, J3DMtxCalc *pCalc) {
#line 37
    JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
    mObj[anm_no].attach(pTrans, pCalc);
}

void AnmControlTrans::registrationBlend(u8 anm_no, J3DAnmTransform *pTrans, J3DMtxCalc *pCalc) {
#line 50
    JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
    mObj[anm_no].attachBlend(pTrans, pCalc);
}

void AnmControlTrans::changeBlendAnm(u8 anm_no, u8 p2, u8 p3) {
#line 64
    JUT_MINMAX_ASSERT(0, anm_no, mMaxAnmNo);
    if (anm_no != mNowAnmNo) {
        _1c = anm_no;
        _1d = 0;
        _1e = p2;
        _1f = p3;
        mFlags |= 2;
        mObj[_1c].resetFrame();
    }
}

void AnmControlTrans::frameProc() {
    doframe(mNowAnmNo);

    if (mFlags & 2) {
        if (_1d > _1e) {
            changeAnm(_1c, false);
            if (mObj[_1c]._28 & 1) {
                mObj[_1c].mCalc->setWeight(0, 1.0f);
                for (u8 i = 1; i < 4; i++) {
                    mObj[_1c].mCalc->setWeight(i, 0.0f);
                }
            }
            mFlags &= ~2;
        }
        else {
            doframe(_1c);
            f32 tmp = (f32)_1d / (f32)_1e;
            mObj[mNowAnmNo].setWeight(0, 1.0f - tmp);
            mObj[mNowAnmNo].setWeight(_1f, tmp);
            _1d++;
        }
    }
    else {
        J3DAnmObjTrans *trans = getObj(mNowAnmNo);
        if (trans->_28 & 1) {
            mObj[mNowAnmNo].setWeight(0, 1.0f);
            for (u8 i = 1; i < 4; i++) {
                mObj[mNowAnmNo].setWeight(i, 0.0f);
            }
        }
    }
}

AnmController::AnmController() {
    mTrans = nullptr;
    mMat = nullptr;
    mCtrls = nullptr;
}

void AnmController::appendMaterial(AnmControlBase *anmObj) {
    mCtrls->append(&anmObj->mLink);
}

void AnmController::InitRegistrationMat(const MatAnmInfo &rAnmInfo, ExModel *pModel) {
    mMat = new AnmControlBase*[4];
    mMatData = new MatAnmData();

    for (u8 i = 0; i < 4; i++) {
        mMat[i] = nullptr;
    }

    mCtrls = new JSUList<AnmControlBase>;
    mCtrls->initiate();

    if (rAnmInfo.mNumBpks != 0) {
        mMatData->mpCtrlColor = new AnmControlMatColor();
        mMat[0] = mMatData->mpCtrlColor;
        mMat[0]->initAnm(rAnmInfo.mNumBpks, pModel);
        appendMaterial(mMat[0]);
    }

    if (rAnmInfo.mNumBtks != 0) {
        mMatData->mpCtrlSRT = new AnmControlMatTexSRT();
        mMat[1] = mMatData->mpCtrlSRT;
        mMat[1]->initAnm(rAnmInfo.mNumBtks, pModel);
        appendMaterial(mMat[1]);
    }

    if (rAnmInfo.mNumBtps != 0) {
        mMatData->mpCtrlPattern = new AnmControlMatTexPattern();
        mMat[2] = mMatData->mpCtrlPattern;
        mMat[2]->initAnm(rAnmInfo.mNumBtps, pModel);
        appendMaterial(mMat[2]);
    }

    if (rAnmInfo.mNumBrks != 0) {
        mMatData->mpCtrlTevReg = new AnmControlMatTevReg();
        mMat[3] = mMatData->mpCtrlTevReg;
        mMat[3]->initAnm(rAnmInfo.mNumBrks, pModel);
        appendMaterial(mMat[3]);
    }
}

void AnmController::ChangeMatAnmAll(u8) {

}

void AnmController::FrameProcTrans() {
    if (mTrans) {
        mTrans->frameProc();
    }
}

void AnmController::FrameProcMat() {
    if (mMat) {
        for (JSUListIterator<AnmControlBase> it(mCtrls->getFirst()); it != mCtrls->getEnd(); ++it) {
            it->frameProc();
        }
    }
}

void AnmController::SetAnimationTrans() {
    if (mTrans) {
        mTrans->setAnmProc();
    }
}

void AnmController::SetAnimationMat() {
    if (mMat) {
        for (JSUListIterator<AnmControlBase> it(mCtrls->getFirst()); it != mCtrls->getEnd(); ++it) {
            it->setAnmProc();
        }
    }
}

void AnmController::FrameProc() {
    FrameProcTrans();
    FrameProcMat();
}

void AnmController::SetAnimation() {
    SetAnimationTrans();
    SetAnimationMat();
}

void AnmController::Reset() {
    ResetAnm();
    ResetMat();
}

void AnmController::ResetMat() {
    if (mMat != nullptr) {
        for (JSUListIterator<AnmControlBase> it(mCtrls->getFirst()); it != mCtrls->getEnd(); ++it) {
            it->resetMatAnm();
        }
    }
}
