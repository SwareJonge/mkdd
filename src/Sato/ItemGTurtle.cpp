#include "JSystem/JGeometry/Matrix.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "JSystem/JParticle/JPAMath.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/CenterLine.h"
#include "Kaneshige/Course/CrsGround.h"
#include "Kaneshige/KartChecker.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/GeographyObj.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ItemObj.h"
#include "Sato/ItemSuccession.h"
#include "Sato/ItemTurtle.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/ObjUtility.h"
#include "Sato/stMath.h"
#include "Yamamoto/kartCtrl.h"
#include "dolphin/mtx.h"
#include "dolphin/vi.h"
#include "kartEnums.h"
#include "math.h"
#include "mathHelper.h"
#include "types.h"
#include <std/math.h>



namespace {
JGeometry::TVec3s sBuildRotAngle(0, 0, 0);

f32 sRotYawRatio = 0.15f;
f32 sQtLerpRatio = 0.1f;
f32 sGravity = -3.5f;
f32 sVelMag = 100.0f;
f32 sSpVelMag = 20.0f;
f32 sHeightOffset = 25.0f;
f32 sBoundRatio = 0.5f;
f32 sScale = 3.0f;
f32 sVelDownRatio = 0.99f;
f32 sVelUpRatio = 1.05f;
u8 sMaxNoColCnt = 5;
u8 sFMaxNoColCnt = 10;
u8 sReleaseOffsetCnt = 20;
f32 sReleaseYRatio = -0.3f;
f32 sBoundsRadius = 30.0f;
f32 sScaleEquip = 1.0f;
f32 sScaleUpRatio = 1.1f;
f32 sScaleUpRatioFall = 1.02f;
f32 sSucReleaseVelMag = 35.0f;
f32 sFirstRotAdd = 0.35f;
f32 sMaxGndNormAngle = 20.0f;
f32 sGndNormalRatio = 0.1f;
f32 sUseAllAngle = 5.0f;
u8 sMaxColWallNum = 10;
u8 sMiniGameMaxColWallNum = 4;
f32 sMaxVel = 150.0f;

GameDiffParam sGameDifParamNormal = {
    sScale, sVelMag, sSpVelMag, sVelUpRatio, sVelDownRatio, sMaxNoColCnt, sBoundsRadius, sReleaseYRatio
};

GameDiffParam sGameDifParamMiniGreen = {
    5.0f, 200.0f, 50.0f, 1.0f, 1.0f, 3, 50.0f, 0.1f
};

GameDiffParam sGameDifParamMiniGreenBig = {
    sScale, sVelMag, sSpVelMag, sVelUpRatio, sVelDownRatio, sMaxNoColCnt, sBoundsRadius, 0.1f
};

GameDiffParam sGameDifParamMiniRed = {
    5.0f, 0.0f, 50.0f, sVelUpRatio, sVelDownRatio, 3, 50.0f, 0.1f
};
}

JGeometry::TVec3f ItemGTurtle::sHandOffsetPos(16.f, 0.f, -26.f);

ItemGTurtle::ItemGTurtle(u32 kind) : ItemObj(kind) {
    reset();
    mJuggle = new ItemJuggle(this);
    mBuild = new ItemBuild(this);
}

void ItemGTurtle::reset() {
    resetObject();
    mKartReaction = 4;
    _2b0 = 0.0f;
    _2b4 = false;
    mCompVec.zero();
    _2c4 = 0.0f;
    mFirstRotate = true;
    _2cc = 0.0f;
    mXDir.zero();
    _2dc = 0;
    _2dd = 0;
    _2de = 0;
    _2df = 0;
    _2e0 = 0;
    _2e1 = 0;

    _28 = sBoundRatio;
    mLerpRatio = sQtLerpRatio;
    mHeightOffset = sHeightOffset;
    _2c = 1.0f;
    _12c |= 0x8000;
    mBlurEmt = nullptr;

    mGameDifParam = &sGameDifParamNormal;
    if (mItemKind == 0 && GetGeoObjMgr()->isMiniGame()) {
        mGameDifParam = &sGameDifParamMiniGreen;
    }

    if (mBounds) {
        mBounds->setRadius(mGameDifParam->mRadius);
    }
}

const char *ItemGTurtle::getBmdFileName() {
    static const char *cGTurtleBmdName = "/Item/item_koura_green.bmd";
    return cGTurtleBmdName;
}

void ItemGTurtle::createModel(JKRSolidHeap *, u32, u32) {}

void ItemGTurtle::doUniv() {
    if (!tstAnyObjectHitting()) {
        switch (mState)
        {
        case 1:
        case 5:
            doEquipScaleUp(sScaleEquip);
            break;
        case 4:
        case 6:
            doFallScaling();
            setScaleCol(mEquipScale);
            break;
        case 7:
            doForceDisappearScaleDown();
            break;
        default:
            f32 scaleUpRatio = sScaleUpRatio;
            if (mState == 4) {
                scaleUpRatio = sScaleUpRatioFall;
            }
            if (mEquipScale < mGameDifParam->_00) {
                mEquipScale *= scaleUpRatio;
            }
            setScaleCol(mEquipScale);
            break;
        }
    }

    (this->*mStateFunc)();
    doSound(0x40001);
}

ItemObj::ItemColFlag ItemGTurtle::calcColReaction(ItemObj *) {
    return cColFlag_4;
}

void ItemGTurtle::callHitItemSound(ItemObj *obj) {
    if (obj->mItemKind == 3 || obj->mItemKind == 4) {
        mSoundMgr->setSe(0x40060);
    }
}

void ItemGTurtle::initEquip() {
    mEquipFlags = 0;
    mVel.zero();
    mPos.zero();
    mMulti->ParamInit(sBuildRotAngle);

    _2b0 = (getItemRnd()->get_ufloat_1() * 2.0f - 1.0f) * F_PI;
    QuaternionReset(&_34);
}

void ItemGTurtle::doEquip() {
    executeEquipHandFunc();
}

void ItemGTurtle::initRelease() {
    doUnivInitRelease(mGameDifParam->_04, false);
}

void ItemGTurtle::doUnivInitRelease(f32 maxVel, bool reverse) {
    mMaxVel = maxVel;
    MtxPtr bodyMtx = ObjUtility::getKartBodyMtx(mOwnerNum);
    stMtx2Qt(&_44, bodyMtx);
    f32 throwAng = mGameDifParam->_1c;
    if (mSuccessionItemLink.getList()) {
        throwAng *= 0.5f;
    }
    setInitThrowDir(&mVel, throwAng, true);
    mVel.scale(mMaxVel);
    
    JGeometry::TVec3f kartVel;
    ObjUtility::getKartVel(mOwnerNum, &kartVel);
    mVel.add(kartVel);
    if (!reverse) {
        JGeometry::TVec3f velStickX;
        getVelStickXThrow(&velStickX, 35.0f, 0.1f);
        mVel.add(velStickX);
    }
    else {
        mVel.x = -mVel.x;
        mVel.z = -mVel.z;
    }
    setInitReleaseParam();
    if (_2de) {
        calcAllUseInitVel(_2dd, mSuccessionParent->get_1fc() > 3 ? sUseAllAngle : sUseAllAngle * 2.0f);
    }

}

void ItemGTurtle::setInitReleaseParam() {
    mEquipFlags = 0xffffffff;
    mEquipFlags &= ~(1 << mOwnerNum);
    _12c |= 2;
    _12c |= 1;
    _2b4 = false;
    initMoveColGround();

    u8 maxNoCol = mGameDifParam->_14;
    if (mItemKind == 9) {
        maxNoCol = sFMaxNoColCnt;
    }
    setFrameNoColKart(maxNoCol, false);
    mpHandAnmMtx = getHandAnmMtx();
    initFirstRotate();

    f32 length = 0.0f;
    getCompVecLength(mCompVec, length);
    mCompVec.scale(length / sReleaseOffsetCnt);
    mpHandAnmMtx = getHandAnmMtx();
    initBlurEmt();
}

void ItemGTurtle::initBlurEmt() {
    switch (mItemKind) {
    case 0:
        mBlurEmt = GetJPAMgr()->createEmt("mk_kouraBlur_g", mPos);
        break;
    case 2:
        mBlurEmt = GetJPAMgr()->createEmt("mk_kouraBlur_r", mPos);
        break;
    }
}

void ItemGTurtle::doUnivRelease() {
    if (mWaitFrame < sReleaseOffsetCnt) {
        mPos.add(mCompVec);
    }

    if (_2de) {
        checkStartColItemFrame(0xf);
    }

    setVelUpDown(mGameDifParam->_0c, mGameDifParam->_10);
    mVel.y += sGravity;
    calcVel();
    doMovePos();



    Quaternion q;
    if (calcRotQuat(&q)) {
        doMoveColGround(&q, false);
    }
    else {
        doMoveColGround(nullptr, false);
    }

    if (mTransferFlags & 4) {
        chkColWallDisappear();
    }

    if (mBlurEmt) {
        mBlurEmt->setGlobalTranslation(mPos);
    }

    if (mVel.length() > sMaxVel) {
        stVecNormalize(mVel);
        mVel.scale(sMaxVel);
    }
}

bool ItemGTurtle::calcRotQuat(Quaternion *q) {
    rotationRad(_2b0, sRotYawRatio);
    QuatRotY(_2b0, q);
    return true;
}

void ItemGTurtle::calcGndUpdateQuat() {
    if (!mFirstRotate) {
        JGeometry::TVec3f up;
        getModelUpDir(&up);
        if (!up.equals(mNormal)) {
            stVec2QtUpdate(_34, _44, up, mNormal);
        }
    }
    else {
        doFirstRotate();
    }    
}

void ItemGTurtle::calcFlyingUpdateQuat() {
    if (!mFirstRotate) {
        calcBalanceFlying();
        _2b4 = false;
    }
    else {
        doFirstRotate();
    }
}

const bool ItemGTurtle::chkColWallDisappear() {
    bool ret = false;
    if (mState == 8) {
        return true;
    }

    u8 colWallNum = getMaxColWallNum();
    if (mItemKind != 9 && mState == 3) {
        colWallNum *= 0.5f; 
    }

    if (_2df < colWallNum) {
        _2df++;
    }

    if (_2df >= colWallNum) {
        mPos = _10;
        setStateDisappear(true);
        ret = true;
    }
    else if (mItemKind != 9) {
        mSoundMgr->setSe(0x4005f);
    }
    return ret;
}

void ItemGTurtle::moveNormal(CrsGround &ground) {
    f32 moveY = mDisScaleAnm.y * mHeightOffset + ground.getHeight();

    if (mPos.y < moveY) {
        mTransferFlags |= 2;
        mTransferFlags |= 0x100;

        mPos.y = moveY;
        f32 velY = mVel.y;
        if (velY < 0.0f) {
            velY = -velY;
            JGeometry::TVec3f norm;
            ground.getNormal(&norm);
            JGeometry::TVec3f moveDir;
            moveDir.scale(norm.dot(mVel), norm);
            mVel.y -= moveDir.y;
            velY *= _28;
            mVel.y += velY;
        }

        f32 len = mVel.length();
        f32 vel = len < mMaxVel ? mMaxVel : len;

        if (_2b4 && vel > 0.0f) {
            JGeometry::TVec3f cp;
            JGeometry::TVec3f up(0.0f, 1.0f, 0.0f);
            calcMoveGndNormalVec(&up, ground);
            
            cp.cross(up, mVel);
            if (cp.length() > 0.0f) {
                cp.normalize();

                JGeometry::TVec3f cp2;
                cp2.cross(cp, mNormal);
                if (cp2.length() > 0.0f) {
                    cp2.normalize();
                    cp2.scale(vel);
                    mVel = cp2;
                }
            }
        }
        _2b4 = true;
        calcGndUpdateQuat();
    }
    else {
        calcFlyingUpdateQuat();
    }

    _2e0 = (mTransferFlags >> 1) & 1;
}

void ItemGTurtle::calcMoveGndNormalVec(JGeometry::TVec3f *out, CrsGround &ground) {
    ItemObj::calcGndNormal(out, ground, sMaxGndNormAngle, sGndNormalRatio);
}

void ItemGTurtle::doRelease() {
    doUnivRelease();
}

void ItemGTurtle::initSpRelease() {
    doUnivInitRelease(mGameDifParam->_08, true);
}

void ItemGTurtle::doSpRelease() {
    doUnivRelease();
}

void ItemGTurtle::initDisappear() {
    mEquipFlags = 0;
    _12c &= ~1;
    _12c |= 0x80;

    if (mItemKind == 0) {
        GetJPAMgr()->createEmt("mk_koura_hahen", mPos);
    }
    else if (mItemKind == 2) {
        GetJPAMgr()->createEmt("mk_koura_hahen_r", mPos);
    }
    GetJPAMgr()->createEmt("mk_koura_smoke", mPos);

    mSoundMgr->setSe(0x4002c);
}

void ItemGTurtle::initForceDisappear() {
    deleteEffect();
    ItemObj::initForceDisappear();
}

void ItemGTurtle::doDisappear() {
    if (mBlurEmt) {
        mBlurEmt->setGlobalAlpha(255 - (mWaitFrame * 51));
    }

    if (mWaitFrame >= 5) {
        deleteEffect();
        mState = 10;
        initEnd();
    }
}

void ItemGTurtle::initCommonMoveFallMtx() {
    _2b0 = 0.0f;
    _2e1 = 0;
    _2b4 = false;
    ItemObj::initCommonMoveFallMtx();
}

void ItemGTurtle::doFall() {
    ItemObj::doFall();
}

void ItemGTurtle::doDivested() {
    moveDivested();
}

void ItemGTurtle::calcBalanceFlying() {
    QuaternionReset(&_44);
}

u8 ItemGTurtle::getMaxColWallNum() {
    if (!GetGeoObjMgr()->isMiniGame()) {
        return sMaxColWallNum;
    }
    return sMiniGameMaxColWallNum;
}

void ItemGTurtle::initFirstRotate() {
    if (mFirstRotate) {
        _2cc = 0.0f;
        ObjUtility::getKartXdir(mOwnerNum, &mXDir);
        if (mState == 3) {
            mXDir.negate();
        }
    }
}

void ItemGTurtle::doFirstRotate() {
    if (mFirstRotate) {
        _2cc += sFirstRotAdd;
        JGeometry::TPos3f m;
        m.setRotate(mXDir, _2cc);
        m.zeroTrans();
        stMtx2Qt(&_44, m);
        if (_2cc >= F_TAU) {
            mFirstRotate = false;
        }
    }
}

void ItemGTurtle::doSound(u32 soundID) {
    if (mSoundMgr) {
        if (mState == 2 || mState == 3) {
            mSoundMgr->setSe(soundID);
        }
    }
}

void ItemGTurtle::deleteEffect() {
    if (mBlurEmt) {
        GetJPAMgr()->deleteEmt(mBlurEmt);
        mBlurEmt = nullptr;
    }
}

void ItemGTurtle::executeEquipHandFunc() {
    mMulti->MoveEquip();
}

void ItemGTurtle::setGeoHitSe(GeographyObj *obj)  {
    if (obj->getItemReaction().tstFlg(mItemKind) == 1) {
        mSoundMgr->setSe(0x4005f);
    }
}

bool ItemGTurtle::countHitGeoReflectNum(GeographyObj *obj) {
    bool ret = false;
    if (obj->getItemReaction().tstFlg(mItemKind) == 1) {
        ret = true;
    }
    return ret;
}

void ItemGTurtle::executeGeoObjHitCallBack(GeographyObj *obj) {
    if (countHitGeoReflectNum(obj)) {
        if (chkColWallDisappear()) {
            return;
        }
    }
    setGeoHitSe(obj);
    obj->getItemReaction().tstFlg(mItemKind);
}

namespace {
f32 sScaleBigEquip = 0.5f;
f32 sScaleBig = 4.0f;
f32 sScaleBigUpRatio = 1.1f;
f32 sBoundsRadiusBig = 80.0f;
f32 sHeightOffsetBig = 70.0f;
f32 sQtLerpRatioBig = 0.1f;
f32 sScaleKartColDownRatio = 0.9f;
u8 sMaxColWallNumBig = 8;
u8 sMiniGameMaxColWallNumBig = 4;
f32 sShadowOffsetScaleBig = 3.0f;
f32 sEfctScaleOffsetBig = 3.0f;
u8 sBigMaxHitSoundWaitFrame = 30;
f32 sBigShadowScaleY = 0.5f;

}

ItemGTurtleBig::ItemGTurtleBig(u32 kind) : ItemGTurtle(kind) {
    mShadowOffsetScale = sShadowOffsetScaleBig;
    mShadowScaleY = sBigShadowScaleY;
}

void ItemGTurtleBig::reset() {
    ItemGTurtle::reset();

    if (GetGeoObjMgr()->isMiniGame()) {
        mGameDifParam = &sGameDifParamMiniGreenBig;
    }

    mKartReaction = 0xb;
    _2b0 = 0.0f;
    _2ec = 0;
    if (mBounds) {
        mBounds->setRadius(sBoundsRadiusBig);
    }
    mLerpRatio = sQtLerpRatioBig;
    mHeightOffset = sHeightOffsetBig;
}

const char *ItemGTurtleBig::getBmdFileName() {
    static const char *cGTurtleBigBmdName = "/Item/item_koura_koopa.bmd";
    return cGTurtleBigBmdName;
}

void ItemGTurtleBig::doUniv() {
    JGeometry::TVec3f kartPos;
    ObjUtility::getKartPos(mOwnerNum, &kartPos);
    CrsGround ground(RCMGetCourse());
    ground.search(kartPos);
    JGeometry::TVec3f pos(mPos);
    f32 h = ground.getHeight();
    f32 maxScale = sScaleBig;
    if (!tstAnyObjectHitting()) {
        if (mState == 1 || mState == 5) {
            doEquipScaleUp(sScaleBigEquip);
        }
        else if (mState == 7) {
            doForceDisappearScaleDown();
        }
        else {
            if (mEquipScale < maxScale) {
                mEquipScale *= sScaleBigUpRatio;
                if (mEquipScale >= maxScale) {
                    mEquipScale = maxScale;
                }
            }
            else if (mEquipScale > maxScale) {
                mEquipScale *= sScaleKartColDownRatio;
                if (mEquipScale <= maxScale) {
                    mEquipScale = maxScale;
                }
            }
            setScaleCol(mEquipScale);
        }
    }

    (this->*mStateFunc)();
    doSound(0x40003);

    if (_2ec != 0) {
        _2ec--;
    }
}

void ItemGTurtleBig::initEquip() {
    ItemGTurtle::initEquip();
    mEquipScale = sScaleBigEquip;
}

void ItemGTurtleBig::initRelease() {
    doUnivInitRelease(sVelMag, false);
}

void ItemGTurtleBig::initSpRelease() {
    doUnivInitRelease(sSpVelMag, true);
}

void ItemGTurtleBig::initDisappear() {
    mEquipFlags = 0;
    _12c &= ~1;
    _12c |= 0x80;
    char *inputName = "mk_koopaKoura_out";
    for (u8 i = 0; i < 3; i++) {
        const char *filename = ObjUtility::concatSimpleFileName(inputName, i);
        JPABaseEmitter *emt = GetJPAMgr()->createEmt(filename, mPos);
        if (emt) {
            emt->setGlobalScale(mDisScaleAnm);
        }
    }

    mSoundMgr->setSe(0x4003f);
}

ItemObj::ItemColFlag ItemGTurtleBig::calcColReaction(ItemObj *obj) {
    ItemColFlag ret = cColFlag_5;
    if (mState == 4 && obj->mItemKind != 1)  {
        return cColFlag_5;
    }
    
    switch (obj->mItemKind) {
    case 4:
    case 8:
        ret = cColFlag_3;
        break;
    case 1:
    case 7:
    case 9:
    case 11:
        ret = cColFlag_0;
        break;
    }
    return ret;    
}

u8 ItemGTurtleBig::getMaxColWallNum() {
    if (!GetGeoObjMgr()->isMiniGame()) {
        return sMaxColWallNumBig;
    }
    return sMiniGameMaxColWallNumBig;
}

void ItemGTurtleBig::calcBalanceFlying() {
    ItemGTurtle::calcBalanceFlying();
}

const bool ItemGTurtleBig::checkKartColDisappear(int idx) {
    bool ret = false;
    if ((GetKartCtrl()->GetKartStatus(idx) & 0x40000 || mState == 4)) {
        ret = true;
    }
    return ret;
}

f32 ItemGTurtleBig::getForceEfctOffsetScale() const {
    return sEfctScaleOffsetBig;
}

void ItemGTurtleBig::executeGeoObjHitCallBack(GeographyObj *obj) {
    if (countHitGeoReflectNum(obj)) {
        if (chkColWallDisappear()) {
            return;
        }
    }
    
    if (mState == 4) {
        setStateDisappear(true);
        return;
    }
    setGeoHitSe(obj);
    
    if (_2ec != 0) {
        return;
    }

    if (obj->isBigItemKind()) {
        mSoundMgr->setSe(0x4008e);
        _2ec = sBigMaxHitSoundWaitFrame;
    }
}

ItemGTurtleSuc::ItemGTurtleSuc(u32 kind) : ItemObjSuc(kind) {}

void ItemGTurtleSuc::reset() {
    ItemObjSuc::reset();
    mKartReaction = 4;
    _1fc = 3;
    mSucKind = 0;
    if (GetGeoObjMgr()->isBombBattle()) {
        mSucKind = 8;
    }
}

void ItemGTurtleSuc::doSucReleaseFunc(ItemObj *obj) {
    ItemGTurtle *itemTurle = (ItemGTurtle *)obj;
    int sucNum = (getSuccessionItemNum() - 1);
    itemTurle->_2c4 = sucNum * sSucReleaseVelMag;
}

void ItemGTurtleSuc::doSucSpReleaseFunc(ItemObj *obj) {
    doSucReleaseFunc(obj);
}

namespace {
f32 sRedVelMag = 100.0f;
f32 sGndAddVel = 2.0f;
f32 sFlyAddVel = 2.0f;
f32 sTargetR = 700.0f;
f32 sAngleAddRatio = 4.0f;
f32 sSearchAngle = 0.5f;
f32 sSearchMaxLengthXZ = 3000.0f;
f32 sSearchMaxLengthY = 900.0;
f32 sMiniGameSearchMaxLengthXZ = 4000.0f;
f32 sAttackAddVel = 25.0f;
u8 sNoAttackMineCnt = 30;
f32 sNoChaseLength = 250.0f;
int sMaxFirstCLCnt = 3;
f32 sFirstCLAngle = 0.7f;
f32 sOver1stCLAngle1 = 0.5f;
f32 sOver1stCLAngle2 = -0.6f;
f32 sEquationLength = 500.0f;
s16 sEquationFrame = 70;
f32 sEquationOffsetVel = 5.0f;
f32 sEquationVelDownrRatio = 0.85f;
f32 sEquationMinKartVel = 40.0f;
f32 sSamePointRadiusMag = 3.0f;
f32 sNoChaseVelRatio = 1.15f;
s16 sMaxRAttackCnt = 180;
}

ItemRTurtle::ItemRTurtle(u32 kind) : ItemGTurtle(kind)  {
    mPointController = new CLPointControllerRTurtle(this);
}

void ItemRTurtle::reset() {
    ItemGTurtle::reset();
    if (mItemKind == 2) {
        if (GetGeoObjMgr()->isMiniGame()) {
            mGameDifParam = &sGameDifParamMiniRed;
        }
    }

    if (mBounds) {
        mBounds->setRadius(mGameDifParam->mRadius);
    }
    _28 = sBoundRatio;
    mMoveType = 0;
    mTargetNum = -1;
    _2f4 = 0;
    _2f8 = 0.0f;
    _2fc = 0;
    _2fe = 0;
    mPointController->reset();
}

const char *ItemRTurtle::getBmdFileName() {
    static const char *cRTurtleBmdName ="/Item/item_koura_red.bmd";
    return cRTurtleBmdName; 
}

void ItemRTurtle::doUniv() {
    if (!tstAnyObjectHitting()) {
        if (mState != 8) {
            switch (mState)
            {
            case 1:
            case 5:
                doEquipScaleUp(sScaleEquip);
                break;
            case 4:
            case 6:
                doFallScaling();
                setScaleCol(mEquipScale);
                break;
            case 7:
                doForceDisappearScaleDown();
                break;

            default:
                f32 scaleUpRatio = sScaleUpRatio;
                if (mState == 4) {
                    scaleUpRatio = sScaleUpRatioFall;
                }
                if (mEquipScale < mGameDifParam->_00) {
                    mEquipScale *= scaleUpRatio;
                }
                setScaleCol(mEquipScale);
                break;
            }
        }
    }

    (this->*mStateFunc)();
    doSound(0x40002);
}

ItemObj::ItemColFlag ItemRTurtle::calcColReaction(ItemObj *) {
    return cColFlag_4;
}

void ItemRTurtle::initRelease() {
    mMaxVel = sRedVelMag;
    ItemGTurtle::setInitReleaseParam();
    MtxPtr bodyMtx = ObjUtility::getKartBodyMtx(mOwnerNum);
    stMtx2Qt(&_44, bodyMtx);
    f32 throwAng = mGameDifParam->_1c;
    if (mSuccessionItemLink.getList()) {
        throwAng *= 0.5f;
    }
    setInitThrowDir(&mVel, throwAng, true);
    mVel.scale(mMaxVel);
    
    JGeometry::TVec3f kartVel;
    ObjUtility::getKartVel(mOwnerNum, &kartVel);
    mVel.add(kartVel);
    mMoveType = 0;
    initCL();
}

const bool ItemRTurtle::searchTarget(f32 searchMaxLen, bool p2) {
    bool ret = false;
    JGeometry::TVec3f vel(mVel);
    vel.y = 0.0f;

    stVecNormalize(vel);

    mTargetNum = -1;

    f32 r = searchMaxLen * sSamePointRadiusMag;
    for (int i = 0; i < RCMGetManager()->getKartNumber(); i++) {
        JGeometry::TVec3f kartPos;
        ObjUtility::getKartPos(i, &kartPos);

        JGeometry::TVec3f diff;
        diff.sub(kartPos, mPos);
        f32 absY = JMAAbs(diff.y);
        diff.y = 0.0f;

        f32 diffLen = diff.length();
        stVecNormalize(diff);
        
        f32 searchAng = sSearchAngle;
        f32 len = searchMaxLen;
        f32 dp = diff.dot(vel);

        if (!p2) {
            CLPoint *center = GetKartCtrl()->GetKartCenterPtr(i);
            if (center == mPointController->getNext()) {
                searchAng = 0.0f;
                len = searchMaxLen * sSamePointRadiusMag;
                _24c = 1;
            }
            else {
                _24c = 0;
            }
        }

        if (dp > searchAng && len > diffLen && sSearchMaxLengthY > absY) {
            if (mOwnerNum != i || mWaitFrame >= sNoAttackMineCnt) {
                if (GetKartCtrl()->GetKartStatus(i) & (0xb00000 | 0x80000)) {
                    continue;
                }
                if (p2) {
                    switch (RCMGetKartChecker(i)->getBalloonNumber()) {
                    case 0:
                        continue;
                    }
                }
                else if (RCMGetKartChecker(i)->getRank() > RCMGetKartChecker(mOwnerNum)->getRank()) {
                    continue;
                }
                if (r > diffLen) {
                    r = diffLen;
                    mTargetNum = i;
                    
                }
            }            
        }

    }

    if (mTargetNum >= 0) {
        mMoveType = 2;
        _2f4 = 0;
        _2f8 = mVel.length();
        ret = true;
    }
    return ret;
}

const bool ItemRTurtle::checkOver1stKart() {
    bool ret = false;

    JGeometry::TVec3f kartPos;
    ObjUtility::getKartPos(0, &kartPos);

    JGeometry::TVec3f diff;
    diff.sub(kartPos, mPos);
    f32 diffLen = diff.length();

    int kartIndex = 0;
    for (int i = 1; i < RCMGetManager()->getKartNumber(); i++) {
        ObjUtility::getKartPos(i, &kartPos);
        diff.sub(kartPos, mPos);
        f32 len = diff.length();
        if (diffLen > len) {
            kartIndex = i;
            diffLen = len;
        }
    }

    KartChecker *kartChecker = RCMGetKartChecker(kartIndex);
    if (kartChecker->getRank() == 1) {
        JGeometry::TVec3f clDir;
        getKartCLDir(kartIndex, &clDir);

        JGeometry::TVec3f pointPos(mPointController->_c);
        if (pointPos.dot(clDir) > sOver1stCLAngle1) {
            ObjUtility::getKartPos(kartIndex, &kartPos);
            JGeometry::TVec3f firstPlaceDiff;
            firstPlaceDiff.sub(kartPos, mPos);
            firstPlaceDiff.y = 0.0f;
            stVecNormalize(firstPlaceDiff);
            if (firstPlaceDiff.dot(pointPos) < sOver1stCLAngle2) {
                ret = true;
                mMoveType = 3;
            }
        }
    }
    return ret;
}

void ItemRTurtle::getKartCLDir(int idx, JGeometry::TVec3f *dir) {
    CLPoint *center = GetKartCtrl()->GetKartCenterPtr(idx);
    const CrsData::SCLPoint *nextPointData = center->getNextPoint(0)->getPointData();
    JGeometry::TVec3f nextPos;
    if (nextPointData) {
        nextPos.set(nextPointData->pos);
    }
    else {
        nextPos.zero();
    }

    JGeometry::TVec3f curPos;
    const CrsData::SCLPoint *curPointData = center->getPointData();
    if (curPointData) {
        curPos.set(curPointData->pos);
    }
    else {
        curPos.zero();
    }

    dir->sub(nextPos, curPos);
    dir->y = 0.0f;
    stVecNormalize(*dir);
}

void ItemRTurtle::moveAttack() {
    JGeometry::TVec3f kartPos;
    ObjUtility::getKartPos(mTargetNum, &kartPos);
    JGeometry::TVec3f diff;
    diff.sub(kartPos, mPos);
    f32 len = diff.length();

    if (mTransferFlags & 2) {
        diff.y = 0.0f;
    }
    f32 prevMaxVel = getMaxVel();
    stVecNormalize(diff);

    if (len < sNoChaseLength || _2f4 >= sEquationFrame) {
        mMoveType = 3;
        return;
    }

    if (GetGeoObjMgr()->isMiniGame()) {
        if (_2fe > sMaxRAttackCnt) {
            mMoveType = 3;
            return;
        }
        _2fe++;
    }

    JGeometry::TVec3f kartVel;
    ObjUtility::getKartVel(mTargetNum, &kartVel);
    f32 kartLen = kartVel.length();
    f32 yVel = mVel.y;
    f32 velLen = mVel.length();
    if (_2fc == 0 && kartLen >= sEquationMinKartVel && len < sEquationLength) {
        _2fc = 1;
    }

    if (_2fc) {
        f32 maxVel = kartLen + sEquationOffsetVel;
        if (_2f8 > maxVel) {
            f32 tmp = (_2f8 * sEquationVelDownrRatio);
            maxVel = (tmp < maxVel ? maxVel : (tmp));
        }
        mVel = diff;
        _2f4++;
        _2f8 = maxVel;
        mMaxVel = maxVel;
    }
    else {
        // Issue is here
        JGeometry::TVec3f attackAddVel(diff);
        
        attackAddVel.scale(sAttackAddVel);
        mVel.y = 0.0f;
        mVel.add(attackAddVel);
        stVecNormalize(mVel);
    }

    f32 xzLen = (velLen > prevMaxVel) ? velLen : getMaxVel();

    if (!(mTransferFlags & 2)) {
        f32 len2 = (xzLen*xzLen);
        f32 len2Y = (yVel * yVel);
        f32 xzVel = JMAFastSqrt(len2 - len2Y);
        if (xzVel > 0.0f) {
            mVel.scale(xzVel);
        }
        mVel.y = yVel;
    }
    else {
        mVel.scale(xzLen);
    }
}

void ItemRTurtle::moveNoChase() {
    if (mTargetNum < 0)
        return;

#line 2026
    JUT_MINMAX_ASSERT(0, mTargetNum, RCMGetManager()->getKartNumber());
    mVel.scale(sNoChaseVelRatio);
    stClampVecMax(mVel, sRedVelMag);
}

void ItemRTurtle::moveCenterLine() {
    JGeometry::TVec3f dir;
    JGeometry::TVec3f nextCLPos;
    if (mPointController->getNext()->getPointData()) {
        nextCLPos.set(mPointController->getNext()->getPointData()->pos);
    }
    else {
        nextCLPos.zero();
    }

    dir.sub(mPos, nextCLPos);
    JGeometry::TVec3f dirXZ(dir);
    dirXZ.y = 0.0f;
    dirXZ.length();
    stVecNormalize(dir);

    f32 dp = dir.dot(mPointController->_c);
    f32 addVel = sFlyAddVel;
    if (_2b4) {
        addVel = sGndAddVel;
    }
    addVel *= sAngleAddRatio * (1.0f - dp);
    
    if (mPointController->getNext()->getPointData()) {
        nextCLPos.set(mPointController->getNext()->getPointData()->pos);
    }
    else {
        nextCLPos.zero();
    }
    JGeometry::TVec3f dirXZ2;
    dirXZ2.sub(nextCLPos, mPos);
    dirXZ2.y = 0.0f;
    stVecNormalize(dirXZ2);
    dirXZ2.scale(addVel);
    f32 yVel = mVel.y;
    f32 len = mVel.length();
    mVel.y = 0.0f;
    mVel.add(dirXZ2);
    stVecNormalize(mVel);

    f32 len2 = (len*len);
    f32 len2Y = (yVel * yVel);
    f32 xzVel = JMAFastSqrt(len2 - len2Y);

    mVel.scale(xzVel);
    mVel.y = yVel;
    if (mPointController->chkArriveAtFwPoint(mPos, sTargetR) || mPointController->chkIsOverFwPoint(mPos)) {
        mPointController->changeNextPoint();
    }
}

void ItemRTurtle::initCL() {
    mMoveType = 1;
    if (!GetGeoObjMgr()->isMiniGame()) {
        mPointController->init_ByKartIndex(mOwnerNum);
        if (mPointController->mPrev) {
            mPointController->calcFirstCLbyAngle(mPos, sMaxFirstCLCnt, sFirstCLAngle);
            mMoveType = 1;
        }
    }
}

void ItemRTurtle::calcVel() {
    if (mState == 3) {
        return;
    }

    switch (mMoveType) {
    case 1: {
        if (GetGeoObjMgr()->isMiniGame()) {
            searchTarget(getMiniGameSearchMaxLengthXZ(), true);
        }
        else {
            if (!checkOver1stKart() && !searchTarget(sSearchMaxLengthXZ, false)) {
                moveCenterLine();
            }
        }
        break;
    }
    case 2: {
        moveAttack();
        break;
    }
    case 3: {
        moveNoChase();
        break;
    }
    }
}

f32 ItemRTurtle::getMiniGameSearchMaxLengthXZ() {
    return sMiniGameSearchMaxLengthXZ;
}

void ItemRTurtle::moveColWall(CrsGround &ground) {
    mTransferFlags |= 4;
    mVel.zero();
    setStateDisappear(true);
}

void ItemRTurtle::calcMoveGndNormalVec(JGeometry::TVec3f *out, CrsGround &) {
    out->set(0.0f, 1.0f, 0.0f);
}

CLPointControllerRTurtle::CLPointControllerRTurtle(ItemObj *obj) {
    mItemObj = obj;
}

int CLPointControllerRTurtle::findNextPointNo(CLPoint *point) {
    int ret = 0;
    int pointCnt = (mIsReverse) ? point->getPrevPointNumber() : point->getNextPointNumber();

    if (pointCnt < 2) {
        return 0;
    }
    
    int rank = RCMGetKartChecker(mItemObj->getOwnerNum())->getRank();
    for (u8 i = 0; i < RCMGetManager()->getKartNumber(); i++) {
        if (i == mItemObj->getOwnerNum()) {
            continue;
        }

        if (rank - 1 == RCMGetKartChecker(i)->getRank()) {
            u8 groupID = GetKartCtrl()->GetKartCenterPtr(i)->getGroupID();
            int pointNo = -1;
            u8 count = 0;
            bool foundTarget = false;
            recursiveTargetCalc(point, groupID, &pointNo, &count, &foundTarget);
            if (foundTarget && pointNo != -1) {
                ret = pointNo;
            }

        }
    }
    return ret;
}

ItemRTurtleSuc::ItemRTurtleSuc(u32 kind) : ItemGTurtleSuc(kind)  {}

void ItemRTurtleSuc::reset() {
    ItemObjSuc::reset();
    mKartReaction = 0xf;
    _1fc = 3;
    mSucKind = 2;
}

namespace  {
bool sTstStopFireBall;

f32 sFScaleEquip = 1.0f;
f32 sFScale = 2.0f;
f32 sFVelMag = 110.0f;
f32 sFSpVelMag = 50.0;
f32 sFHeightOffset = 20.0f;
f32 sFQtLerpRatio = 0.5f;
f32 sFGravity = -3.1f;
f32 sFBoundVel = 40.0f;
f32 sFBoundsRadius = 80.0f;
f32 sFScaleUpRatio = 1.05f;
s16 sFMaxMoveFrame = 240;
u8 sMaxFireBallColWallNum = 2;
f32 sFEfctAttachRadius = 70.0f;
f32 sFEfctRotSpd = 0.35f;
f32 sFEfctScale = 2.5f;
u8 sFireBallEfctNum = 2;
u8 sFireBallBaseEfctNum = 3; 
f32 sFSucHeightOffset = 15.0f;
f32 sFMaxVel = 200.0f;

char *sJPARedFireBallBaseName[3] = {
    "mk_fireball_a",
    "mk_fireball_b",
    "mk_fireball_c"
};

char *sJPAGreenFireBallBaseName[3] = {
    "mk_fireballG_a",
    "mk_fireballG_b",
    "mk_fireballG_c"
};

const char *sJPARedFireBallName[2] = {
    "mk_fireball_nage_b",
    "mk_fireball_nage_c"
};

const char *sJPAGreenFireBallName[2] = {
    "mk_fireballG_nage_b",
    "mk_fireballG_nage_c"
};

}

ItemFireBall::ItemFireBall(u32 kind) : ItemGTurtle(kind) {
    ItemGTurtle::reset();
}

void ItemFireBall::reset() {
    ItemGTurtle::reset();
    _12c &= ~0x8000;
    mKartReaction = 10;
    _2c = 1.0f;
    mHeightOffset = sFHeightOffset;
    mLerpRatio = sFQtLerpRatio;
    _28 = 1.0f;
    if (mBounds) {
        mBounds->setRadius(sFBoundsRadius);
    }

    for (u8 i = 0 ; i < sFireBallEfctNum; i++) {
        _2f8[i] = nullptr;
    }

    for (u8 i = 0 ; i < sFireBallBaseEfctNum; i++) {
        _2ec[i] = nullptr;
    }

    _300 = sJPARedFireBallName;
    _12c |= 0x100;
}

const char *ItemFireBall::getBmdFileName() {
    static const char *cFireBallBmdName = "/Item/item_koura_green.bmd";
    return cFireBallBmdName;
}

void ItemFireBall::doUniv() {
    if (mState == 8) {
        mEquipScale = 1.0f;
        mBounds->setScale(mEquipScale);
    }
    else if (!tstAnyObjectHitting()) {
        switch (mState)
        {
        case 1:
        case 5:
            doEquipScaleUp(sFScaleEquip);
            break;
        case 7:
            doForceDisappearScaleDown();
            break;
        default:
            f32 scaleUpRatio = sFScaleUpRatio;
            if (mState == 4) {
                scaleUpRatio = sScaleUpRatioFall;
            }
            if (mEquipScale < sFScale) {
                mEquipScale *= scaleUpRatio;
                if (mEquipScale >= sFScale) {
                    mEquipScale = sFScale;
                }
            }
            mBounds->setScale(mEquipScale);
            setScaleCol(mEquipScale);
            break;
        }
    }
    (this->*mStateFunc)();

    JGeometry::TVec3f tr(0.0f, sFEfctAttachRadius, 0.0f);
    PSMTXMultVec(mModel.getBaseTRMtx(), &tr, &tr);
    f32 scale = sFEfctScale;
    for (u8 i = 0; i < sFireBallEfctNum; i++) {
        if (_2f8[i]) {
            JPASetRMtxfromMtx(mModel.getBaseTRMtx(), _2f8[i]->mGlobalRot);
            _2f8[i]->setGlobalTranslation(tr);
            _2f8[i]->setGlobalScale(scale);
        }
    }

    _12c |= 0x80;
}

void ItemFireBall::initEffect() {
    deleteHaveEffect();
    if (*_2f8 == nullptr) {
        for (u8 i = 0; i < sFireBallEfctNum; i++) {
            _2f8[i] = GetJPAMgr()->createEmt(_300[i], mPos);
        }
    }
}

void ItemFireBall::initHaveEffect() {
    char **baseName = nullptr;
    switch (getItemColorID()) {
    case 1:
        baseName = sJPARedFireBallBaseName;
        break;
    case 2:
        baseName = sJPAGreenFireBallBaseName;
        break;
    }
#line 2588
    JUT_MINMAX_ASSERT(1, getItemColorID(), 3);

    for (u8 i = 0; i < sFireBallBaseEfctNum; i++) {
        if (!_2ec[i]) {
            _2ec[i] = GetJPAMgr()->createEmt(baseName[i], mPos);
        }
    }
}

void ItemFireBall::deleteEffect() {
    for (u8 i = 0; i < sFireBallEfctNum; i++) {
        if (_2f8[i]) {
            GetJPAMgr()->deleteEmt(_2f8[i]);
            _2f8[i] = nullptr;
        }
    }
    deleteHaveEffect();
}

void ItemFireBall::deleteHaveEffect() {
    for (u8 i = 0; i < sFireBallBaseEfctNum; i++) {
        if (_2ec[i]) {
            GetJPAMgr()->deleteEmt(_2ec[i]);
            _2ec[i] = nullptr;
        }
    }
}

void ItemFireBall::initEquip() {
    ItemGTurtle::initEquip();
    mBuild->_35 = 1;
    mMulti = mBuild;

    if (mColorID == 0) {
        ECharID driverID = RaceMgr::getManager()->getKartInfo(getOwnerNum())->getDriverCharID(getDriverNum());
        if (driverID == LUIGI) {
            mColorID = 2; // Green
        }
        else {
            mColorID = 1; // Red
        }
    }

    if (!mSuccessionItemLink.getList()) {
        initHaveEffect();
    }
    if (mColorID == 2) {
        _300 = sJPAGreenFireBallName;
    }
}

void ItemFireBall::doEquip() {
    if (!mSuccessionItemLink.getList()) { 
        ItemObj::doEquipScaleUp(1.0f);
        mDisScaleAnm.scale(getThunderScale());
        JGeometry::TVec3f handOffsetPos(getHandOffsetPos());
        PSMTXMultVec(mpHandAnmMtx, &handOffsetPos, &mPos);
        moveBaseEfct();
    }
    executeEquipHandFunc();
}

void ItemFireBall::moveBaseEfct() {
    for (u8 i = 0; i < sFireBallBaseEfctNum; i++) {
        if (_2ec[i]) {
            _2ec[i]->setGlobalTranslation(mPos);
            _2ec[i]->setGlobalScale(mDisScaleAnm);
        }
    }
}

void ItemFireBall::initRelease() {
    ItemGTurtle::doUnivInitRelease(sFVelMag, false);
    initEffect();
}

void ItemFireBall::initSpRelease() {
    ItemGTurtle::doUnivInitRelease(sFSpVelMag, true);
    initEffect();
}

void ItemFireBall::initDisappear() {
    mEquipFlags = 0;
    _12c &= ~1;
    if (_2f8[0]) {
        mSoundMgr->setSe(0x4001f);
        GetJPAMgr()->deleteEmt(_2f8[0]);
    }

    if (_2f8[1]) {
       _2f8[1]->becomeInvalidEmitter();
    }

    for (u8 i = 0; i < sFireBallEfctNum; i++) {
        _2f8[i] = nullptr;
    }

    mVel.zero();
}

void ItemFireBall::doDisappear() {
    if (mWaitFrame >= 30) {
        mState = 10;
        initEnd();
    }
}

void ItemFireBall::initFall() {
    ItemObj::initFall();
    initEffect();
}

void ItemFireBall::doFall() {
    ItemObj::doFall();
    if (mTransferFlags & 2) {
        setStateDisappear(true);
    }
}

void ItemFireBall::initForceDisappear() {
    deleteEffect();
    ItemObj::initForceDisappear();
}

void ItemFireBall::doDivested() {
    ItemObj::moveDivested();
    moveBaseEfct();
}

void ItemFireBall::doUnivRelease() {
    if (mWaitFrame < sReleaseOffsetCnt) {
        mPos.add(mCompVec);
    }

    if (_2de) {
        checkStartColItemFrame(0xf);
    }

    setVelUpDown(sVelUpRatio, sVelDownRatio);
    mVel.y += sFGravity;
    calcVel();

    if (!sTstStopFireBall) {
        doMovePos();
    }
    rotationRad(_2b0, sFEfctRotSpd);
    JGeometry::TVec3f velXZ(mVel);
    velXZ.y = 0.0f;
    stVecNormalize(velXZ);

    JGeometry::TVec3f cp;
    JGeometry::TVec3f up(0.0f, 1.0f, 0.0f);
    cp.cross(up, velXZ);
    JGeometry::TPos3f m;
    m.setRotate(cp, _2b0);
    stMtx2Qt(&_44, m);
    doMoveColGround(nullptr, false);

    if (mVel.length() > sFMaxVel) {
        stVecNormalize(mVel);
        mVel.scale(sFMaxVel);
    }

    if (GetGeoObjMgr()->isMiniGame() && mTransferFlags & 4) {
        setStateDisappear(true);
    }

    if (mWaitFrame > sFMaxMoveFrame && !sTstStopFireBall) {
        setStateDisappear(true);
    }

    if (mTransferFlags & 4) {
        chkColWallDisappear();
    }
}

void ItemFireBall::moveNormal(CrsGround &ground) {
    if (sTstStopFireBall) {
        return;
    }
    f32 moveY = mDisScaleAnm.y * mHeightOffset + ground.getHeight();

    if (mPos.y < moveY) {
        mTransferFlags |= 2;
        mTransferFlags |= 0x100;

        mPos.y = moveY;
        JGeometry::TVec3f vel(mVel);
        vel.y = 0.0f;
        stVecNormalize(vel);

#line 2937
        JUT_ASSERT(getMaxVel() > sFBoundVel);
        doMoveGndParallel(vel, sFBoundVel);
    }
}

ItemObj::ItemColFlag ItemFireBall::calcColReaction(ItemObj *obj) {
    ItemColFlag ret = cColFlag_5;
    
    switch (obj->mItemKind) {
    case 1:
    case 4:
    case 7:
    case 8:
    case 11:
        ret = cColFlag_4;
        break;
    }
    return ret; 
}

bool ItemFireBall::calcRotQuat(Quaternion *q) {
    rotationRad(_2b0, sFEfctRotSpd);
    QuatRotX(_2b0, q);
    return true;
}

u8 ItemFireBall::getMaxColWallNum() {
    return sMaxFireBallColWallNum;
}

void ItemFireBall::executeGeoObjHitCallBack(GeographyObj *obj) {
    if (countHitGeoReflectNum(obj)) {
        if (mState == 8) {
            return;
        }

        u8 colWallNum = getMaxColWallNum();
        if (mItemKind != 9 && mState == 3) {
            colWallNum *= 0.5f; 
        }

        if (_2df < colWallNum) {
            _2df++;
        }

        if (_2df >= colWallNum) {
            mPos = _10;
            setStateDisappear(true);
            return;
        }
        else if (mItemKind != 9) {
            mSoundMgr->setSe(0x4005f);
        }
    }

}

ItemFireBallSuc::ItemFireBallSuc(u32 kind) : ItemObjSuc(kind) {
    for (u8 i = 0; i < sFireBallBaseEfctNum; i++) {
        mBaseFireEmt[i] = nullptr;
    }
}

void ItemFireBallSuc::reset() {
    ItemObjSuc::reset();
    mKartReaction = 0xf;
    _1fc = 5;
    if (GetGeoObjMgr()->isMiniGame()) {
        _1fc = 3;
    }
    mSucKind = 9;
    _12c |= 0x40;

    for (u8 i = 0; i < sFireBallBaseEfctNum; i++) {
        mBaseFireEmt[i] = nullptr;
    }
    mDisappearAll = false;
    mHeightOffset = sFSucHeightOffset;
}

void ItemFireBallSuc::doSucReleaseFunc(ItemObj *obj) {
    ItemFireBall *fireBall = (ItemFireBall *)obj;
    int sucNum = (getSuccessionItemNum());
    sucNum -= 1; // inline?
    fireBall->_2c4 = 0.0f;
    fireBall->_2de = 1;
    fireBall->_2dd = sucNum;
}

void ItemFireBallSuc::doSucSpReleaseFunc(ItemObj *obj) {
    doSucReleaseFunc(obj);
}

void ItemFireBallSuc::initEquip() {
    ItemObjSuc::initEquip();
    mpHandAnmMtx = getHandAnmMtx();

#line 3128
    JUT_ASSERT(mBaseFireEmt[0] == 0);

    char **baseName = RaceMgr::getManager()->getKartInfo(getOwnerNum())->getDriverCharID(getDriverNum()) == LUIGI ? 
            sJPAGreenFireBallBaseName : sJPARedFireBallBaseName;

    for (u8 i = 0; i < sFireBallBaseEfctNum; i++) {
        mBaseFireEmt[i] = GetJPAMgr()->createEmt(baseName[i], mPos);
    }
}

void ItemFireBallSuc::doEquip() {
    ItemObjSuc::doEquip();
    ItemObj::doEquipScaleUp(1.0f);
    mDisScaleAnm.scale(getThunderScale());
    JGeometry::TVec3f handOffsetPos(getHandOffsetPos());
    PSMTXMultVec(mpHandAnmMtx, &handOffsetPos, &mPos);

    if (_1a4.getNumLinks() != 0) {
        for (u8 i = 0; i < sFireBallBaseEfctNum; i++) {
            mBaseFireEmt[i]->setGlobalTranslation(mPos);
            mBaseFireEmt[i]->setGlobalScale(mDisScaleAnm);
        }
    }
}

void ItemFireBallSuc::doDivested() {
    ItemObjSuc::doDivested();
    doHandOneEquip(true, (f32)mWaitFrame / (f32)sDivestedFrame);
    
    if (_1a4.getNumLinks() != 0) {
        moveEffect(true);
    }
}

void ItemFireBallSuc::initDisappear() {
    mDisappearAll = true;
    deleteEffect();
    mState = 10;
    initEnd();
}

void ItemFireBallSuc::initFall() {
    mDisappearAll = true;
    ItemObj::initFall();
}

void ItemFireBallSuc::doFall() {
    ItemObj::doFall();
    moveEffect(true);
    _12c &= ~1;
    mEquipFlags = 0;
    if (mTransferFlags & 2) {
        setStateForceDisappear();
    }
}

void ItemFireBallSuc::initForceDisappear() {
    ItemObj::initForceDisappear();
    mDisappearAll = true;
}

void ItemFireBallSuc::doForceDisappear() {
    ItemObj::doForceDisappear();
    ItemObj::doForceDisappearScaleDown();
    moveEffect(true);
}

void ItemFireBallSuc::initEnd() {
    deleteEffect();
    if (mDisappearAll) {
        disappearAllChild(true);
    }
}

void ItemFireBallSuc::deleteEffect() {
    for (u8 i = 0; i < sFireBallBaseEfctNum; i++) {
        if (mBaseFireEmt[i]) {
            GetJPAMgr()->deleteEmt(mBaseFireEmt[i]);
            mBaseFireEmt[i] = nullptr;
        }
    }
}

void ItemFireBallSuc::moveEffect(bool applyScale) {
    for (u8 i = 0; i < sFireBallBaseEfctNum; i++) {
        if (mBaseFireEmt[i]) {
            mBaseFireEmt[i]->setGlobalTranslation(mPos);
            if (applyScale) {
                mBaseFireEmt[i]->setGlobalScale(mDisScaleAnm);
            }
        }
    }
}

int ItemFireBallSuc::IsEfctTypeRed() const {
    bool ret = 0;
    if ((_1a4.getNumLinks() != 0)) {
        ret = _1a4.getFirst()->getObject()->getColorID() == 1;
    }
    return ret;
}

#include "JSystem/JAudio/JASFakeMatch2.h"
