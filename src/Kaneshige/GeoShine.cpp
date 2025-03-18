#include "Kaneshige/Objects/GeoShine.h"
#include "JSystem/JGeometry/Matrix.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/Objects/GeoRabbitMark.h"
#include "Kaneshige/RaceMgr.h"
#include "Sato/GeographyObj.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/Objects/GeoItemBox.h"
#include "dolphin/mtx.h"
#include "types.h"

const char *GeoShine::sNormalEmitterName[3] = {
    "mk_shine_a",
    "mk_shine_b",
    "mk_shine_c"
};

const char *GeoShine::sWinnerEmitterName[2] = {
    "mk_shine_d",
    "mk_shine_e"
};

JGeometry::TVec3f GeoShine::sShadowScale(1.5f, 1.0f, 0.5f);
f32 GeoShine::sScalingNearZ = 1000.0f;
f32 GeoShine::sScalingFarZ = 9000.0f;
f32 GeoShine::sMaxScale = 4.0f;
f32 GeoShine::sRotAnmMaxVel = 3.0f;
f32 GeoShine::sRotAnmVelNormal = 0.03f;
f32 GeoShine::sRotAnmFastV0 = 2.0f;
f32 GeoShine::sRotAnmResist = 0.98f;

GeoShine::GeoShine() : GeographyObj(0x26b4) {
    mMark = nullptr;
    mConsoleNo = 0xffffffff;
    reset();
}

void GeoShine::reset() {
    resetObject();
    _58 = 0;
    clrAllCheckKartHitFlag();
    clrObjFlagCheckItemHitting();
    setObjFlagSimpleDraw();
    clrObjFlagHidding();

    for (int i = 0; i < 3; i++) {
        mNormalEmitter[i] = nullptr;
    }

    for (int i = 0; i < 2; i++) {
        mWinnerEmitter[i] = nullptr;
    }

    if (mMark) {
        mMark->getPosition(&mPos);
    }

    _154.set(0.0f, 0.0f, 1.0f);
    _160.set(0.0f, 1.0f, 0.0f);
    _16c.set(1.0f, 0.0f, 0.0f);

    _178.set(_154);
    _184.set(_160);
    _190.cross(_184, _178);
    _190.normalize();
    mRotAnmVel = sRotAnmVelNormal;
    _1a0 = 0;
    mWinEffectOn = false;
    if (getShadowModel()) {
        showShadow();
        calcShadowModel();
#line 177
        JUT_ASSERT(mConsoleNo != 0xffffffff);
        getShadowModel()->setDetailFlg();
        getShadowModel()->clrVisibleAll();
        getShadowModel()->setVisible(mConsoleNo);
    }
}

void GeoShine::calcShadowModel() {
    if (getShadowModel()->isInvisibleAll())
        return;

    CrsGround ground(RCMGetCourse());
    ground.search(this, mPos, mPos);

    JGeometry::TVec3f zDir(_178);
    JGeometry::TVec3f yDir(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f xDir;
    xDir.cross(yDir, zDir);
    xDir.normalize();
    zDir.cross(xDir, yDir);
    //zDir.normalize();

    JGeometry::TPos3f tr;
    tr.setXYZDir(xDir, yDir, zDir);
    tr[0][3] = mPos.x;
    tr[1][3] = ground.getHeight();
    tr[2][3] = mPos.z;
    getShadowModel()->setBaseTRMtx(tr);
    JGeometry::TVec3f baseScale(mScale);
    baseScale.x *= sShadowScale.x;
    baseScale.y *= sShadowScale.y;
    baseScale.z *= sShadowScale.z;

    getShadowModel()->getModel()->setBaseScale(baseScale);
}

const char *GeoShine::getShadowBmdFileName() {
    return GeoItemBox::getShadowBmdName();
}

void GeoShine::loadAnimation() {}

void GeoShine::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsSphere(225.0f, 1.0f);
}

void GeoShine::createModel(JKRSolidHeap *, u32) {}

void GeoShine::simpleDraw(u32 viewNo) {
    if (viewNo == mConsoleNo)
        GeographyObj::simpleDraw(viewNo);
}

void GeoShine::calc() {}

void GeoShine::move() {
#line 305
    JUT_ASSERT(mConsoleNo != 0xffffffff);
    KartCam *cam = RCMGetCamera(mConsoleNo);
    
    JGeometry::TVec3f footPos;
    mMark->getFootPosition(&footPos);

    JGeometry::TVec3f offset;
    offset.sub(*cam->GetCameraPos(),footPos);

    f32 len = offset.length();
    f32 scale = (len - sScalingNearZ) / (sScalingFarZ - sScalingNearZ);
    if ((scale < 0.0f)) {
        scale = 0.0f;
    } else if (scale > 1.0f) {
        scale = 1.0f;
    }

    scale = (((sMaxScale - 1.0f) * scale + 1.0f) * mMark->getMarkScale());
    mScale.setAll(scale);
    mPos.scaleAdd(scale * mMark->getFootLength(), _184, footPos);
    f32 dVar9 = _16c.dot(_178);
    f32 dVar10 = _160.dot(_178);
    f32 dVar11 = _154.dot(_178);

    JGeometry::TVec3f TStack_78;
    TStack_78.sub(*cam->GetCameraLookPos(), *cam->KartCam::GetCameraPos());
    TStack_78.normalize();
    _154.set(TStack_78);
    _16c.cross(_160, _154);
    _16c.normalize();
    _154.cross(_16c, _160);
    _178.zero();
    _178.scaleAdd(dVar9, _16c, _178);
    _178.scaleAdd(dVar10, _160, _178);
    _178.scaleAdd(dVar11, _154, _178);

    mRotAnmVel *= sRotAnmResist;
    if (mRotAnmVel < sRotAnmVelNormal) {
        mRotAnmVel = sRotAnmVelNormal;
    } 

    if (mRotAnmVel <= sRotAnmVelNormal) {
        _1a0++;
    }
    else {
        _1a0 = 0;
    }

    _178.scaleAdd(mRotAnmVel, _190, _178);
    _178.normalize();
    _190.cross(_184, _178);
    _190.normalize();
    _184.cross(_178, _190);

    mRotMtx.setXYZDir(_190, _184, _178);
    calcShadowModel();

    for (int i = 0; i < 3; i++) {
        if (mNormalEmitter[i]) {
            mNormalEmitter[i]->setGlobalTranslation(mPos);
            mNormalEmitter[i]->setGlobalScale(mScale);
        }
        else {
            mNormalEmitter[i] = GetJPAMgr()->createEmtCam(sNormalEmitterName[i], mPos, mConsoleNo);
        }
    }

    for (int i = 0; i < 2; i++) {
        if (mWinnerEmitter[i]) {
            mWinnerEmitter[i]->setGlobalTranslation(mPos);
            mWinnerEmitter[i]->setGlobalScale(mScale);
        }
        else if (mWinEffectOn) {
            mWinnerEmitter[i] = GetJPAMgr()->createEmtCam(sWinnerEmitterName[i], mPos, mConsoleNo);  
        }
    }
}

void GeoShine::startFastRot() {
    mRotAnmVel += sRotAnmFastV0;

    if (mRotAnmVel > sRotAnmMaxVel)
        mRotAnmVel = sRotAnmMaxVel;
}

#include "JSystem/JAudio/JASFakeMatch2.h"
