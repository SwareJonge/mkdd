#include "Kaneshige/Objects/GeoNormCar.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Sato/GeographyObj.h"

#include "JSystem/JAudio/JASFakeMatch2.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ObjUtility.h"
#include "mathHelper.h"


J3DAnmTexPattern *GeoNormCar::sBodyBtpAnm;
J3DAnmTextureSRTKey *GeoNormCar::sTireBtkAnm;

const Vec GeoNormCar::sTireOfsTable[4] = {
    {160.0, 0.0f, 220.0f},
    {-160.0, 0.0f, 220.0f},
    {160.0, 0.0f, -220.0f},
    {-160.0, 0.0f, -220.0f},
};

GeoNormCar::GeoNormCar(const CrsData::SObject &obj) : GeoCar(obj) {
    if (obj.mParam1 != 0) {
        _160 = obj.mParam1;
    }
    else {
        _160 = 30.0f;
    }

    mFrScale.add(getTirePosOffset(0), getTirePosOffset(1));
    mFrScale.scale(0.5f);

    mBcScale.add(getTirePosOffset(2), getTirePosOffset(3));
    mBcScale.scale(0.5f);
    mFrame = (getSupervisor()->getCarNumber() - 1) % 5;
    
    mShadow = (GeoCarShadow*)GetGeoObjMgr()->createSubObj(0x1003);
    JGeometry::TVec3f shadowScale(1.25f, 1.0f, 1.1f);
    mShadow->setBaseScale(shadowScale);
    reset();
}

void GeoNormCar::reset() {
    GeoCar::reset();
    _58 = 3;
    GeographyObj::setObjFlagSimpleDraw();
    mBodyMat.resetFrame();
    mBodyMat.getFrameCtrl()->setFrame(mFrame);
    mBodyMat.getFrameCtrl()->stop();

    mTireMat.resetFrame();
    _2a8 = 0.0f;
}

void GeoNormCar::createModel(JKRSolidHeap *slid, u32 viewNo) {
    mBodyMat.setExModel(&mModel);
    mBodyMat.attach(sBodyBtpAnm);
    if (isTireBtkEnable()) {
        mTireMat.setExModel(&mModel);
        mTireMat.attach(sTireBtkAnm); 
    }
}

void GeoNormCar::createColModel(J3DModelData *mdlData) {
    static const f32 cBoundHeight[2] = {
        300.0f, 300.0f
    };

    static const f32 cBoundZ[2] = {
        140.0f, -190.0f
    };
    GeographyObj::createMultiBoundsPtrAll(2);

    for (u8 i = 0; i < GeographyObj::getAllBoundsNum(); i++) {
        GeographyObj::createMultiBoundsCylinder(i, mdlData, 200.0f, cBoundHeight[i]);

        JGeometry::TVec3f colPos;
        colPos.set(0.0f, 0.0f, cBoundZ[i]);
        GeographyObj::setColObjPos(colPos, i);
    }
    _158 = cBoundHeight[1];
}

void GeoNormCar::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *btp = ObjUtility::getPtrCourseArc("/Objects/Car_Public1.btp");
    J3DAnmObjMaterial::setupTexPatternAnmData(&sBodyBtpAnm, mdlData, btp);
    void *btk = ObjUtility::getPtrCourseArc("/Objects/Car_Public1.btk");
    if (btk) {
        J3DAnmObjMaterial::setupTexSRTAnmData(&sTireBtkAnm, mdlData, btk);
    }
}

void GeoNormCar::simpleDraw(u32 viewNo) {
    mBodyMat.anmFrameProc();
    if (isTireBtkEnable()) {
        mTireMat.anmFrameProc();
    }
    GeoCar::setTevColor();
    GeographyObj::simpleDraw(viewNo);
}

void GeoNormCar::calc() {
    GeoCar::calc();
    if (isTireBtkEnable()) {
        mTireMat.getFrameCtrl()->setRate(getTireAnmRate());
        mTireMat.frameProc();
    }

    if (mShadow) {
        JGeometry::TVec3f shadowPos;
        shadowPos.scaleAdd(-25.0f, mFrDir, mPos);
        mShadow->setCarPose(mFrDir, shadowPos);
    } 
}

void GeoNormCar::update() {
    mBodyMat.anmFrameProc();
    if (isTireBtkEnable()) {
        mTireMat.anmFrameProc();
    }
    GeographyObj::update();
}

const Vec &GeoNormCar::getTirePosOffset(int tireNo) {
#line 479
    JUT_MINMAX_ASSERT(0, tireNo, 4);
    return sTireOfsTable[tireNo];
}
