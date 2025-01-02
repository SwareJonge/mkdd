#include "Kaneshige/Objects/GeoAirJet.h"
#include "Sato/JPEffectMgr.h"

StateObserver::StateFuncSet<GeoAirJet> GeoAirJet::sTable[1] = {0, &GeoAirJet::initFuncWait, &GeoAirJet::doFuncWait};

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoAirJet::GeoAirJet(const CrsData::SObject &rObj) : GeographyObj(rObj)
{
    reset();
}

void GeoAirJet::reset() {
    resetObject();
    _58 = 0;
    setAllCheckKartHitFlag();
    clrObjFlagCheckItemHitting();
    ResetState();
    mDirZ.set(mObjData->zNorm);
    mDirZ.scale(0.0001f);
    mDirY.set(mObjData->yNorm);
    mDirY.scale(0.0001f);
    mDirX.cross(mDirY, mDirZ);
    mDirX.normalize();
    this->mEmt = nullptr;
}

void GeoAirJet::createColModel(J3DModelData *model) { createBoundsCylinder(50.0f * mScale.x, 100.0f * mScale.y, 1.0f); }

void GeoAirJet::calc() {
    ExecuteState();
    if(mEmt == nullptr) {
        mRotMtx.setXYZDir(mDirX, mDirY, mDirZ);
        mEmt = GetJPAMgr()->createEmt("mk_reiki_a", mPos);
        mEmt->setGlobalRMatrix(mRotMtx);
        mEmt->setGlobalTranslation(mPos);
    }
}

void GeoAirJet::InitExec() { Observer_FindAndInit(GeoAirJet, 1); }

void GeoAirJet::MoveExec() { Observer_FindAndExec(GeoAirJet, 1); }

void GeoAirJet::initFuncWait() {}

void GeoAirJet::doFuncWait() {}
