#include "Kaneshige/Objects/GeoBombCar.h"
#include "JSystem/JGeometry/Vec.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Kaneshige/Objects/GeoCarShadow.h"
#include "Sato/GeographyObj.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/ObjUtility.h"
#include "Sato/StateObserver.h"

#include "Yamamoto/kartCtrl.h"
#include "mathHelper.h"

#include "JSystem/JAudio/JASFakeMatch2.h"
#include "types.h"

J3DAnmTextureSRTKey *GeoBombCar::sTireBtkAnm;

s16 GeoBombCar::sMaxBombReactFrame = 20;

const Vec GeoBombCar::sTireOfsTable[4] = {
    {100.0, 0.0f, 10.0f},
    {-100.0, 0.0f, 10.0f},
    {100.0, 0.0f, -10.0f},
    {-100.0, 0.0f, -10.0f},
};

GeoBombCar::GeoBombCar(const CrsData::SObject &obj) : GeoCar(obj) {
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

    mShadow = (GeoBombShadow *)GetGeoObjMgr()->createSubObj(0x1001);
    mColBounds = nullptr;
    mEffObj = new BombEffectObj();
    _26c = nullptr;
    mIsBlast = false;
    reset();
}

void GeoBombCar::reset() {
    GeoCar::reset();
    GeographyObj::setObjFlagSimpleDraw();
    _58 = 9;
    mEffObj->reset();
    _26c = nullptr;
    _274 = 0;
    mIsBlast = false;
    _278 = 0;
    _27c = mObjData->mParam3;
    mMat.resetFrame(); 
}

void GeoBombCar::createModel(JKRSolidHeap *slid, u32 p2) {
    mMat.setExModel(&mModel);
    mMat.attach(sTireBtkAnm);
}

void GeoBombCar::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsCylinder(mdlData, 80.0f, 250.0f);
    mColBounds = GeographyObj::getBounds(0);
    _158 = 250.0f;
}

const Vec &GeoBombCar::getTirePosOffset(int tireNo) {
#line 292
    JUT_MINMAX_ASSERT(0, tireNo, 4);
    return sTireOfsTable[tireNo];
}

void GeoBombCar::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *brk = ObjUtility::getPtrCourseArc("/Objects/Car_Bomb1.btk");
    J3DAnmObjMaterial::setupTexSRTAnmData(&sTireBtkAnm, mdlData, brk);
}

void GeoBombCar::setCurrentViewNo(u32 viewNo) {
    Mtx lightMtx;
    mModel.setCurrentViewNo(viewNo);
    ObjUtility::getCamDependLightMtx(viewNo, mPos, lightMtx);
    mModel.setEffectMtx(lightMtx, 1);
}

void GeoBombCar::update() {
    mMat.anmFrameProc();
    GeographyObj::update();
}

void GeoBombCar::simpleDraw(u32 viewNo) {
    mMat.anmFrameProc();
    GeoCar::setTevColor();
    GeographyObj::simpleDraw(viewNo);
}

void GeoBombCar::calc() {
    GeoCar::calc();
    mMat.getFrameCtrl()->setRate(getTireAnmRate());
    mMat.frameProc();

    if (mShadow) {
        mShadow->setCarPose(mFrDir, mPos);
    }

    if (_274 > 0) {
        _274--;
    }

    JGeometry::TVec3f translation(mPos);
    translation.scaleAdd(280.0f, mUpDir, translation);

    if (_26c) {
        _26c->setGlobalTranslation(translation);
        JGeometry::TVec3f ptclScale;
        ptclScale.setAll(6.0f);
        _26c->setGlobalParticleScale(ptclScale);
    }
    else {
        _26c = GetJPAMgr()->createEmt("bombchiri01", mPos);
        _26c->becomeContinuousParticle();
    }

    GetKartCtrl()->SetObjectPos(0, mPos);
}

void GeoBombCar::hitKart(int kartNo) {
    if (_274 == 0) {
        GeographyObj::clrCheckKartHitFlag(kartNo);
        StateObserver::setState(4);
    }    
}

void GeoBombCar::hitItemBomb() 
{
    if (_274 == 0) {
        StateObserver::setState(4);
    }
}

void GeoBombCar::hitItemWanWan() {
    StateObserver::setState(2);
}

void GeoBombCar::hitItemGTurtleBig() {
    GeoBombCar::hitItemBomb();
}

void GeoBombCar::hitItemGTurtle() {
    GeoBombCar::hitItemBomb();
}

void GeoBombCar::hitItemRTurtle() {
    GeoBombCar::hitItemBomb();
}

void GeoBombCar::hitItemFireBall() {
    GeoBombCar::hitItemBomb();
}

void GeoBombCar::hitItemYoshiEgg() {
    GeoBombCar::hitItemBomb();
}

void GeoBombCar::initCallBackDeath() {
    mEffObj->init(mRotMtx, 0, 1.0f);
    this->setBounds(mEffObj->getBlastBound(), 0);
    this->setObjFlagHidding();
    mShadow->setObjFlagHidding();
    _26c->stopCreateParticle();
    mIsBlast = true;
    _278 = 0;
}

void GeoBombCar::doCallBackDeath() {
    if (StateObserver::getStateCount() >= sMaxBombReactFrame) {
        _58 = 4;
        mIsBlast = false;
    }
    if (mEffObj->execute()) {
        GeographyObj::setBounds(mColBounds, 0);

        if (GeographyObj::IsPathMove()) {
            _278++;
            if (_278 >= _27c) {
                _58 = 9;
                GeographyObj::clrObjFlagHidding();
                mShadow->clrObjFlagHidding();
                _26c->playCreateParticle();
                StateObserver::setState(1);
                GeoCar::setTargetPoint(0);
                GeoCar::setAllObjectCollision(true);
            }
            else {
                setAllObjectCollision(false);
            }
        }
        else {
            _58 = 0;
            GeoCar::setAllObjectCollision(false);
            StateObserver::setState(0);
        }
        _274 = 180;
    }
    else {
        mIsBlast = true;
    }
}
