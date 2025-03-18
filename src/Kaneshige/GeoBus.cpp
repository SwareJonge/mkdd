#include "Kaneshige/Objects/GeoBus.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ObjUtility.h"

#include "JSystem/JAudio/JASFakeMatch2.h"
#include "mathHelper.h"

J3DAnmTexPattern *GeoBus::sBodyBtpAnm;
J3DAnmTextureSRTKey *GeoBus::sTireBtkAnm;

const Vec GeoBus::sTireOfsTable[4] = {
    {180.0, 0.0f, 570.0f},
    {-180.0, 0.0f, 570.0f},
    {180.0, 0.0f, -570.0f},
    {-180.0, 0.0f, -570.0f},
};

GeoBus::GeoBus(const CrsData::SObject &obj) : GeoCar(obj) {
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
    mFrame = (getSupervisor()->getCarNumber() - 1) % 2;
    mShadow = (GeoCarShadow*)GetGeoObjMgr()->createSubObj(0x1003);
    JGeometry::TVec3f shadowScale(1.5f, 1.0f, 1.9f);
    mShadow->setBaseScale(shadowScale);
    reset();
}


void GeoBus::reset() {
    GeoCar::reset();
    _58 = 3;
    GeographyObj::setObjFlagSimpleDraw();
    mBodyMat.resetFrame();
    mBodyMat.getFrameCtrl()->setFrame(mFrame);
    mBodyMat.getFrameCtrl()->stop();

    mTireMat.resetFrame();
    _2a8 = 0.0f;
}

void GeoBus::createModel(JKRSolidHeap *slid, u32 viewNo) {
    mBodyMat.setExModel(&mModel);
    mBodyMat.attach(sBodyBtpAnm);
    
    mTireMat.setExModel(&mModel);
    mTireMat.attach(sTireBtkAnm);
}

void GeoBus::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *btp = ObjUtility::getPtrCourseArc("/Objects/Car_Bus1.btp");
    J3DAnmObjMaterial::setupTexPatternAnmData(&sBodyBtpAnm, mdlData, btp);
    
    void *btk = ObjUtility::getPtrCourseArc("/Objects/Car_Bus1.btk");
    J3DAnmObjMaterial::setupTexSRTAnmData(&sTireBtkAnm, mdlData, btk);
}

void GeoBus::createColModel(J3DModelData *mdlData) {
    GeographyObj::createMultiBoundsPtrAll(3);

    for (u8 i = 0; i < GeographyObj::getAllBoundsNum(); i++) {
        GeographyObj::createMultiBoundsCylinder(i, mdlData, 225.0f, 525.0f);

        JGeometry::TVec3f colPos;
        colPos.set(0.0f, 0.0f, 350.0f * i + -350.0f);
        GeographyObj::setColObjPos(colPos, i);
    }
    _158 = 525.0f;
}

const Vec &GeoBus::getTirePosOffset(int tireNo) {
#line 261
    JUT_MINMAX_ASSERT(0, tireNo, 4);
    return sTireOfsTable[tireNo];
}

void GeoBus::simpleDraw(u32 viewNo) {
    mBodyMat.anmFrameProc();
    mTireMat.anmFrameProc();
    
    GeoCar::setTevColor();
    GeographyObj::simpleDraw(viewNo);
}

void GeoBus::calc() {
    GeoCar::calc();
        
    mTireMat.getFrameCtrl()->setRate(getTireAnmRate());
    mTireMat.frameProc();

    GeoCar::getShadowArea();
    if (mShadow) {
        mShadow->setCarPose(mFrDir, mPos);
    }     
}
