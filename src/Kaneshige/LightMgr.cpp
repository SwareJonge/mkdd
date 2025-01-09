#include "Kaneshige/LightMgr.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/KartDrawer.h"
#include "Kaneshige/RaceLight.h"
#include "Kaneshige/RaceMgr.h"
#include "Yamamoto/kartCtrl.h"
#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXLighting.h"
#include "dolphin/gx/GXStruct.h"
#include "dolphin/mtx.h"
#include "types.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

LightMgr *LightMgr::sLightManager;

LightObj::LightObj(const char *name) : mLink(this) {
    init(name, 0xffffffff);
}

LightObj::LightObj(const char *name, u32 viewNo) : mLink(this) {
    init(name, viewNo);
}

LightObj::~LightObj() {
    if (LightMgr::getManager()) {
        LightMgr::getManager()->removeLight(this);
    }
}

void LightObj::init(const char *name, u32 viewNo) {
    mDrawFlags = 0;
    mViewNo = viewNo;
    mTagName = 0;
    _34 = 0;
}

void LtObjDiffuse::setGXObj() {
#line 242
    JUT_ASSERT(mViewMtx != 0);

    JGeometry::TVec3f lightPos;

    PSMTXMultVec(mViewMtx, &mPos, &lightPos);

    GXLightObj *lt_obj = &mLightObj;
    GXInitLightPos(lt_obj, lightPos.x, lightPos.y, lightPos.z);
    GXInitLightAttn(lt_obj, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    GXInitLightColor(lt_obj, mColor);
}

void LtObjDiffuse::draw() {
    GXLoadLightObjImm(&mLightObj, (GXLightID)(1 << mLoadNo));
}

void LtObjDiffuse::load(GXLightID light) {
    GXLoadLightObjImm(&mLightObj, light);
}

void LtObjAmbient::draw() {
    GXSetChanAmbColor(GX_COLOR0A0, mColor);
    GXSetChanAmbColor(GX_COLOR1A1, mColor);
}   

LightMgr::LightMgr() {
    mCurViewNo = 0;
    mLights.initiate();
    mParent = nullptr;
}

void LightMgr::createCharColor() {}

bool LightMgr::createManager() {
    if (sLightManager == nullptr) {
        sLightManager = new LightMgr();
        sLightManager->createCharColor();
    }
    return false;
}

void LightMgr::reset() {}

void LightMgr::appendLight(LightObj *obj) {
    mLights.append(&obj->mLink);
}

void LightMgr::removeLight(LightObj *obj) {
    mLights.remove(&obj->mLink);
}

void LightMgr::draw() {
    for (JSUListIterator<LightObj> it(mLights.getFirst()); it != mLights.getEnd(); it++) {
        if (it->tstManualDraw())
            continue;

        if (it->isAllView()) {
            it->draw();
        }
        else if (mCurViewNo == it->getViewNo()) {
            it->draw();
        }
    }
}

void LightMgr::calc() {
    for (JSUListIterator<LightObj> it(mLights.getFirst()); it != mLights.getEnd(); it++) {
        it->calc();
        it->setGXObj();
    }
}

LightObj *LightMgr::searchLight(u32 tagName) {
    LightObj *ret = nullptr;
    if (tagName != 0) {
        for (JSUListIterator<LightObj> it(mLights.getFirst()); it != mLights.getEnd(); it++) {
            if (tagName == it->getTagName()) {
                ret = it.getObject();
                break;
            } 
        }
    }

    return ret;
}

RaceSceneLight *LightMgr::searchRaceSceneLight(u32 viewNo) {
    // UNUSED, should call another unused function
    // getTagName__14RaceSceneLightFUl
}

RaceKartLight *LightMgr::searchRaceKartLight(u32 viewNo, int kartNo) {
    // UNUSED
    return RaceMgr::getManager()->getKartDrawer(kartNo)->getLight(viewNo);
}

LightMgr::~LightMgr() {
    for (JSUListIterator<LightObj> it(mLights.getFirst()); it != mLights.getEnd(); it++) {
        mLights.remove(&it->mLink);
    }
    if (mParent)
        delete mParent;
    sLightManager = nullptr;
}
