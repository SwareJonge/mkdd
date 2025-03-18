#include "Kaneshige/Objects/GeoTruck.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Sato/GeographyObj.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ObjUtility.h"

#include "JSystem/JAudio/JASFakeMatch2.h"
#include "mathHelper.h"

J3DAnmTexPattern *GeoTruck::sBodyBtpAnm;
J3DAnmTextureSRTKey *GeoTruck::sTireBtkAnm;

const Vec GeoTruck::sTireOfsTable[4] = {
    {200.0, 0.0f, 900.0f},
    {-200.0, 0.0f, 900.0f},
    {200.0, 0.0f, -870.0f},
    {-200.0, 0.0f, -870.0f},
};

GeoTruck::GeoTruck(const CrsData::SObject &obj) : GeoCar(obj) {
    if (obj.mParam1 != 0) {
        _160 = obj.mParam1;
    }
    else {
        _160 = 20.0f;
    }
    mFrScale.add(getTirePosOffset(0), getTirePosOffset(1));
    mFrScale.scale(0.5f);

    mBcScale.add(getTirePosOffset(2), getTirePosOffset(3));
    mBcScale.scale(0.5f);
    
    mFrame = (getSupervisor()->getCarNumber() - 1) % 3;
    mShadow = (GeoCarShadow*)GetGeoObjMgr()->createSubObj(0x1003);
    JGeometry::TVec3f shadowScale(1.4f, 1.0f, 1.7f);
    mShadow->setBaseScale(shadowScale);
    reset();
}

void GeoTruck::reset() {
    GeoCar::reset();
    _58 = 3;
    GeographyObj::setObjFlagSimpleDraw();
    mBodyMat.resetFrame();
    mBodyMat.getFrameCtrl()->setFrame(mFrame);
    mBodyMat.getFrameCtrl()->stop();

    mTireMat.resetFrame();
    _2a8 = 0.0f;
}

void GeoTruck::createModel(JKRSolidHeap *slid, u32 viewNo) {
    mBodyMat.setExModel(&mModel);
    mBodyMat.attach(sBodyBtpAnm);
    
    mTireMat.setExModel(&mModel);
    mTireMat.attach(sTireBtkAnm);
}

void GeoTruck::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *btp = ObjUtility::getPtrCourseArc("/Objects/Car_Truck1.btp");
    J3DAnmObjMaterial::setupTexPatternAnmData(&sBodyBtpAnm, mdlData, btp);
    
    void *btk = ObjUtility::getPtrCourseArc("/Objects/Car_Truck1.btk");
    J3DAnmObjMaterial::setupTexSRTAnmData(&sTireBtkAnm, mdlData, btk);
}

void GeoTruck::createColModel(J3DModelData *mdlData) {
    static const f32 cBoundHeight[3] = {
        450.0f, 600.0f, 600.0f
    };

    static const f32 cBoundsZ[3] = {
        350.0f, 0.0f, -300.0f
    };
    GeographyObj::createMultiBoundsPtrAll(3);

    for (u8 i = 0; i < GeographyObj::getAllBoundsNum(); i++) {
        GeographyObj::createMultiBoundsCylinder(i, mdlData, 200.0f, cBoundHeight[i]);

        JGeometry::TVec3f colPos;
        colPos.set(0.0f, 0.0f, cBoundsZ[i]);
        GeographyObj::setColObjPos(colPos, i);
    }
    _158 = cBoundHeight[1];
}

const Vec &GeoTruck::getTirePosOffset(int tireNo) {
#line 267
    JUT_MINMAX_ASSERT(0, tireNo, 4);
    return sTireOfsTable[tireNo];
}

void GeoTruck::simpleDraw(u32 viewNo) {
    mBodyMat.anmFrameProc();
    mTireMat.anmFrameProc();
    
    GeoCar::setTevColor();
    GeographyObj::simpleDraw(viewNo);
}

void GeoTruck::calc() {
    GeoCar::calc();
        
    mTireMat.getFrameCtrl()->setRate(getTireAnmRate());
    mTireMat.frameProc();

    if (mShadow) {
        mShadow->setCarPose(mFrDir, mPos);
    }    
}
