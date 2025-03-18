#include "Kaneshige/Objects/GeoWater.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JMath/JMath.h"
#include "Sato/GeographyObj.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ObjUtility.h"
#include "Sato/StateObserver.h"

u16 GeoWater::sSupervisorCreateNum;
GeoWaterSupervisor *GeoWater::sSupervisor;
J3DAnmTextureSRTKey *GeoWater::sBtkAnm;

f32 GeoWater::sStartAngle = 90.0f;

StateObserver::StateFuncSet<GeoWater> GeoWater::sTable[] = {
    {0,& GeoWater::initFuncWait, &GeoWater::doFuncWait}
};

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoWaterSupervisor::GeoWaterSupervisor() {
    mList.initiate();
}

GeoWaterSupervisor::~GeoWaterSupervisor() {
    GeoWater::sSupervisor = nullptr;
}

void GeoWaterSupervisor::reset() {}

void GeoWaterSupervisor::calc() {}

void GeoWaterSupervisor::entry(GeoWater *water) {
    mList.append(&water->mLink);
}

GeoWater *GeoWaterSupervisor::searchWater(s32 id) {
    GeoWater *water = nullptr;
    for (JSUListIterator<GeoWater> it(mList.getFirst()); it != mList.getEnd(); it++) {
        if (it->isSameID(id)) {
            water = it.getObject();
            break;
        }
    }

    return water;
}

GeoWater::GeoWater(const CrsData::SObject &obj) : GeographyObj(obj), mLink(this)  {
    if (sSupervisorCreateNum == 0) {
        sSupervisor = new GeoWaterSupervisor();
        sSupervisorCreateNum++;
    }
    sSupervisor->entry(this);
    reset();
    mSubs[3] = (GeoWaterUp2*)GetGeoObjMgr()->createSubObj(0xda5);
    mSubs[2] = (GeoWaterUp*)GetGeoObjMgr()->createSubObj(0xda4);
    mSubs[0] = (GeoWaterDown*)GetGeoObjMgr()->createPrevDrawSubObj(0xda2);
    mSubs[1] = (GeoWaterMiddleD*)GetGeoObjMgr()->createPrevDrawSubObj(0xda3);
}

void GeoWater::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrAllCheckKartHitFlag();
    GeographyObj::clrObjFlagCheckItemHitting();
    StateObserver::ResetState();
    GeographyObj::setObjFlagSimpleDraw();
    
    if (!RaceMgr::getManager()->isCrsDemoMode() && RaceMgr::getManager()->getProcLevel() > 0) {
        setObjFlagHidding();
    }
    else {
        clrObjFlagHidding();
    }
    clrObjFlagLODBias();

    mFrDir.set(mObjData->zNorm);
    mFrDir.scale(0.0001f);

    mUpDir.set(mObjData->yNorm);
    mUpDir.scale(0.0001f);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();

    _19c = 450.0f;
    if (mObjData->mParam7 != 0) {
        _19c = mObjData->mParam7;
    }

    _1a0 = 800.0f;
    if (mObjData->mParam8 != 0) {
        _1a0 = mObjData->mParam8;
    }

    mAngle = 65536.0f * sStartAngle / 360.0f;
    mAngleInc = 32;
    if (mObjData->mParam6 != 0) {
        mAngleInc = mObjData->mParam6;
    }

    _1a8 = 0.0f;
    _1ac = 0.0007f;
    _1b0 = -50.0f;
    _1b4 = 0.97f;
    _1b8 = 0.0f;
    mMat.resetFrame();
}

void GeoWater::loadAnimation() {
    void *btk = ObjUtility::getPtrCourseArc("/Objects/sea1_spc.btk");
    J3DAnmObjMaterial::setupTexSRTAnmData(&sBtkAnm, mModel.getModelData(), btk);
}

void GeoWater::createModel(JKRSolidHeap *slid, u32 viewNo) {
    mMat.setExModel(&mModel);
    mMat.attach(sBtkAnm);
}

void GeoWater::simpleDraw(u32 viewNo) {
    mMat.anmFrameProc();
    GeographyObj::simpleDraw(viewNo);
}

void GeoWater::update() { GeographyObj::update(); }

void GeoWater::calc() {
    StateObserver::ExecuteState();
    mRotMtx.setXYZDir(mLfDir, mUpDir, mFrDir);
    mMat.frameProc();
}

void GeoWater::InitExec() { Observer_FindAndInit(GeoWater, 1); }

void GeoWater::MoveExec() { Observer_FindAndExec(GeoWater, 1); }

f32 GeoWater::getSurfaceHeight() {
    return mPos.y;
}

void GeoWater::initFuncWait() {}

void GeoWater::doFuncWait() {
    f32 newAng = 0.5f * (1.0f + JMASSin(mAngle));
     
    JGeometry::TVec3f offsetPos(mPos);
    offsetPos.y = newAng * (_1a0 - _19c) + _19c;

    for (int i = 0; i < 4; i++) {
        JGeometry::TVec3f newPos(offsetPos);

        switch (i) {
        case 0: {
            if (_1a8 < newAng) {
                _1a8 = newAng;
            }
            else {
                _1a8 -= _1ac;
                if (_1a8 < newAng) {
                    _1a8 = newAng;
                }
            }
            newPos.y = _19c + _1b0 + _1a8 * (_1a0 - _19c);
            break;
        }
        case 3: {
            f32 ang = newAng;
            if (mAngle > -0x4000 && mAngle < 0x4000) {
                
                if (newAng > 0.5f) {
                    ang = ((newAng - 0.5f) * _1b4 + 0.5f);
                }
                _1b8 = (newAng - ang);
            }
            else {
                ang = (newAng - _1b8);
                if (ang < 0.0f) {
                    ang = 0.0f;
                }
            }
            newPos.y = (ang * (_1a0 - _19c) + _19c);
            break;
        }
        default: {
            newPos.y = offsetPos.y;
            break;
        }
        }

        mSubs[i]->setPosition(newPos);
        mSubs[i]->mScale.set(mScale);
    }
    mSubs[3]->getPosition(&mPos);
    mAngle += mAngleInc;
}
