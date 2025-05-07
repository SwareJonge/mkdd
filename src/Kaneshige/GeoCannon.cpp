#include "Kaneshige/Objects/GeoCannon.h"
#include "JSystem/J3D/J3DFrameCtrl.h"
#include "JSystem/J3D/J3DModel.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JMath/JMath.h"
#include "Sato/GeographyObj.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/ObjUtility.h"
#include "Sato/StateObserver.h"
#include "types.h"

u16 GeoCannon::sSupervisorCreateNum;
GeoCannonSupervisor *GeoCannon::sSupervisor;
J3DAnmTevRegKey *GeoCannon::sTevRegKey;
J3DAnmTransform *GeoCannon::sBcaAnmTrans;
J3DMtxCalc *GeoCannon::sBcaMtxCalc;

StateObserver::StateFuncSet<GeoCannon> GeoCannon::sTable[1] = {0, &GeoCannon::initFuncWait, &GeoCannon::doFuncWait};

char *GeoCannon::sEmitterName[4] = {
    "mk_dkCannon_a",
    "mk_dkCannon_b",
    "mk_dkCannon_c",
    "mk_dkCannon_d"
};

GeoCannonSupervisor::GeoCannonSupervisor() {
    mList.initiate();
}

GeoCannonSupervisor::~GeoCannonSupervisor() {
    GeoCannon::sSupervisor = nullptr;
}

void GeoCannonSupervisor::reset() {}

void GeoCannonSupervisor::calc() {}

void GeoCannonSupervisor::entryCannon(GeoCannon *cannon) {
    mList.append(&cannon->mLink);
}

GeoCannon *GeoCannonSupervisor::searchCannon(s32 id) {
    GeoCannon *cannon = nullptr;
    for (JSUListIterator<GeoCannon> it(mList.getFirst()); it != mList.getEnd(); it++) {
        if (it->isSameID(id)) {
            cannon = it.getObject();
            break;
        }
    }
    return cannon;
}

GeoCannon::GeoCannon(const CrsData::SObject &obj) : GeographyObj(obj), mLink(this) {
    if (sSupervisorCreateNum == 0) {
        sSupervisor = new GeoCannonSupervisor();
        sSupervisorCreateNum++;
    }
    GeographyObj::NewAnmCtrl();
    reset();
    sSupervisor->entryCannon(this);
}

void GeoCannon::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrObjFlagCheckGeoHitting();
    GeographyObj::clrObjFlagCheckItemHitting();
    GeographyObj::clrAllCheckKartHitFlag();

    if (mObjData->mParam3 != 0) {
        setObjFlagHidding();
    }
    else {
        clrObjFlagHidding();
    }

    StateObserver::ResetState();
    mFrDir.set(mObjData->zNorm);
    mFrDir.scale(0.0001f);
    mUpDir.set(mObjData->yNorm);
    mUpDir.scale(0.0001f);
    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();
    _18c = mObjData->mParam4;

    s16 v = (mObjData->mParam2 << 16) / 360;
    f32 c = JMASCos(v);
    f32 s = JMASSin(v);

    _190.x = (c * mFrDir.x + (s * mUpDir.x));
    _190.y = (c * mFrDir.y + (s * mUpDir.y));
    _190.z = (c * mFrDir.z + (s * mUpDir.z));

    _190.normalize();
    GeoCannon::resetAnimation();
    mMat.resetFrame();
    mMat.getFrameCtrl()->stop();

    for (int i = 0; i < 4; i++) {
        mpaEmitter[i] = nullptr;
    }

}

void GeoCannon::resetAnimation() {
    if (GeographyObj::getAnmCtrl()->IsAvailableTrans()) {
        J3DFrameCtrl *ctrl = GeographyObj::getAnmCtrl()->getFrameCtrl(0);
        ctrl->setAttribute(0);
        ctrl->reset();
        ctrl->stop();
    }
}

void GeoCannon::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *bca = ObjUtility::getPtrCourseArc("/Objects/Cannon1.bca");
    J3DAnmObjTrans::setupTransAnmData(&sBcaAnmTrans, &sBcaMtxCalc, mdlData, bca);
    void *brk = ObjUtility::getPtrCourseArc("/Objects/Cannon1.brk");
    J3DAnmObjMaterial::setupTevRegAnmData((J3DAnmTevRegKey **)&sTevRegKey, mdlData, brk);
}

void GeoCannon::createModel(JKRSolidHeap *slid, u32 p2) {
    mModel.createDifferedModel(slid, p2, 0x1000000, false);
    GeographyObj::getAnmCtrl()->InitRegistration(1, &mModel);
    GeographyObj::getAnmCtrl()->RegisterTrans(0, sBcaAnmTrans, sBcaMtxCalc);
    GeoCannon::resetAnimation();
    mMat.setExModel(&mModel);
    mMat.attach(sTevRegKey);

}

void GeoCannon::calc() {
    StateObserver::ExecuteState();
    mMat.frameProc();
    mRotMtx.setXYZDir(mLfDir, mUpDir, mFrDir);
}

void GeoCannon::update() {
    mMat.anmFrameProc();
    GeographyObj::update();

    int anmIdx = mModel.getModelData()->getJointName()->getIndex("taru");
    const Mtx &anmMtx = mModel.getModel()->getAnmMtx(anmIdx);

    for (int i = 0; i < 4; i++) {
        if (mpaEmitter[i]) {
            if (mpaEmitter[i]->isEnableDeleteEmitter()) {
                mpaEmitter[i] = nullptr;
            }
            else {
                mpaEmitter[i]->setGlobalRMatrix(mRotMtx);
                JGeometry::TVec3f translation(anmMtx[0][3], anmMtx[1][3], anmMtx[2][3]);
                mpaEmitter[i]->setGlobalTranslation(translation);
            }
        }
    }
}

void GeoCannon::InitExec() { Observer_FindAndInit(GeoCannon, 1); }

void GeoCannon::MoveExec() { Observer_FindAndExec(GeoCannon, 1); }

void GeoCannon::shot() {
    if (!GeographyObj::tstObjFlagHidding()) {
        J3DFrameCtrl *ctrl = GeographyObj::getAnmCtrl()->getFrameCtrl(0);
        ctrl->reset();
        ctrl->setRate(1.0f);
        mMat.resetFrame();
        mMat.setRate(1.0f);
        for (int i = 0; i < 4; i++) {
            mpaEmitter[i] = GetJPAMgr()->createEmt(sEmitterName[i], mPos);
        }
    }
}

void GeoCannon::initFuncWait() {}

void GeoCannon::doFuncWait() {}

#include "JSystem/JAudio/JASFakeMatch2.h"
