#include "Sato/GeographyObj.h"

#include "Kaneshige/Course/CrsGround.h"
#include "Kaneshige/ExModel.h"
#include "Sato/GeographyObjMgr.h"
#include "Sato/ObjCollision.h"
#include "Shiraiwa/SiUtil.h"

#include "Osako/shadowModel.h"

GeographyObj::GeographyObj(const CrsData::SObject &data) : mModel(1), mObjLink(this)
{
    mAnmCtrl = nullptr;
    mObjData = &data;
    mShadowMdl = nullptr;
    mKind = data.objID;
    mObjFlag = 0;
    mBounds = nullptr;
    mColObjPos = nullptr;
    mBoundsNum = 0;
    mDefaultBound = 0;
    mRotMtx.mMtx[0][3] = 0.0f;
    mRotMtx.mMtx[1][3] = 0.0f;
    mRotMtx.mMtx[2][3] = 0.0f;
    mSoundMgr = nullptr;
    mCheckKartHitFlags = 0;

    if (data.colFlag == 0) { setObjFlagNoCollision(); }

    resetObject();
    mReaction.init();
    setDefaultItemReaction();
    setObjFlagLODBias();
}

GeographyObj::GeographyObj(u32 kind) : mModel(1), mObjLink(this)
{
    mAnmCtrl = nullptr;
    mObjData = nullptr;
    mKind = kind;
    mShadowMdl = nullptr;
    mObjFlag = 0;
    mBounds = nullptr;
    mColObjPos = nullptr;
    mBoundsNum = 0;
    mDefaultBound = 0;
    mRotMtx.mMtx[0][3] = 0.0f;
    mRotMtx.mMtx[1][3] = 0.0f;
    mRotMtx.mMtx[2][3] = 0.0f;
    mSoundMgr = nullptr;
    mCheckKartHitFlags = 0;

    resetObject();
    mReaction.init();
    setDefaultItemReaction();
    setObjFlagLODBias();
}

void GeographyObj::setDefaultItemReaction()
{
    mReaction.setFlg(0, 1);
    mReaction.setFlg(1, 1);
    mReaction.setFlg(3, 1);
    mReaction.setFlg(4, 1);
    mReaction.setFlg(8, 1);
    mReaction.setFlg(6, 1);
    mReaction.setFlg(5, 1);
    mReaction.setFlg(7, 1);
    mReaction.setFlg(10, 1);

    mReaction.setFlg(2, 2);
    mReaction.setFlg(9, 2);
    mReaction.setFlg(11, 2);
}

void GeographyObj::calc_norm()
{
    calc();
    calcBoundsGlPos();
}

void GeographyObj::update_norm()
{
    if (mAnmCtrl) {
        mAnmCtrl->FrameProc();
        mAnmCtrl->SetAnimation();
    }
    update();
}

void GeographyObj::update()
{
    setModelMatrixAndScale();
    mModel.update(0);
}

void GeographyObj::viewCalc(u32 viewNo)
{
    mModel.viewCalc(viewNo);
}

void GeographyObj::setCurrentViewNo(u32 viewNo)
{
    mModel.setCurrentViewNo(viewNo);
}

void GeographyObj::loadShadowModel(J3DModelData *modelData, u32 param_2)
{
    if (!modelData) { return; }

    u8 viewCtrlModelFlags = 0xf;
    if (param_2 == 0xff) { viewCtrlModelFlags = 0x10; }

    mShadowMdl = new ShadowModel(viewCtrlModelFlags, getShadowKind(), -1);
    mShadowMdl->setModelData(modelData);
}

void GeographyObj::setModelMatrixAndScale()
{
    mRotMtx.mMtx[0][3] = mPos.x;
    mRotMtx.mMtx[1][3] = mPos.y;
    mRotMtx.mMtx[2][3] = mPos.z;
    mModel.setBaseTRMtx(mRotMtx);
    mModel.setBaseScale(mScale);
}

void GeographyObj::getAssignedModelXYZdir(JGeometry::TVec3f *xDir, JGeometry::TVec3f *yDir, JGeometry::TVec3f *zDir)
{
    if (mObjData) {
        zDir->set(0.0001f * mObjData->zNorm.x, 0.0001f * mObjData->zNorm.y, 0.0001f * mObjData->zNorm.z);
        yDir->set(0.0001f * mObjData->yNorm.x, 0.0001f * mObjData->yNorm.y, 0.0001f * mObjData->yNorm.z);
    } else {
        zDir->set(0.0f, 0.0f, 1.0f);
        yDir->set(0.0f, 1.0f, 0.0f);
    }
    xDir->cross(*yDir, *zDir);
}

// unused but size matches and this is most likely what it did, TU matches fine without this function tho
void GeographyObj::getAssignedModelZdir(JGeometry::TVec3f *zDir)
{
    if (mObjData) {
        zDir->set(0.0001f * mObjData->zNorm.x, 0.0001f * mObjData->zNorm.y, 0.0001f * mObjData->zNorm.z);
    } else {
        zDir->set(0.0f, 0.0f, 1.0f);
    }
}

void GeographyObj::getAssignedModelMatrix(Mtx out_mtx)
{
    JGeometry::TVec3f xDir, yDir, zDir;
    getAssignedModelXYZdir(&xDir, &yDir, &zDir);

    f32 invMag_z = 1.0f / zDir.length();
    out_mtx[0][2] = invMag_z * zDir.x * mScale.z;
    out_mtx[1][2] = invMag_z * zDir.y * mScale.z;
    out_mtx[2][2] = invMag_z * zDir.z * mScale.z;

    f32 invMag_x = 1.0f / xDir.length();
    out_mtx[0][0] = invMag_x * xDir.x * mScale.x;
    out_mtx[1][0] = invMag_x * xDir.y * mScale.x;
    out_mtx[2][0] = invMag_x * xDir.z * mScale.x;

    f32 invMag_y = 1.0f / yDir.length();
    out_mtx[0][1] = invMag_y * yDir.x * mScale.y;
    out_mtx[1][1] = invMag_y * yDir.y * mScale.y;
    out_mtx[2][1] = invMag_y * yDir.z * mScale.y;

    JGeometry::TVec3f position;
    const CrsData::SObject *pObjData = mObjData;
    if (!pObjData) {
        position = mPos;
    } else {
        position = pObjData->position;
    }

    out_mtx[0][3] = position.x;
    out_mtx[1][3] = position.y;
    out_mtx[2][3] = position.z;
}

void GeographyObj::resetObject()
{
    if (mObjData) {
        mScale = mObjData->scale;
        mPos = mObjData->position;
    } else {
        mScale.set(1.0f, 1.0f, 1.0f);
        mPos.zero();
    }

    JGeometry::TVec3f xDir, yDir, zDir;
    getAssignedModelXYZdir(&xDir, &yDir, &zDir);

    mRotMtx.mMtx[0][0] = xDir.x;
    mRotMtx.mMtx[1][0] = xDir.y;
    mRotMtx.mMtx[2][0] = xDir.z;

    mRotMtx.mMtx[0][1] = yDir.x;
    mRotMtx.mMtx[1][1] = yDir.y;
    mRotMtx.mMtx[2][1] = yDir.z;

    mRotMtx.mMtx[0][2] = zDir.x;
    mRotMtx.mMtx[1][2] = zDir.y;
    mRotMtx.mMtx[2][2] = zDir.z;

    mVel.zero();

    _58 = 0;
    mGeoObjFlag = 0;
    mIsHitKartSoundFlg = 0;
    mIsHitKartSoundPlayedFlg = 0;
    mUserFlg1 = 0;
    mIsHitKartFlags = 0;
    mGeoObjFlag |= 2;
    setAllCheckKartHitFlag();

    if (GeographyObjManager::gMgr != nullptr) { SiUtil::setItemReaction(GeographyObjManager::gMgr->getGeoHitTable(), this); }
}

bool GeographyObj::IsHitKart(const JGeometry::TVec3f &spherePos, f32 sphereRadius, JGeometry::TVec3f *posAdjust)
{
    bool isHit = false;
    ObjColBase **pBounds = mBounds;
    for (u8 i = 0; i < mBoundsNum; i++, pBounds++) {
        if ((*pBounds)->IsHitSphere(getBoundsGlPos(i), spherePos, sphereRadius)) {
            if (posAdjust && (_58 != 2)) {
                JGeometry::TVec3f collPos = (*pBounds)->getPos();
                f32 distance = (*pBounds)->getBoundDepth();
                collPos *= distance;
                *posAdjust += collPos;
            }
            isHit = true;
        }
    }
    return isHit;
}

bool GeographyObj::IsHitObject(const JGeometry::TVec3f &hitCheckPos, ObjColBase *collider)
{
    bool isHit = false;
    ObjColBase::CKind colliderKind = collider->getKind();
    switch (collider->getKind()) {
    case ObjColBase::SPHERE:
        isHit = (*mBounds)->IsHitSphere(mPos, hitCheckPos, collider->getRadius());
        break;
    case ObjColBase::CYLINDER:
        isHit = (*mBounds)->IsHitCylinder(mPos, hitCheckPos, *(ObjColCylinder *)collider);
        break;
    default:
        break;
    }
    return isHit;
}

void GeographyObj::setIsHitKartFlg(int kart_index)
{
    if ((mIsHitKartSoundPlayedFlg & (1 << kart_index)) == 0) { // If sound not already played
        if ((mIsHitKartSoundFlg & (1 << kart_index)) == 0) {   // And not yet triggered
            mIsHitKartSoundFlg |= (1 << kart_index);           // Trigger kart hit sound
        } else {                                               // Otherwise (previously triggerd)
            mIsHitKartSoundPlayedFlg |= (1 << kart_index);     // Mark as played
            mIsHitKartSoundFlg &= ~(1 << kart_index);          // Clear trigger bit
        }
    }

    mIsHitKartFlags |= (1 << kart_index);
}

void GeographyObj::clrIsHitKartSoundFlg(int kart_index)
{
    mIsHitKartSoundFlg &= ~(1 << kart_index);
    mIsHitKartSoundPlayedFlg &= ~(1 << kart_index);
}

bool GeographyObj::tstUserFlg1(int kart_index) const
{
#line 470
    JUT_MINMAX_ASSERT(0, kart_index, 8);
    return (mUserFlg1 & (1 << kart_index)) != 0;
}

void GeographyObj::clrUserFlg1(int kart_index)
{
#line 479
    JUT_MINMAX_ASSERT(0, kart_index, 8);
    mUserFlg1 &= ~(1 << kart_index);
    return;
}

void GeographyObj::setUserFlg1(int kart_index)
{
#line 488
    JUT_MINMAX_ASSERT(0, kart_index, 8);
    mUserFlg1 |= (1 << kart_index);
}

f32 GeographyObj::getHitSoundDegree()
{
    f32 degree = 0.1f * getAllBoundDepth();
    if (degree > 1.0f) { degree = 1.0f; }
    return degree;
}

u32 GeographyObj::getSoundID() const
{
    return 0x10007;
}

void GeographyObj::createMultiBoundsPtrAll(u8 param_1)
{
    if (mBoundsNum != 0) return;

    mBoundsNum = param_1;
    mBounds = new ObjColBase *[mBoundsNum];
    mColObjPos = new JGeometry::TVec3f[mBoundsNum];
    mBoundsGlPos = new JGeometry::TVec3f[mBoundsNum];
    for (u8 i = 0; i < mBoundsNum; i++) {
        mColObjPos[i].zero();
        mBoundsGlPos[i].zero();
        mBounds[i] = nullptr;
    }
    mObjFlag |= 0x40;
}

// unused / inlined into createBounds*, size match
void GeographyObj::initSingleBound()
{
    mBounds = new ObjColBase *;
    mColObjPos = new JGeometry::TVec3f(0.0f, 0.0f, 0.0f);
    mBoundsGlPos = new JGeometry::TVec3f(0.0f, 0.0f, 0.0f);
    mBoundsNum++;
}

void GeographyObj::calcBoundsGlPos()
{
    if (!mBounds) return;

    JGeometry::TVec3f *pColObjPos = mColObjPos;
    Mtx srtMtx;
    getSRTMtx(srtMtx);
    JGeometry::TVec3f *pBoundsGlPos = mBoundsGlPos;
    for (u8 i = 0; i < mBoundsNum; i++, pColObjPos++, pBoundsGlPos++) {
        PSMTXMultVec(srtMtx, pColObjPos, pBoundsGlPos);
    }
}

void GeographyObj::createBoundsSphere(J3DModelData *modelData)
{
    if (mBoundsNum != 0) return;

    initSingleBound();
    *mBounds = new ObjColSphere(modelData, mScale);
}

void GeographyObj::createBoundsSphere(f32 radius, f32 scale)
{
    if (mBoundsNum != 0) return;

    initSingleBound();
    *mBounds = new ObjColSphere(radius, scale);
}

bool GeographyObj::createMultiBoundsSphere(u8 index, f32 radius, f32 scale)
{
    bool created = false;
    if (mBounds[index] == nullptr) {
        mBounds[index] = new ObjColSphere(radius, scale);
        created = true;
    }
    return created;
}

void GeographyObj::createBoundsCylinder(J3DModelData *modelData, f32 radius, f32 height)
{
    if (mBoundsNum != 0) return;

    initSingleBound();

    ObjColCylinder *cylinder = new ObjColCylinder(modelData, mScale);
    cylinder->setCylinderRadius(radius);
    cylinder->setCylinderHeight(height);

    *mBounds = cylinder;
}

void GeographyObj::createBoundsCylinder(f32 radius, f32 height, f32 scale)
{
    if (mBoundsNum != 0) return;

    initSingleBound();

    ObjColCylinder *cylinder = new ObjColCylinder(radius, scale);
    cylinder->setCylinderRadius(radius);
    cylinder->setCylinderHeight(height);

    *mBounds = cylinder;
}

bool GeographyObj::createMultiBoundsCylinder(const u8 index, J3DModelData *modelData, f32 radius, f32 height)
{
    bool created = false;
    if (mBounds[index] == nullptr) {
        ObjColCylinder *cylinder = new ObjColCylinder(modelData, mScale);
        cylinder->setCylinderRadius(radius);
        cylinder->setCylinderHeight(height);
        mBounds[index] = cylinder;
        created = true;
    }
    return created;
}

void GeographyObj::createBoundsCube(J3DModelData *modelData)
{
    if (mBoundsNum != 0) return;

    initSingleBound();

    Mtx assignedModelMatrix;
    getAssignedModelMatrix(assignedModelMatrix);

    ObjColCube *cube = new ObjColCube(modelData, mScale);
    cube->makeParameter(modelData, assignedModelMatrix);
    *mBounds = cube;
}

bool GeographyObj::setColObjPos(const JGeometry::TVec3f &pos, u8 index)
{
    bool indexInRange = false;
    if (index < mBoundsNum) {
        indexInRange = true;
        mColObjPos[index] = pos;
    }
    return indexInRange;
}

void GeographyObj::createColModel(J3DModelData *) {}

void GeographyObj::createShadowModel(JKRSolidHeap *heap, u32 param_2)
{
    if (!mShadowMdl) { return; }

    mShadowMdl->createModel(heap, param_2, 0x60000);
    CrsGround ground(RaceMgr::sRaceManager->getCourse());
    ground.search(this, mPos, mPos);

    Mtx assignedModelMatrix;
    getAssignedModelMatrix(assignedModelMatrix);

    assignedModelMatrix[1][3] = ground.getHeight();

    mShadowMdl->setBaseTRMtx(assignedModelMatrix);
    mShadowMdl->getModel()->setBaseScale(mScale);
}

stRandom *GeographyObj::getGeoRnd()
{
    return stGetRnd(1);
}

void GeographyObj::moveShadowModel()
{
    if (mShadowMdl->isInvisibleAll()) { return; }

    CrsGround ground(RaceMgr::sRaceManager->getCourse());
    ground.search(this, mPos, mPos);

    Mtx trMtx;
    PSMTXIdentity(trMtx);

    trMtx[0][3] = mPos.x;
    trMtx[1][3] = ground.getHeight();
    trMtx[2][3] = mPos.z;

    mShadowMdl->setBaseTRMtx(trMtx);
    mShadowMdl->getModel()->setBaseScale(mScale);
}

void GeographyObj::doKartColCallBack(int) {}

void GeographyObj::createSoundMgr()
{
    if (mSoundMgr) { return; }

    mSoundMgr = new GameAudio::ObjectSoundMgr(&mPos, nullptr);
}

f32 GeographyObj::getColRadius()
{
    f32 fVar1 = 0.0f;
    if (mBounds != nullptr) {
        ObjColBase *bound = mBounds[mDefaultBound];

        switch (bound->getKind()) {
        case ObjColBase::CYLINDER:
            fVar1 = ((ObjColCylinder *)bound)->getCylinderRadius();
            break;
        case ObjColBase::SPHERE:
            fVar1 = bound->getRadius();
            break;
        default:
            break;
        }
    }
    return fVar1;
}

f32 GeographyObj::getColScaleRadius()
{
    f32 radius = getColRadius();

    if (mBounds && radius > 0.0f) { radius *= mBounds[mDefaultBound]->getScale(); }
    return radius;
}

f32 GeographyObj::getAxisMaxScale()
{
    f32 maxScale = mScale.x;
    if (mScale.y > maxScale) { maxScale = mScale.y; }
    if (mScale.z > maxScale) { maxScale = mScale.z; }
    return maxScale;
}

void GeographyObj::simpleDraw(u32 viewNo)
{
    if (!tstObjFlagSimpleDraw()) { return; }

    LightObj *light = LightMgr::getManager()->searchLight(0x53434e30 + viewNo); // SCN0
    MtxPtr effectMtx = light->getEffectMtx();

    mModel.setScale(mScale);
    mModel.simpleDraw(viewNo, effectMtx, 1);
}

void GeographyObj::lockMdl()
{
    if (!tstObjFlagSimpleDraw()) { mModel.lock(); }
}

void GeographyObj::getSRTMtx(Mtx mtxOut)
{
    Mtx mtx;

    PSMTXScale(mtx, mScale.x, mScale.y, mScale.z);
    PSMTXConcat(mRotMtx, mtx, mtxOut);

    mtxOut[0][3] = mPos.x;
    mtxOut[1][3] = mPos.y;
    mtxOut[2][3] = mPos.z;
}

void GeographyObj::setBoundScaleAll()
{
    if (!mBounds) return;

    for (u8 i = 0; i < mBoundsNum; i++) {
        mBounds[i]->setScale(mScale);
    }
}

f32 GeographyObj::getAllBoundDepth()
{
    f32 totalDepth = 0.0f;
    ObjColBase **bound = mBounds;
    for (u8 i = 0; i < mBoundsNum; i++, bound++) {
        totalDepth += (*bound)->getBoundDepth();
    }
    return totalDepth;
}

void GeographyObj::getItemThrowDirPow(JGeometry::TVec3f *, f32 *, const ItemObj &) {}

void GeographyObj::getKartThrowDirPow(JGeometry::TVec3f *, f32 *, int) {}

void GeographyObj::setObjData(const CrsData::SObject &objData, bool doReset)
{
    mObjData = &objData;
    if (doReset) { reset(); }
}

void ItemColReaction::init()
{
    for (u8 i = 0; i < 0x10; i++) {
        mFlags[i] = 0;
    }
}

void ItemColReaction::setFlg(u32 flgId, u8 flgVal)
{
    u8 flgByte = flgId >> 1;
    u8 flgNibble = flgId & 1;
    u32 flgMask = 0xf0 >> (flgNibble * 4);
    u32 flgBits = (flgVal & 0x0f) << ((1 - flgNibble) * 4);

    mFlags[flgByte] = (mFlags[flgByte] & ~flgMask) | flgBits;
}

u8 ItemColReaction::tstFlg(u32 flgId)
{
    u32 flgByte = flgId >> 1;
    u8 flgNibble = flgId & 1;
    u32 shiftAmt = (1 - flgNibble) * 4;
    return (mFlags[flgByte] >> shiftAmt) & 0xf;
}

void GeoObjSupervisor::createModel(JKRSolidHeap *, unsigned long) {}

#include "JSystem/JAudio/JASFakeMatch2.h"
