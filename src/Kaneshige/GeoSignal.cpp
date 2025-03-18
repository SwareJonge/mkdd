#include "Kaneshige/Objects/GeoSignal.h"
#include "Sato/GeographyObj.h"
#include "Sato/ObjUtility.h"
#include "Sato/StateObserver.h"

u16 GeoSignal::sSupervisorCreateNum;
GeoSignalSupervisor *GeoSignal::sSupervisor;
J3DAnmTevRegKey *GeoSignal::sTevRegKey;

StateObserver::StateFuncSet<GeoSignal> GeoSignal::sTable[] = {
    {0, &GeoSignal::initFuncWait, &GeoSignal::doFuncWait }
};

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoSignalSupervisor::GeoSignalSupervisor() {
    mList.initiate();
}

GeoSignalSupervisor::~GeoSignalSupervisor() {
    GeoSignal::sSupervisor = nullptr;
}

void GeoSignalSupervisor::reset() {}

void GeoSignalSupervisor::calc() {}

void GeoSignalSupervisor::entrySignal(GeoSignal *signal) {
    mList.append(&signal->mLink);
}

GeoSignal::GeoSignal(const CrsData::SObject &obj) : GeographyObj(obj), mLink(this) {

    if (sSupervisorCreateNum == 0) {
        sSupervisor = new GeoSignalSupervisor();
        sSupervisorCreateNum++;
    }
    reset();

    mFrDir.set(mObjData->zNorm);
    mFrDir.scale(0.0001f);

    mUpDir.set(mObjData->yNorm);
    mUpDir.scale(0.0001f);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();

    sSupervisor->entrySignal(this);
}

void GeoSignal::reset() {
    GeographyObj::resetObject();
    _58 = 1;
    GeographyObj::setAllCheckKartHitFlag();
    StateObserver::ResetState();
    GeographyObj::setObjFlagSimpleDraw();
    GeographyObj::setObjFlagCheckItemHitting();
    mMat.resetFrame();
    mMat.setRate(1.0f);
    _168 = false;
}

void GeoSignal::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *brk = ObjUtility::getPtrCourseArc("/Objects/Signal1.brk");
    J3DAnmObjMaterial::setupTevRegAnmData(&sTevRegKey, mdlData, brk);
}

void GeoSignal::createColModel(J3DModelData *mdlData) {
    createBoundsCylinder(mdlData, 65.0f, 1100.0f);
}

void GeoSignal::createModel(JKRSolidHeap *slid, u32 viewNo) {
    mMat.setExModel(&mModel);
    mMat.attach(sTevRegKey);
}

void GeoSignal::simpleDraw(u32 viewNo) {
    mMat.anmFrameProc();
    GeographyObj::simpleDraw(viewNo);
}

void GeoSignal::calc() {
    StateObserver::ExecuteState();
    mMat.frameProc();
    mRotMtx.setXYZDir(mLfDir, mUpDir, mFrDir);
}

void GeoSignal::update() {
    mMat.anmFrameProc();
    GeographyObj::update();
}

void GeoSignal::InitExec() { Observer_FindAndInit(GeoSignal, 1); }

void GeoSignal::MoveExec() { Observer_FindAndExec(GeoSignal, 1); }

void GeoSignal::initFuncWait() {}

void GeoSignal::doFuncWait() {}
