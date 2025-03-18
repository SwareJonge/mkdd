#include "Kaneshige/Objects/GeoHanaBody.h"

#include "JSystem/J3D/J3DModel.h"
#include "JSystem/JAudio/JASFakeMatch2.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ObjUtility.h"
#include "mathHelper.h"

J3DAnmTextureSRTKey *GeoHanaBody::sTireBtkAnm;

const Vec GeoHanaBody::sTireOfsTable[4] = {
    {160.0f, 0.0f, 220.0f},
    {-160.0f, 0.0f, 220.0f},
    {160.0f, 0.0f, -220.0f},
    {-160.0f, 0.0f, -220.0f}
};

f32 GeoHanaBody::sBodyToBodyLength = 450.0f;

GeoHanaBody::GeoHanaBody() : GeoCar(0xfaa, false) {
    mShadow = (GeoHanaBodyShadow*)GetGeoObjMgr()->createSubObj(0xfff);
    mParentHana = nullptr;
    mParentCar = nullptr;
    mArea = new CrsArea();
    reset();
}

void GeoHanaBody::reset() {
    GeoCar::reset();
    setObjFlagSimpleDraw();
    initPos();
    _58 = 0xf;
    mTireMat.resetFrame();
    mArea->reset();
}

void GeoHanaBody::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *btk = ObjUtility::getPtrCourseArc("/Objects/Car_Hana1Body.btk");
    J3DAnmObjMaterial::setupTexSRTAnmData(&sTireBtkAnm, mdlData, btk);
}

void GeoHanaBody::createModel(JKRSolidHeap *slid, u32 viewNo) {
    mTireMat.setExModel(&mModel);
    mTireMat.attach(sTireBtkAnm);
}

void GeoHanaBody::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsCylinder(mdlData, 270.0f, 420.0f);
    _158 = 420.0f;
}

void GeoHanaBody::simpleDraw(u32 viewNo) {
    mTireMat.anmFrameProc();
    GeoCar::setTevColor();
    GeographyObj::simpleDraw(viewNo);
}

void GeoHanaBody::initPos() {
    if (mParentCar) {
        mParentCar->getScale(&mScale);
        mParentCar->getBasePosition(&mBasePos);
        mParentCar->getFrontDirection(&mFrDir);

        mBasePos.scaleAdd(-sBodyToBodyLength, mFrDir, mBasePos);
        mLfDir.cross(mUpDir, mFrDir);
        mLfDir.normalize();
    }
}

void GeoHanaBody::calc() {}

void GeoHanaBody::move() {
    JGeometry::TVec3f parentPos;
    mParentCar->getBasePosition(&parentPos);

    if (mParentCar->getKind() != getKind()) {
        parentPos.y += mParentCar->getTirePosOffset(0).y;
    }

    JGeometry::TVec3f dir;
    dir.sub(parentPos, mBasePos);
    f32 bodyToLength = dir.length();
    dir.normalize();

    if (bodyToLength > sBodyToBodyLength) {
        bodyToLength = sBodyToBodyLength;
    }
    mBasePos.scaleAdd(-bodyToLength, dir, parentPos);

    mFrDir.set(dir);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();
    GeoCar::calcNewPose(&mFrDir, &mUpDir, &mLfDir, &mBasePos);

    mTireMat.getFrameCtrl()->setRate(GeoCar::getTireAnmRate(mParentHana->getDriveVelocity()));
    mTireMat.frameProc();

    if (mShadow) {
        mShadow->setCarPose(mFrDir, mBasePos);
    }

    mPos.set(mBasePos);
    mRotMtx.setXYZDir(mLfDir, mUpDir, mFrDir);
    mArea->searchShadow(mPos);
    GeoCar::getDarkAnm()->calc(*mArea);
}

const Vec &GeoHanaBody::getTirePosOffset(int tireNo) {
#line 327
    JUT_MINMAX_ASSERT(0, tireNo, 4);
    return sTireOfsTable[tireNo];
}
