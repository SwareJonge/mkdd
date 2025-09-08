#include "Sato/Objects/GeoItemBox.h"
#include "JSystem/JGeometry/Matrix.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JParticle/JPAEmitter.h"
#include "Kaneshige/RaceMgr.h"
#include "Osako/shadowModel.h"
#include "Sato/GeographyObj.h"

#include "Sato/GeographyObjMgr.h"
#include "Sato/ItemObj.h"
#include "Sato/ItemObjMgr.h"
#include "Sato/JPEffectMgr.h"
#include "Sato/ObjUtility.h"
#include "Sato/Objects/GeoObjPathMove.h"
#include "Yamamoto/kartCtrl.h"
#include "mathHelper.h"
#include "JSystem/JAudio/JASFakeMatch2.h"
#include "types.h"

namespace {
    u8 sInactiveTime = 60;
    u8 sUseInactiveTime = 60;
}

RotAnimationObj::RotAnimationObj() {
    mRandDir = 0;
    _28 = 0.0f;
    _2c = 0.0f;
    mModelXDir.zero();
    mModelYDir.zero();
    mModelZDir.zero();
}

void RotAnimationObj::resetRotAnimation(stRandom *rand) {
    mRandDir = rand->get();
    _28 = 0.0f;
    _2c = 0.0f;
}

void RotAnimationObj::setModelXYZDir(const JGeometry::TVec3f &xDir, const JGeometry::TVec3f &yDir, const JGeometry::TVec3f &zDir) {
    mModelYDir = yDir;
    mModelXDir = xDir;
    mModelZDir = zDir;
}

void RotAnimationObj::calcRotAnimation(JGeometry::TPos3f &rot) {
    JGeometry::TVec3f scaledY;
    _28 += JMASSin(mRandDir) * 0.001f;
    scaledY.scale(_28, mModelYDir);
    
    JGeometry::TVec3f scaledX;
    _2c += JMASCos(mRandDir) * -0.001f;
    scaledX.scale(_2c, mModelXDir);

    mModelZDir.add(scaledX);
    mModelZDir.add(scaledY);
    mModelZDir.normalize();
    mModelYDir.cross(mModelZDir, mModelXDir);
    mModelYDir.normalize();
    mModelXDir.cross(mModelYDir, mModelZDir);
    mRandDir += 0x100;
    stMakeRMtx(rot, mModelXDir, mModelYDir, mModelZDir);
}

GeoAnmTableEntry GeoItemBox::sMatAnmTable = {
    0, "/Item/itembox_normal.btk"
};

GeoAnmTableEntry GeoItemBox::sMatAnmTable4P = {
    0, "/Item/itembox_normal_4p.btk"
};

GeoAnmTableEntry GeoItemBox::sMatAnmTableMiniBomb = {
    0, "/Objects/MiniGameitembox_bomb.btk"
};

GeoAnmTableEntry GeoDoubleItemBox::sDoubleMatAnmTable = {
    0, "/Item/itembox_double.btk"
};

GeoAnmTableEntry GeoDoubleItemBox::sDoubleMatAnmTable4P = {
    0, "/Item/itembox_double_4p.btk"
};

GeoAnmTableEntry GeoDoubleItemBox::sDoubleMatAnmTableMiniBomb = {
    0, "/Objects/MiniGameitemboxW_bomb.btk"
};

GeoItemBox::GeoItemBox(const CrsData::SObject &object) : GeographyObj(object) {
    GeoItemBox::initialize();
}

GeoItemBox::GeoItemBox() : GeographyObj(1) {
    GeoItemBox::initialize();
}

void GeoItemBox::initialize() {
    GeographyObj::setObjFlagShadow();
    GeographyObj::setObjFlagMainResource();
    if (RCMGetManager()->isTinyProcess()) {
        GeographyObj::setObjFlagSimpleDraw();
    }
    mObjPos.zero();
    _1f8 = false;
}

GeoAnmTableEntry *GeoItemBox::getAnmTbl() {
    if (RCMGetManager()->isTinyProcess()) {
        return nullptr;
    }

    GeoAnmTableEntry *ret = &sMatAnmTable;
    if (GetGeoObjMgr()->isBombBattle()) {
        ret = &sMatAnmTableMiniBomb;
    }
    else if (RCMGetManager()->getConsoleNumber() >= 2) {
        if (!GetGeoObjMgr()->isMiniGame()) {
            ret = &sMatAnmTable4P;
        }
    }
    return ret;
}

u16 GeoItemBox::getSizeAnmTbl() {
    if (RCMGetManager()->isTinyProcess()) {
        return 0;
    }

    u32 ret = sizeof(sMatAnmTable);
    if (GetGeoObjMgr()->isBombBattle()) {
        ret = sizeof(sMatAnmTableMiniBomb);
    }
    else if (RCMGetManager()->getConsoleNumber() >= 2) {
        if (!GetGeoObjMgr()->isMiniGame()) {
            ret = sizeof(sMatAnmTable4P);
        }
    }
    return ret;
}

void GeoItemBox::setInactiveTime() {
    sUseInactiveTime = sInactiveTime;
}

void GeoItemBox::resetRot() {
    // UNUSED
    mMode = 1;
    _180 = 0;
    resetRotAnimation(getGeoRnd());
}
void GeoItemBox::reset() {
    GeographyObj::resetObject();
    GeographyObj::setBoundScaleAll();
    resetRot();
    _58 = 0;
    GeographyObj::clrObjFlagCheckItemHitting();
    
    if (mObjData) {
        mPos.y += mObjData->mParam1;
        if (getKind() == 0xc) {
            mPos.y += 75.0f;
        }
        if (mObjData->mParam3 > 0) {
            _1f8 = true;
        }
    }

    JGeometry::TVec3f modelXDir, modelYDir, modelZDir;
    GeographyObj::getAssignedModelXYZdir(&modelXDir, &modelYDir, &modelZDir);
    mModelYDir = modelYDir;
    mModelXDir = modelXDir;
    mModelZDir = modelZDir;
    mRank = -1;

    if (mShadowMdl) {
        GeographyObj::showShadow();
        GeographyObj::moveShadowModel();
    }

    _1e8 = 0;
    mpEmitter[1] = nullptr;
    mpEmitter[0] = nullptr;
    mObjPos = mPos;
}

void GeoItemBox::createModel(JKRSolidHeap *heap, u32 viewNo) {
    if (!RCMGetManager()->isTinyProcess()) {
        mModel.createDifferedModel(heap, viewNo, 0x200, true);
    }
}

void GeoItemBox::createColModel(J3DModelData *) {
    GeographyObj::createBoundsSphere(150.0f, 1.0f);
}

void GeoItemBox::createShadowModel(JKRSolidHeap *heap, u32 p2) {
    GeographyObj::createShadowModel(heap, p2);
    GeographyObj::hideAllShadow();
}

const char *GeoItemBox::getBmdFilePtr() {
    // UNUSED
}

const char *GeoItemBox::getBmdName() {
    const char *ret = "/Item/itembox_normal.bmd";
    if (RCMGetManager()->isTinyProcess()) {
        ret = "/Item/itembox_lan.bmd";
    }
    else if (GetGeoObjMgr()->isBombBattle()) {
        ret = "/Objects/MiniGameitembox_bomb.bmd";
    }
    else if (RCMGetManager()->getConsoleNumber() >= 2) {
        if (!GetGeoObjMgr()->isMiniGame()) {
            ret = "/Item/itembox_normal_4p.bmd";
        }
    }
    return ret;
}

const char *GeoItemBox::getShadowBmdName() {
    static const char *cShadowBmdName = "/Item/IBNkage.bmd";
    return cShadowBmdName;
}

const char *GeoItemBox::getBmdFileName() {
    return getBmdName();
}

const char *GeoItemBox::getShadowBmdFileName() {
    return getShadowBmdName();
}

void GeoItemBox::calc() {
    doMode();
    rotAnimation();
}

void GeoItemBox::rotAnimation() {
    calcRotAnimation(mRotMtx);
}

void GeoItemBox::setItemSlot(const int kartId) {
    // UNUSED
    u32 soundID = 0x10013;
    switch (getKind()) {
    case 0xc: {
        soundID = 0x10030;
        GetItemObjMgr()->startItemShuffleDouble(kartId);
        break;
    }
    default:  {
        GetItemObjMgr()->startItemShuffleSingle(kartId, _1f8);
        break;
    }
    }
    GetKartCtrl()->GetKartSoundMgr(kartId)->setSe(soundID);
}


void GeoItemBox::doMode() {
    switch (mMode) {
    case 0:
        doInactive();
        break;
    case 1:
        doOpening();
        break;
    case 2:
        break;
    case 3:
        doEnding();
        break;
    }
}

void GeoItemBox::setEffect(bool isStaffRoll) {
    // UNUSED
    JGeometry::TVec3f kartPos;
    ObjUtility::getKartPos(mKartIdx, &kartPos);
    u8 numEmts;
    u16 hashVal[2];    
    if (isStaffRoll) {
        numEmts = 2;
        for (u8 i = 0; i < 2; i++) {
            hashVal[i] = JPEffectMgr::getHashValue(ObjUtility::concatSimpleFileName("mk_itemget_normal", i));
        }
    }
    else {
        numEmts = 1;
        hashVal[0] = JPEffectMgr::getHashValue("mk_itemget_suka");
    }

    for (u8 i = 0; i < numEmts; i++) {
        if (mpEmitter[i] == nullptr) {
            mpEmitter[i] = GetJPAMgr()->createEmt(hashVal[i], kartPos);
        }
    }
}

void GeoItemBox::doInactive() {
    for (u8 i = 0; i < 2; i++) {
        JPABaseEmitter *emt = mpEmitter[i];
        if (emt) {
            JGeometry::TVec3f kartPos;
            ObjUtility::getKartPos(mKartIdx, &kartPos);
            
            emt->setGlobalTranslation(kartPos);
            if (emt->isEnableDeleteEmitter()) {
                GetJPAMgr()->deleteEmt(emt);
                mpEmitter[i] = nullptr;
            }
        }
    }

    if (RCMGetManager()->isStaffRoll()) {
        return;
    }
    if (++_180 < sUseInactiveTime) {
        return;
    }

    u8 inactiveEmts = 0;
    for (u8 i = 0; i < 2; i++) {
        if (mpEmitter[i] == nullptr) {
            inactiveEmts++;
        }
    }

    if (inactiveEmts < 2) {
        return;
    }

    if (mObjData->mParam2 == 0 && (getKind() == 1 || getKind() == 0xc)) {
        GetGeoObjMgr()->setNewItemBoxObj(this);
    }
    else {
        mMode = 1;
        GeographyObj::showShadow();
        GeographyObj::clrObjFlagHidding();
        GeographyObj::setAllCheckKartHitFlag();
        
        _180 = 0;
        setOpeningPos();    
    }
}

void GeoItemBox::doOpening() {
    if (mShadowMdl->isInvisibleAll()) {
        mShadowMdl->setVisibleAll();
    }
    f32 scalar = (++_180) / 20.0f;
    mScale = mObjData->scale;
    
    if (scalar < 1.0) {
        mScale.scale(scalar);
        mShadowMdl->getModel()->setBaseScale(mScale);
    }
    else {
        mMode = 2;
        _180 = 0;
        mScale.scale(1.0f);
    }
}

void GeoItemBox::doEnding() {
    f32 scalar = (++_180);
    if (scalar < 1.0) { 
        mScale = mObjData->scale;
        mScale.scale(1.0f - scalar);
    }
    else {
        doHide();
    }
    mShadowMdl->getModel()->setBaseScale(mScale);
}

void GeoItemBox::doHide() {
    mMode = 0;
    GeographyObj::hideAllShadow();
    GeographyObj::setObjFlagHidding();
    _180 = false;

    mScale.setAll(0.001f);
    mVel.zero();
}

void GeoItemBox::doKartColCallBack(const int kartId) {
    if (mMode != 2) {
        return;
    }

    GetGeoObjMgr()->setKartHitItemBoxTrigger(kartId);
    mRank = RCMGetManager()->getKartChecker(kartId)->getRank();
    mKartIdx = kartId;
    bool isStaffRoll = false;

    if (RCMGetManager()->isStaffRoll()) {
        isStaffRoll = true;
    }
    else {
        if (!GetKartCtrl()->CheckItem(kartId) && GetItemObjMgr()->IsAvailableRollingSlot(kartId, getKind())) {
            isStaffRoll = true;
            setItemSlot(kartId);
            JGeometry::TVec3f kartPos;
            ObjUtility::getKartPos(mKartIdx, &kartPos);
        }
    }

    setEffect(isStaffRoll);
    mMode = 3;
    _180 = 0;
    GeographyObj::clrAllCheckKartHitFlag();
}

u8 GeoItemBox::getInactiveTime() {
    return sUseInactiveTime;
}

namespace {
    f32 sF_MoveAcc[3] = {0.1f, 0.3f, 0.5f};
    f32 sF_MoveMaxVel[3] = {5.0f, 10.0f, 15.0f};
}

GeoF_ItemBox::GeoF_ItemBox(const CrsData::SObject &obj) : GeoItemBox(obj) {
    mMove = nullptr;
    switch (obj._28) {
    case 0: {
        mMove = new GeoObjPathMove(obj);
        break;
    }
    case 1: {
        mMove = new GeoObjPathMoveWaveY(obj);
        break;
    }
    }
}

void GeoF_ItemBox::reset() {
    GeoItemBox::reset();

    mMove->initPath(getGeoRnd(), &mPos, mObjData->mParam1);
    mPos.y += mObjData->mParam1;
    _20c = mObjData->mParam2;
}

void GeoF_ItemBox::calc() {
    doMode();
    mMove->movePath(&mPos, &mVel, sF_MoveAcc[_20c], sF_MoveMaxVel[_20c]);
    rotAnimation();
    GeographyObj::moveShadowModel();
}

GeoDoubleItemBox::GeoDoubleItemBox() {
    mKind = 0xc;
}

const char *GeoDoubleItemBox::getBmdFileName() {
    const char *ret = "/Item/itembox_double.bmd";
    if (RCMGetManager()->isTinyProcess()) {
        ret = "/Item/itemboxW_lan.bmd";
    }
    else if (GetGeoObjMgr()->isBombBattle()) {
        ret = "/Objects/MiniGameitemboxW_bomb.bmd";
    }
    else if (RCMGetManager()->getConsoleNumber() >= 2) {
        if (!GetGeoObjMgr()->isMiniGame()) {
            ret = "/Item/itembox_double_4p.bmd";
        }
    }
    return ret;
}

void GeoDoubleItemBox::createColModel(J3DModelData *) {
    GeographyObj::createBoundsSphere(170.0f, 1.0f);
}

GeoAnmTableEntry *GeoDoubleItemBox::getAnmTbl() {
    if (RCMGetManager()->isTinyProcess()) {
        return nullptr;
    }

    GeoAnmTableEntry *ret = &sDoubleMatAnmTable;
    if (RCMGetManager()->getConsoleNumber() >= 2) {
        if (!GetGeoObjMgr()->isMiniGame()) {
            ret = &sDoubleMatAnmTable4P;
        }
    }
    return ret;
}

u16 GeoDoubleItemBox::getSizeAnmTbl() {
    if (RCMGetManager()->isTinyProcess()) {
        return 0;
    }

    u32 ret = sizeof(sDoubleMatAnmTable);
    if (GetGeoObjMgr()->isBombBattle()) {
        ret = sizeof(sDoubleMatAnmTableMiniBomb);
    }
    else if (RCMGetManager()->getConsoleNumber() >= 2) {
        if (!GetGeoObjMgr()->isMiniGame()) {
            ret = sizeof(sDoubleMatAnmTable4P);
        }
    }
    return ret;
}
