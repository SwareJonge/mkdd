#include "Kaneshige/Objects/GeoKinoCar.h"

#include "JSystem/J3D/J3DModel.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/ExModel.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Sato/GeographyObj.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/ObjUtility.h"
#include "Sato/StateObserver.h"
#include "dolphin/mtx.h"

#include "mathHelper.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

J3DAnmTextureSRTKey *GeoKinoCar::sTireBtkAnm;

const Vec GeoKinoCar::sTireOfsTable[4] = {
    {120.0f, 0.0f, 100.0f},
    {-120.0f, 0.0f, 100.0f},
    {120.0f, 0.0f, -100.0f,},
    {-120.0f, 0.0f, -100.0f}
};
s16 GeoKinoCar::sOccurLimitSpeed = 50;
s16 GeoKinoCar::sNextOccurTime = 60;
f32 GeoKinoCar::sOccurVel0 = 180.0f;


GeoKinoCar::GeoKinoCar(const CrsData::SObject &obj) : GeoCar(obj) {
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
    
    mShadow = (GeoKinoShadow*)GetGeoObjMgr()->createSubObj(0x1002);
    reset();
    _58 = 1;
}

void GeoKinoCar::reset() {
    GeoCar::reset();
    GeographyObj::setObjFlagSimpleDraw();
    mTimer = 0;
    mTireMat.resetFrame();
}

void GeoKinoCar::createModel(JKRSolidHeap *slid, u32 viewNo) {
    mTireMat.setExModel(&mModel);
    mTireMat.attach(sTireBtkAnm);
}

void GeoKinoCar::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsCylinder(mdlData, 150.0f, 200.0f);
    _158 = 200.0f;
}

void GeoKinoCar::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *btk = ObjUtility::getPtrCourseArc("/Objects/Car_Kinoko1.btk");
    J3DAnmObjMaterial::setupTexSRTAnmData(&sTireBtkAnm, mdlData, btk);
}

const Vec &GeoKinoCar::getTirePosOffset(int tireNo) {
#line 238
    JUT_MINMAX_ASSERT(0, tireNo, 4);
    return sTireOfsTable[tireNo];
}

void GeoKinoCar::calc() {
    GeoCar::calc();
    mTireMat.getFrameCtrl()->setRate(getTireAnmRate());
    mTireMat.frameProc();

    if (mShadow) {        
        mShadow->setCarPose(mFrDir, mPos);
    }

    if (mTimer > 0) {
        mTimer--;
    }
}

void GeoKinoCar::simpleDraw(u32 viewNo) {
    mTireMat.anmFrameProc();
    GeoCar::setTevColor();
    GeographyObj::simpleDraw(viewNo);
}

void GeoKinoCar::hitKart(int kartNo) {
    u64 carStatus = KartCtrl::getKartCtrl()->GetKartStatus(kartNo);
    bool hit = false;

    if (carStatus & 0x40000) {
        hit = true;
        StateObserver::setState(2);
    }
    else if (carStatus & (0x4000 | 0x4)) {
        hit = true;
    }
    else {
        if (GetKartCtrl()->GetCarSpeed(kartNo) >= sOccurLimitSpeed) {
            hit = true;
        }
    }

    if (hit) {
        JGeometry::TVec3f dir;
        ObjUtility::getKartZdir(kartNo, &dir);
        occurKinoko(dir);
        GeoCar::setCrushSE(0x4002f);
    }
}

void GeoKinoCar::occurKinoko(const JGeometry::TVec3f &dir) {
    if (mTimer > 0) {
        return;
    }

    bool positive = true;
    if (dir.dot(mFrDir) < 0.0f)
        positive = false;
   
    mTimer = sNextOccurTime;

    JGeometry::TVec3f occurVel;
    JGeometry::TVec3f frDir(mFrDir);

    if (!positive) {
       frDir.scale(-1.0f);
    }
    occurVel.scaleAdd(0.3f, mUpDir, frDir);
    occurVel.normalize();
    occurVel.scale(sOccurVel0);
    JGeometry::TVec3f TStack_38;
    if (positive) {
        TStack_38.scaleAdd(100.0f, mFrDir,mPos);
    }
    else {
        TStack_38.scaleAdd(-100.0f, mFrDir,mPos);
    }

    GetItemObjMgr()->occurItem(5, TStack_38, occurVel);
}

void GeoKinoCar::occurKinoko(ItemObj *obj) {
    JGeometry::TVec3f dir;
    dir.sub(mPos, obj->getPos());
    dir.normalize();
    occurKinoko(dir);
}

void GeoKinoCar::hitItemGTurtle() {
    occurKinoko(GeographyObj::getColItemObj());
}

void GeoKinoCar::hitItemGTurtleBig() {
    StateObserver::setState(2);
    occurKinoko(GeographyObj::getColItemObj());
}

void GeoKinoCar::hitItemYoshiEgg() {
    occurKinoko(GeographyObj::getColItemObj());
}

void GeoKinoCar::hitItemRTurtle() {
    occurKinoko(GeographyObj::getColItemObj());
}
