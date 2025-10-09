#include "Kaneshige/RaceLight.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/TColor.h"
#include "Kaneshige/Course/CrsArea.h"
#include "Kaneshige/KartInfo.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/LightMgr.h"
#include "Sato/ObjUtility.h"
#include "Yamamoto/kartBody.h"
#include "Yamamoto/kartCtrl.h"
#include "dolphin/mtx.h"

#include "mathHelper.h"
#include "JSystem/JAudio/JASFakeMatch2.h"

MenuSceneLight *MenuSceneLight::sSceneLight;
MenuAmbient *MenuAmbient::sAmbient;
MenuSubLight *MenuSubLight::sSubLight[2] = {
        nullptr, nullptr
};

RaceSceneLight::RaceSceneLight(const char *name, u32 viewNo, const JUtility::TColor &color, const JGeometry::TVec3f &pos) : LtObjDiffuse(name, viewNo) {
    mColor.set(color);
    mTargetCam = RCMGetCamera(viewNo);
    mOffsetPos.set(pos);
    mFrDir.set(0.0f, 0.0f, 1.0f);
    mUpDir.set(0.0f, 1.0f, 0.0f);
    mLfDir.set(1.0f, 0.0f, 0.0f);
    PSMTXIdentity(mEffectMtx);
    setLoadNo(0);
    setTagName(0x53434e30 + viewNo); // SCN0
}

void RaceSceneLight::calc() {
    JGeometry::TVec3f *targetPos = mTargetCam->GetCameraLookPos();
    JGeometry::TVec3f *camPos = mTargetCam->GetCameraPos();
    
    JGeometry::TVec3f dist;
    dist.sub(*targetPos, *camPos);
    dist.normalize();
    mFrDir.set(dist);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();
    mFrDir.cross(mLfDir, mUpDir);
    mFrDir.normalize();

    mPos.x = (camPos->x - mOffsetPos.x * mLfDir.x) + (mOffsetPos.y * mUpDir.x) - (mOffsetPos.z * mFrDir.x);
    mPos.y = (camPos->y - mOffsetPos.x * mLfDir.y) + (mOffsetPos.y * mUpDir.y) - (mOffsetPos.z * mFrDir.y);
    mPos.z = (camPos->z - mOffsetPos.x * mLfDir.z) + (mOffsetPos.y * mUpDir.z) - (mOffsetPos.z * mFrDir.z);

    JGeometry::TVec3f targetDiff;
    targetDiff.sub(*targetPos, mPos);
    targetDiff.normalize();

    JGeometry::TVec3f target;
    target.add(targetDiff, dist);
    target.normalize();

    JGeometry::TVec3f up(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
    C_MTXLookAt(mEffectMtx, &pos, &up, &target);
    mViewMtx = mTargetCam->GetMtx();  
}

RaceKartLight::RaceKartLight(RaceSceneLight *sceneLight, int kartIdx) : LtObjDiffuse("カートライト", sceneLight->getViewNo()), mAmbient("カートアンビエント", TCOLOR_BLACK) {
    KartInfo *kartInfo = RaceMgr::getManager()->getKartInfo(kartIdx);
    mSceneLight = sceneLight;
    mViewMtx = mSceneLight->getTargetCamera()->GetMtx();
    mSceneLight->getColor(&mColor);
    mKartIdx = kartIdx;
    mOffsetPos.set(-20000.0f, 50000.0f, 30000.0f);
    PSMTXIdentity(mEffectMtx);
    _110 = false;
    mLightColor.set(mColor);
    mAmbient.getColor(&mAmbientColor);
    mLightOffsetPos.set(mOffsetPos);

    setLoadNo(1);
    setTagName(0x4b543030 + (kartIdx << 4) + sceneLight->getViewNo()); // KT00
    setManualDraw();
}

void RaceKartLight::calc() {
    JGeometry::TVec3f kartPos;
    ObjUtility::getKartPos(mKartIdx, &kartPos);

    CrsArea areaLight;
    areaLight.searchLight(kartPos);

    CrsArea *pAreaLight = &areaLight;
    if (!areaLight.isInside() ) {
        CrsArea *shadowArea = &KartCtrl::getKartCtrl()->getKartBody(mKartIdx)->mShadowArea;
        if (shadowArea->isInside()) {
            pAreaLight = shadowArea;
        }        
    }
    f32 rate = pAreaLight->getRate();
    JUtility::TColor ambientColor(0, 0, 0, 0);

    if (rate <= 0.0f) {
        mSceneLight->getColor(&mColor);
        mSceneLight->getOffsetPosition(&mOffsetPos);
        LightMgr::getManager()->searchLight(0x414d4249)->getColor(&ambientColor); // AMBI        
    }
    else {
        CrsData::LightParam *lightParam = RCMGetCourse()->getCrsData()->getLightParam(pAreaLight->getLightParamID());
#line 358
        JUT_ASSERT_F(lightParam, "NO LINK LIGHT PARAM:%d", pAreaLight->getLightParamID());
        if (_110) {
            rate = 1.0f;
        }
        
        JUtility::TColor color;
        mSceneLight->getColor(&color);

        JGeometry::TVec3f offsetPos;
        mSceneLight->getOffsetPosition(&offsetPos);

        JUtility::TColor lightAmbient;
        LightMgr::getManager()->searchLight(0x414d4249)->getColor(&lightAmbient); // AMBI

        if (!_110) {
            lightParam->getLightColor(&mLightColor);
        }

        mColor.r = (rate * ((f32)mLightColor.r - color.r)) + color.r;
        mColor.g = (rate * ((f32)mLightColor.g - color.g)) + color.g;
        mColor.b = (rate * ((f32)mLightColor.b - color.b)) + color.b;
    
        if (!_110) {
            lightParam->getLightOffsetPosition(&mLightOffsetPos);
        }

        JGeometry::TVec3f dist;
        dist.sub(mLightOffsetPos, offsetPos);
        mOffsetPos.scaleAdd(rate, dist, offsetPos);

        if (!_110) {
            lightParam->getAmbientColor(&mAmbientColor);
        }

        ambientColor.r = (rate * ((f32)mAmbientColor.r - lightAmbient.r)) + lightAmbient.r;
        ambientColor.g = (rate * ((f32)mAmbientColor.g - lightAmbient.g)) + lightAmbient.g;
        ambientColor.b = (rate * ((f32)mAmbientColor.b - lightAmbient.b)) + lightAmbient.b;    
    }


    mAmbient.setColor(ambientColor);
    JGeometry::TVec3f *targetPos = mSceneLight->getTargetCamera()->GetCameraLookPos();
    JGeometry::TVec3f *camPos = mSceneLight->getTargetCamera()->GetCameraPos();
    
    JGeometry::TVec3f frDir;
    mSceneLight->getFrDirection(&frDir);
    JGeometry::TVec3f upDir;
    mSceneLight->getUpDirection(&upDir);
    JGeometry::TVec3f lfDir;
    mSceneLight->getLfDirection(&lfDir);

    mPos.x = (camPos->x - mOffsetPos.x * lfDir.x) + (mOffsetPos.y * upDir.x) - (mOffsetPos.z * frDir.x);
    mPos.y = (camPos->y - mOffsetPos.x * lfDir.y) + (mOffsetPos.y * upDir.y) - (mOffsetPos.z * frDir.y);
    mPos.z = (camPos->z - mOffsetPos.x * lfDir.z) + (mOffsetPos.y * upDir.z) - (mOffsetPos.z * frDir.z);

    JGeometry::TVec3f targetDiff;
    targetDiff.sub(*targetPos, mPos);
    targetDiff.normalize();

    JGeometry::TVec3f dist;
    dist.sub(*targetPos, *camPos);
    dist.normalize();

    JGeometry::TVec3f target;
    target.add(targetDiff, dist);
    target.normalize();

    JGeometry::TVec3f up(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
    C_MTXLookAt(mEffectMtx, &pos, &up, &target);
}

void RaceKartLight::draw() {
        LtObjDiffuse::draw();
    mAmbient.draw();
}

RaceBalloonLight::RaceBalloonLight(u32 viewNo) : LtObjDiffuse("ふうせん", viewNo) {
    mColor.set(0xff, 0xff, 0xff, 0xff);
    mTargetCam = RCMGetCamera(viewNo);
    mOffsetPos.set(0.0f, 0.0f, 0.0f);
    mFrDir.set(0.0f, 0.0f, 1.0f);
    mUpDir.set(0.0f, 1.0f, 0.0f);
    mLfDir.set(1.0f, 0.0f, 0.0f);
    
    setLoadNo(7);
    setTagName(0x424c4e30 + viewNo); // BLN0
}

void RaceBalloonLight::calc() {
    JGeometry::TVec3f *targetPos = mTargetCam->GetCameraLookPos();
    JGeometry::TVec3f *camPos = mTargetCam->GetCameraPos();
    
    JGeometry::TVec3f dist;
    dist.sub(*targetPos, *camPos);
    dist.normalize();
    mFrDir.set(dist);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();
    mFrDir.cross(mLfDir, mUpDir);
    mFrDir.normalize();

    mPos.x = (camPos->x - mOffsetPos.x * mLfDir.x) + (mOffsetPos.y * mUpDir.x) - (mOffsetPos.z * mFrDir.x);
    mPos.y = (camPos->y - mOffsetPos.x * mLfDir.y) + (mOffsetPos.y * mUpDir.y) - (mOffsetPos.z * mFrDir.y);
    mPos.z = (camPos->z - mOffsetPos.x * mLfDir.z) + (mOffsetPos.y * mUpDir.z) - (mOffsetPos.z * mFrDir.z);

    JGeometry::TVec3f targetDiff;
    targetDiff.sub(*targetPos, mPos);
    targetDiff.normalize();

    JGeometry::TVec3f target;
    target.add(targetDiff, dist);
    target.normalize();

    JGeometry::TVec3f up(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
    C_MTXLookAt(mEffectMtx, &pos, &up, &target);
    mViewMtx = mTargetCam->GetMtx();
}

RaceCupLight::RaceCupLight(RaceSceneLight *sceneLight) : LtObjDiffuse("カップライト", sceneLight->getViewNo()), mAmbient("カップアンビエント", JUtility::TColor(0x8c, 0x8c, 0x8c, 0)) {
    setLoadNo(7);
    setTagName(0x43555030 + sceneLight->getViewNo()); // CUP0
    setManualDraw();
    mColor.set(0xc8, 0xc8, 0xc8, 0xff);
    mSceneLight = sceneLight;
}

void RaceCupLight::calc() {
    mViewMtx = mSceneLight->getViewMtx();
    mSceneLight->getPosition(&mPos);
}

void RaceCupLight::draw() {
    LtObjDiffuse::draw();
    mAmbient.draw();
}

MenuSceneLight::MenuSceneLight(Mtx m) : LtObjDiffuse("前", 0) {
    mViewMtx = m;
    mColor.set(0xff, 0xff, 0xff, 0xff);

    mOffsetPos.set(-500.0f, 500.0f, -2000.0f);
    mCameraPos.set(0.0f, 0.0f, 100.0f);
    mTargetPos.set(0.0f, 0.0f, 0.0f);
    mFrDir.set(0.0f, 0.0f, 1.0f);
    mUpDir.set(0.0f, 1.0f, 0.0f);
    mLfDir.set(1.0f, 0.0f, 0.0f);
    PSMTXIdentity(mEffectMtx);

#line 656
    JUT_ASSERT_MSG(sSceneLight == nullptr, "SCENE LIGHT ONLY ONE"); // MenuSceneLight should only be called once 
    sSceneLight = this;
}

MenuSceneLight *MenuSceneLight::createAndAppend(JKRHeap *heap, Mtx m) {
    MenuSceneLight *sceneLight = sSceneLight;
    if (sSceneLight == nullptr) {
        sceneLight = new (heap, 0) MenuSceneLight(m);
        LightMgr::getManager()->appendLight(sceneLight);
    }
    return sceneLight;
}

void MenuSceneLight::calc() {
    JGeometry::TVec3f *targetPos = &mTargetPos;
    JGeometry::TVec3f *camPos = &mCameraPos;

    JGeometry::TVec3f dist;
    dist.sub(*targetPos, *camPos);
    dist.normalize();
    mFrDir.set(dist);

    mLfDir.cross(mUpDir, mFrDir);
    mLfDir.normalize();
    mFrDir.cross(mLfDir, mUpDir);
    mFrDir.normalize();

    mPos.x = (camPos->x - mOffsetPos.x * mLfDir.x) + (mOffsetPos.y * mUpDir.x) - (mOffsetPos.z * mFrDir.x);
    mPos.y = (camPos->y - mOffsetPos.x * mLfDir.y) + (mOffsetPos.y * mUpDir.y) - (mOffsetPos.z * mFrDir.y);
    mPos.z = (camPos->z - mOffsetPos.x * mLfDir.z) + (mOffsetPos.y * mUpDir.z) - (mOffsetPos.z * mFrDir.z);

    JGeometry::TVec3f targetDiff;
    targetDiff.sub(*targetPos, mPos);
    targetDiff.normalize();

    JGeometry::TVec3f target;
    target.add(targetDiff, dist);
    target.normalize();

    JGeometry::TVec3f up(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
    C_MTXLookAt(mEffectMtx, &pos, &up, &target);
}

MenuSubLight *MenuSubLight::createAndAppend(JKRHeap *heap, int id, MenuSceneLight *sceneLight, u8 loadNo) {
#line 769
    JUT_MINMAX_ASSERT(0, id, 2);
    MenuSubLight *sub = sSubLight[id];

    if (sub == nullptr) {
        sub = new (heap, 0) MenuSubLight(id, sceneLight, loadNo);
        LightMgr::getManager()->appendLight(sub);
    }

    return sub;
}

MenuSubLight::MenuSubLight(int id, MenuSceneLight *scenLight, u8 loadNo) : LtObjDiffuse("サブ", 0) {
#line 792
    JUT_MINMAX_ASSERT(0, id, 2);
    mId = id;
    mSceneLight = scenLight;
    mViewMtx = mSceneLight->getViewMtx();
    mSceneLight->getColor(&mColor);
    mOffsetPos.zero();
    PSMTXIdentity(mEffectMtx);

#line 803
    JUT_ASSERT(loadNo <= 2);
    setLoadNo(loadNo);

    JUT_MINMAX_ASSERT(0, id, 2); // clap
    sSubLight[id] = this;
}

MenuSubLight::~MenuSubLight() {
    sSubLight[mId] = nullptr;
}

void MenuSubLight::calc() {
    mSceneLight->getColor(&mColor);
    JGeometry::TVec3f targetPos;
    mSceneLight->getTargetPos(&targetPos);
    
    JGeometry::TVec3f camPos;
    mSceneLight->getCameraPos(&camPos);

    JGeometry::TVec3f dist;
    dist.sub(targetPos, camPos);
    dist.normalize();

    JGeometry::TVec3f upDir(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f frDir(dist);
    JGeometry::TVec3f lfDir;
    lfDir.cross(upDir, frDir);
    lfDir.normalize();
    frDir.cross(lfDir, upDir);
    frDir.normalize();

    mPos.x = (camPos.x - mOffsetPos.x * lfDir.x) + (mOffsetPos.y * upDir.x) - (mOffsetPos.z * frDir.x);
    mPos.y = (camPos.y - mOffsetPos.x * lfDir.y) + (mOffsetPos.y * upDir.y) - (mOffsetPos.z * frDir.y);
    mPos.z = (camPos.z - mOffsetPos.x * lfDir.z) + (mOffsetPos.y * upDir.z) - (mOffsetPos.z * frDir.z);

    JGeometry::TVec3f targetDiff;
    targetDiff.sub(targetPos, mPos);
    targetDiff.normalize();

    JGeometry::TVec3f target;
    target.add(targetDiff, dist);
    target.normalize();

    JGeometry::TVec3f up(0.0f, 1.0f, 0.0f);
    JGeometry::TVec3f pos(0.0f, 0.0f, 0.0f);
    C_MTXLookAt(mEffectMtx, &pos, &up, &target);
}

MenuAmbient *MenuAmbient::createAndAppend(JKRHeap *heap, JUtility::TColor color) {
    MenuAmbient *amb = sAmbient;
    if (amb == nullptr) {
        amb = new (heap, 0) MenuAmbient(color);
        LightMgr::getManager()->appendLight(amb);
    }
    return amb;
}

MenuAmbient::MenuAmbient(JUtility::TColor color) : LtObjAmbient(color) {
    sAmbient = this;
}

MenuAmbient::~MenuAmbient() {
    sAmbient = nullptr;
}
