#include "Kaneshige/Objects/GeoKinojii.h"
#include "JSystem/J3D/J3DModel.h"
#include "Kaneshige/LightMgr.h"
#include "Kaneshige/RaceLight.h"
#include "Sato/GeographyObj.h"
#include "Sato/ObjUtility.h"
#include "Sato/StateObserver.h"
#include "Yamamoto/kartCtrl.h"
#include "dolphin/mtx.h"

J3DAnmTransform *GeoKinojii::sBcaAnmTrans;
J3DMtxCalc *GeoKinojii::sBcaMtxCalc;
J3DAnmTexPattern *GeoKinojii::sWinkBtpAnm;

s16 GeoKinojii::sWinkRate = 60;
s16 GeoKinojii::sWinkInterval = 5;

StateObserver::StateFuncSet<GeoKinojii> GeoKinojii::sTable[1] = {
    {0, &GeoKinojii::initFuncWait, &GeoKinojii::doFuncWait}
};

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoKinojii::GeoKinojii(const CrsData::SObject &obj) : GeographyObj(obj) {
    GeographyObj::NewAnmCtrl();
    reset();
}

void GeoKinojii::reset() {
    GeographyObj::resetObject();
    _58 = 0;
    GeographyObj::clrObjFlagCheckGeoHitting();
    GeographyObj::clrObjFlagCheckItemHitting();
    GeographyObj::clrAllCheckKartHitFlag();
    StateObserver::ResetState();

    mFrDir.set(mObjData->zNorm);
    mFrDir.scale(0.0001f);
    
    mUpDir.set(mObjData->yNorm);
    mUpDir.scale(0.0001f);
    
    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();

    mKartNo = 0;
    mWinkTimer = 0;
    _17c.set(0.0f, 53.5f, 16.5f);
    mMat.resetFrame();
    resetAnimation();
}

void GeoKinojii::resetAnimation() {
    if (GeographyObj::getAnmCtrl()->IsAvailableTrans()) {
        J3DFrameCtrl *ctrl = GeographyObj::getAnmCtrl()->getFrameCtrl(0);
        ctrl->reset();
        ctrl->stop();
    }
}

void GeoKinojii::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *btp = ObjUtility::getPtrCourseArc("/Objects/kinojii_wink.btp");
    J3DAnmObjMaterial::setupTexPatternAnmData(&sWinkBtpAnm, mdlData, btp);

    void *bca = ObjUtility::getPtrCourseArc("/Objects/kinojii_drive.bca");
    J3DAnmObjTrans::setupTransAnmData(&sBcaAnmTrans, &sBcaMtxCalc, mdlData, bca);

    for (u16 i = 0; i < mdlData->getShapeNum(); i++) {
        mdlData->getShapeNodePointer(i)->setTexMtxLoadType(0x2000);
    }
}

void GeoKinojii::createModel(JKRSolidHeap *slid, u32 viewNo) {
    mModel.createDifferedModel(slid, viewNo, 0x20200, true);
    GeographyObj::getAnmCtrl()->InitRegistration(1, &mModel);
    GeographyObj::getAnmCtrl()->RegisterTrans(0, sBcaAnmTrans, sBcaMtxCalc);

    resetAnimation();
    mMat.setExModel(&mModel);
    mMat.attach(sWinkBtpAnm);
    mMat.getFrameCtrl()->setAttribute(1);
}

void GeoKinojii::setCurrentViewNo(u32 viewNo) {
    mModel.setCurrentViewNo(viewNo);
    LightObj *light = LightMgr::getManager()->searchLight(RaceSceneLight::getSceneLightTagName(viewNo));
    MtxPtr efct_m = light->getEffectMtx();
    mModel.setEffectMtx(efct_m, 1);
}

void GeoKinojii::calc() {
    StateObserver::ExecuteState();
    mRotMtx.setXYZDir(mLfDir, mUpDir, mFrDir);
    mMat.frameProc();
}

void GeoKinojii::update() {
    mMat.anmFrameProc();
    GeographyObj::update();
}

void GeoKinojii::InitExec() { Observer_FindAndInit(GeoKinojii, 1); }

void GeoKinojii::MoveExec() { Observer_FindAndExec(GeoKinojii, 1); }

void GeoKinojii::initFuncWait() {}

void GeoKinojii::doFuncWait() {

    f32 r = GetKartCtrl()->getKartBody(mKartNo)->_388;

    if (r < -1.0f) {
        r = -1.0f;
    }
    else if (r > 1.0f) {
        r = 1.0f;
    }

    J3DFrameCtrl *frameCtrl = GeographyObj::getAnmCtrl()->getFrameCtrl(0);
    frameCtrl->setFrame((1.0f + r) * 0.5f * 40.0f);
    frameCtrl->stop();

    JGeometry::TVec3f kartPos;
    ObjUtility::getKartPos(mKartNo, &kartPos);
    ObjUtility::getKartZdir(mKartNo, &mFrDir);
    ObjUtility::getKartYdir(mKartNo, &mUpDir);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();

    mPos.set(kartPos);
    mPos.scaleAdd(_17c.x, mLfDir, mPos);
    mPos.scaleAdd(_17c.y, mUpDir, mPos);
    mPos.scaleAdd(_17c.z, mFrDir, mPos);

    if (mMat.getFrameCtrl()->getRate() == 0.0f) {
        mWinkTimer++;
        if (mWinkTimer > sWinkInterval) {
            u32 rnd = GeographyObj::getGeoRnd()->getRandom();
            if ((rnd % sWinkRate) == 0) {
                mMat.getFrameCtrl()->reset();
            }
        }
    }
    else {
        mWinkTimer = 0;
    }
}
