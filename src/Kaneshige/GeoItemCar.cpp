#include "Kaneshige/Objects/GeoItemCar.h"
#include "JSystem/J3D/J3DModel.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/Objects/GeoCar.h"
#include "Sato/GeographyObj.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/ObjUtility.h"
#include "Sato/Objects/GeoItemBox.h"
#include "Sato/StateObserver.h"
#include "Sato/stMath.h"
#include "Yamamoto/kartCtrl.h"
#include "types.h"

#include "mathHelper.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

J3DAnmTextureSRTKey *GeoItemCar::sBtkAnm;

f32 GeoItemCar::sSwingAmp = 10.0f;
s16 GeoItemCar::sSwingVel = 800;

const Vec GeoItemCar::sTireOfsTable[4] = {
    {100.0, -155.0f, 20.0f},
    {-100.0, -155.0f, 20.0f},
    {100.0, -155.0f, -20.0f},
    {-100.0, -155.0f, -20.0f},
};

GeoItemCar::GeoItemCar(const CrsData::SObject &obj) : GeoCar(obj)  {
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

    GeoCarSupervisor *pSupervisor = getSupervisor();
    _262 = pSupervisor->getCarNumber() * 0x6666;
    _264 = pSupervisor->getCarNumber() * 0x4444;
    reset();
}

void GeoItemCar::reset() {
    GeoCar::reset();
    GeographyObj::setObjFlagMainResource();
    GeographyObj::clrObjFlagCheckItemHitting();
    _58 = 0;
    _260 = _262;
    _266 = _264;
    _268 = 0.0f;
    _26c = 0.0f;
    _270.set(0.0f, 0.0f, 1.0f);
    _27c.set(0.0f, 1.0f, 0.0f);
    _288.set(1.0f, 0.0f, 0.0f);
    mMat.resetFrame();
    mMat.setRate(1.0f);

    mKartNo = 0;
    for (int i = 0; i < 2; i++) {
        mpEmitter[i] = nullptr;
    }

    if (GeographyObj::getShadowModel()) {
        GeographyObj::showShadow();
        GeographyObj::moveShadowModel();
    }
}

void GeoItemCar::loadAnimation() {
    J3DModelData *mdlData = mModel.getModelData();
    void *brk = ObjUtility::getPtrMainArc("/Item/itembox_normal.btk");
    J3DAnmObjMaterial::setupTexSRTAnmData(&sBtkAnm, mdlData, brk);
}

const char *GeoItemCar::getBmdFileName() { return "/Objects/Car_Item1.bmd"; }

const char *GeoItemCar::getShadowBmdFileName() { return GeoItemBox::getShadowBmdName(); }

void GeoItemCar::createModel(JKRSolidHeap *slid, u32 p2) {
    mModel.createDifferedModel(slid, p2, 0x200, false);
    mMat.setExModel(&mModel);
    mMat.attach(sBtkAnm);
}

void GeoItemCar::createColModel(J3DModelData *mdlData) {
    GeographyObj::createBoundsSphere(getColRadius(), 1.0f);
    _158 = 105.0f;
}

void GeoItemCar::calc() {
    _188 = getSwingHeight(0.0f, _260);
    _260 += sSwingVel;
    GeoCar::calc();
    mMat.frameProc();
    GeographyObj::moveShadowModel();

    for (int i = 0; i < 2; i++) {
        if (mpEmitter[i]) {
            JGeometry::TVec3f kartPos;
            ObjUtility::getKartPos(mKartNo, &kartPos);
            mpEmitter[i]->setGlobalTranslation(kartPos);
            if (mpEmitter[i]->isEnableDeleteEmitter()) {
                GetJPAMgr()->deleteEmt(mpEmitter[i]);
                mpEmitter[i] = nullptr;
            }

        }
    }
}

void GeoItemCar::setRMtx() {
    JGeometry::TVec3f v1;
    _268 += JMASSin(_266) * 0.001f;
    v1.scale(_268, _27c);

    JGeometry::TVec3f v2;
    _26c += JMASCos(_266) * -0.001f;
    v2.scale(_26c, _288);

    _270.add(v2);
    _270.add(v1);
    _270.normalize();
    _27c.cross(_270, _288);
    _27c.normalize();
    _288.cross(_27c, _270);

    _266 += 0x100;
    stMakeRMtx(mRotMtx, _288, _27c, _270);
}

void GeoItemCar::update() {
    mMat.anmFrameProc();
    GeographyObj::update();
}

const Vec &GeoItemCar::getTirePosOffset(int tireNo) {
#line 439
    JUT_MINMAX_ASSERT(0, tireNo, 4);
    return sTireOfsTable[tireNo];
}

void GeoItemCar::hitKart(int kartNo) {
    if (tstIsHitKartFlg(kartNo)) {
        hitItemBoxProc(kartNo, &mpEmitter[0]);
        mKartNo = kartNo;
        clrAllIsHitKartFlg();
    }

    if (StateObserver::getState() != 4) {
        StateObserver::setState(4);
    }
}

void GeoItemCar::initCallBackDeath() {
    clrCarCollision();
}

void GeoItemCar::doCallBackDeath() {
    if (GeographyObj::IsPathMove()) {
        StateObserver::setState(1);
        setTargetPoint(0);
    }
    else {
        StateObserver::setState(0);
    }
    GeoCar::setCarCollision();
}

bool GeoItemCar::hitItemBoxProc(int kartNo, JPABaseEmitter**paEmitter) {
    bool rolling = false;
    if (!GetKartCtrl()->CheckItem(kartNo)) {
        if (GetItemObjMgr()->IsAvailableRollingSlot(kartNo, 1)) {
            rolling = true;
            GetItemObjMgr()->startItemShuffleSingle(kartNo, false);
            GetKartCtrl()->GetKartSoundMgr(kartNo)->setSe(0x10013);
        }
    }

    JGeometry::TVec3f kartPos;
    ObjUtility::getKartPos(kartNo, &kartPos);

    if (rolling) {
        paEmitter[0] = GetJPAMgr()->createEmt(JPEffectMgr::getHashValue("mk_itemget_normal_a"), kartPos);
        paEmitter[1] = GetJPAMgr()->createEmt(JPEffectMgr::getHashValue("mk_itemget_normal_b"), kartPos);
    }
    else {        
        paEmitter[0] = GetJPAMgr()->createEmt(JPEffectMgr::getHashValue("mk_itemget_suka"), kartPos);
        paEmitter[1] = nullptr;
    }

    GetGeoObjMgr()->setKartHitItemBoxTrigger(kartNo);
    return rolling;
}
