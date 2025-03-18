#include "Kaneshige/Objects/GeoHanaCar.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/ExModel.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Kaneshige/Objects/GeoCarShadow.h"
#include "Kaneshige/Objects/GeoHanaBody.h"


#include "JSystem/JAudio/JASFakeMatch2.h"
#include "Sato/GeographyObj.h"
#include "Sato/GeographyObjMgr.h"
#include "mathHelper.h"

s16 GeoHanaCar::sHeadDownTime = 16;
f32 GeoHanaCar::sHeadDownHeight = 120.0f;
f32 GeoHanaCar::sHeadHeight = 750.0f;

const Vec GeoHanaCar::sTireOfsTable[] = {
    {160.0f, -165.0f, 220.0f},
    {-160.0f, -165.0f, 220.0f},
    {160.0f, -165.0f, -220.0f},
    {-160.0f, -165.0f, -220.0f}
};

GeoHanaCar::GeoHanaCar(const CrsData::SObject &obj) : GeoCar(obj) {

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

    mShadow = (GeoHanaShadow *)GetGeoObjMgr()->createSubObj(0x1000);
    mBodyCount = obj.mParam3;

#line 169
    JUT_ASSERT_MSG(mBodyCount <= MAX_HANABODY_COUNT, "HANACHAN BODY NUM OVER");

    for (int i = 0; i < MAX_HANABODY_COUNT; i++) {
        mBodies[i] = nullptr;
    }

    reset();
    createBody();
}

void GeoHanaCar::reset() {
    GeoCar::reset();
     _58 = 0xf;
    setObjFlagSimpleDraw();
    
    for (int i = 0; i < mBodyCount; i++) {
        if (mBodies[i]) {
            mBodies[i]->initPos();
        }
    }

}

void GeoHanaCar::createBody() {
    GeoCar *parent = this;
    for (int i = 0; i < mBodyCount; i++) {
        mBodies[i] = (GeoHanaBody*)GetGeoObjMgr()->createSubObj(0xfaa);
        mBodies[i]->setParent(this, parent);
        mBodies[i]->initPos();
        parent = mBodies[i];
    }
}

void GeoHanaCar::createModel(JKRSolidHeap *, u32) {}

void GeoHanaCar::loadAnimation() {
    mModel.getModelData();
}

void GeoHanaCar::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsCylinder(mdlData, 270.0f, 200.0f);
    JGeometry::TVec3f colPos;
    colPos.set(0.0f, -100.0f, 0.0f);
    setColObjPos(colPos, 0);
    _158 = 200.0f;
}

void GeoHanaCar::simpleDraw(u32 viewNo) {
    GeoCar::setTevColor();
    GeographyObj::simpleDraw(viewNo);
}

void GeoHanaCar::calc() {
    checkRoof();
    GeoCar::calc();
    if (mShadow) {
        mShadow->setCarPose(mFrDir, mPos);
    }

    for (int i = 0; i < mBodyCount; i++) {
        mBodies[i]->move();
    }
}

void GeoHanaCar::checkRoof() {
    JGeometry::TVec3f headPos;
    headPos.scaleAdd(250.0f, mFrDir, mPos);

    f32 headDownHeight = sHeadDownHeight / (f32)sHeadDownTime;

    CrsArea area;
    area.searchRoof(headPos);

    if (area.isInside()) {
        JGeometry::TVec3f roofPos;
        area.getRoofPosition(&roofPos);
        if (headPos.y + sHeadHeight > roofPos.y) {
            _188 -= headDownHeight;
            if (_188 < -sHeadDownHeight) {
                _188 = -sHeadDownHeight;
            }
        }
    }
    else {
        _188 += headDownHeight;
        if (_188 > 0.0f) {
            _188 = 0.0f;
        }
    }
}

const Vec &GeoHanaCar::getTirePosOffset(int tireNo) {
#line 381
    JUT_MINMAX_ASSERT(0, tireNo, 4);
    return sTireOfsTable[tireNo];
}
