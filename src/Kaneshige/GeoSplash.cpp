#include "Kaneshige/Objects/GeoSplash.h"
#include "Sato/GeographyObj.h"
#include "Sato/StateObserver.h"

u16 GeoSplash::sSupervisorCreateNum;
GeoSplashSupervisor *GeoSplash::sSupervisor;

StateObserver::StateFuncSet<GeoSplash> GeoSplash::sTable[1] = {
    {0, &GeoSplash::initFuncWait, &GeoSplash::doFuncWait }
};

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoSplashSupervisor::GeoSplashSupervisor() {
    mList.initiate();
}

GeoSplashSupervisor::~GeoSplashSupervisor() {
    GeoSplash::sSupervisor = nullptr;
}

void GeoSplashSupervisor::reset() {}

void GeoSplashSupervisor::calc() {}

void GeoSplashSupervisor::entry(GeoSplash *splash) {
    mList.append(&splash->mLink);
}

GeoSplash *GeoSplashSupervisor::search(u8 id) {
    GeoSplash *splash = nullptr;
    for (JSUListIterator<GeoSplash> it(mList.getFirst()); it != mList.getEnd(); it++) {
        if (it->isSameID(id)) {
            splash = it.getObject();
            break;
        }
    }

    return splash;
}

GeoSplash::GeoSplash(const CrsData::SObject &obj) : GeographyObj(obj), mLink(this) {
    if (sSupervisor == nullptr) {
        sSupervisor = new GeoSplashSupervisor();
    }
    sSupervisor->entry(this);
    reset();
}

void GeoSplash::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrAllCheckKartHitFlag();
    GeographyObj::clrObjFlagCheckItemHitting();
    StateObserver::ResetState();
    mFrDir.set(mObjData->zNorm);
    mFrDir.scale(0.0001f);

    mUpDir.set(mObjData->yNorm);
    mUpDir.scale(0.0001f);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();
}

void GeoSplash::calc() {
    StateObserver::ExecuteState();
    mRotMtx.setXYZDir(mLfDir, mUpDir, mFrDir);
}

void GeoSplash::InitExec() { Observer_FindAndInit(GeoSplash, 1); }

void GeoSplash::MoveExec() { Observer_FindAndExec(GeoSplash, 1); } 

f32 GeoSplash::getSurfaceHeight() {
    return mPos.y;
}

void GeoSplash::initFuncWait() {}

void GeoSplash::doFuncWait() {}
