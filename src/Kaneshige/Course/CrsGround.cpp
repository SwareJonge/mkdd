#include "Kaneshige/Course/CrsGround.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kaneshige/JugemPoint.h"
#include "Kaneshige/RaceMgr.h"
#include "Kaneshige/Objects/GeoCannon.h"
#include "Kaneshige/Objects/GeoPuller.h"
#include "Kaneshige/Objects/GeoSplash.h"
#include "Kaneshige/Objects/GeoWater.h"
#include "Shiraiwa/MapObjPool.h"

#include "mathHelper.h"

#include "JSystem/JAudio//JASFakeMatch2.h"
#include "types.h"

CrsGround::CrsGround(Course *course) {
    mCourse = RaceMgr::getManager()->getCourse();
    reset();
}

void CrsGround::reset() {
    mWorldPos.zero();
    _10.zero();
    mExceptVally = false;
    _1d = 0;

    mGround = nullptr;
    mGeoObj = nullptr;
    mNormal.set(0.0f, 1.0f, 0.0f);
    mVelocity.zero();
    mWallNormal.set(0.0f, 1.0f, 0.0f);

    if (mCourse->getCrsData()->isAvailableCol()) {
        mAttribute = Attr_255;
        mHeight = 0.0f;
    }
    else {
        mAttribute = Attr_1;
        mHeight = 1000.0f;
    }

    mAttrIdx = 0;
    mMaterial = Mat_255;
    mDepth = -1.0f;
}

void CrsGround::search(const JGeometry::TVec3f &pos) {
    searchBase(nullptr, pos, pos, 3);
}

void CrsGround::search(const JGeometry::TVec3f &pos1, const JGeometry::TVec3f &pos2) {
    searchBase(nullptr, pos1, pos2, 3);
}

void CrsGround::search(GeographyObj *geoObj, const JGeometry::TVec3f &pos1, const JGeometry::TVec3f &pos2) {
    searchBase(geoObj, pos1, pos2, 3);
}

void CrsGround::searchBase(GeographyObj *, const JGeometry::TVec3f &, const JGeometry::TVec3f &, unsigned long) {}

void CrsGround::search(const JGeometry::TVec3f &, const JGeometry::TVec3f &, float, bool) {}

void CrsGround::roundDepth(const JGeometry::TVec3f &vec) {
    JGeometry::TVec3f v(vec);
    v.normalize();
    JGeometry::TVec3f norm(mWallNormal);
    norm.normalize();
    if (v.dot(norm) > -0.2f)
        mDepth = 0.1f;
}

void CrsGround::checkGridBySphere(float *, const CrsData::SColInfoSphere &, unsigned long, float, float, float) {}

bool CrsGround::checkPolygonCover(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, float, const CrsData::Ground *, float *) {}

void CrsGround::checkPolygonCoverMain(const JGeometry::TVec3f &, const CrsData::SColInfoSphere &, unsigned long, float, float, float, const CrsData::Ground *) {}

void CrsGround::isInsideGrid(const JGeometry::TVec3f &, float, float, float, float) {}

void CrsGround::searchGridIndex(int *, int *, Course *, const JGeometry::TVec3f &, float, float, float, float, int, int) {}

f32 CrsGround::getHeight() const { return mHeight; }

void CrsGround::getNormal(JGeometry::TVec3f *out) const { out->set(mNormal); }

u8 CrsGround::getCameraCode() const {
    u8 ret = 0;
    if (!isObject()) {
        ret = mGround ? mGround->camCode : ret;
    }
    return ret;    
}

bool CrsGround::isItemInvalGround() const {
    bool ret = false;
    if (!isObject()) {
        ret = (mGround) ? mGround->itemType != 0 : false;
    }
    return ret;
}

u8 CrsGround::getAddThickness() const {
    u8 ret = 0;
    if (!isObject()) {
        ret = mGround ? mGround->getAddThickness() : ret;
    }
    return ret;
}

u8 CrsGround::getStaggerCode() const {
    u8 ret = 0;
    if (!isObject()) {
        if (mGround) {
            ret = (u8)mGround->staggerCode;
        }
        else if (mCourse->getCrsData()->isAvailableCol()) {
            ret = 1;
        }
        else {
            ret = 0;
        }
    }
    return ret;    
}

u8 CrsGround::getSpiralCode() const {
    u8 ret = 0;
    if (!isObject() && mGround) {
        ret = (u8)mGround->spiralCode;
    }
    return ret;
}

u8 CrsGround::getSplashCode() const {
    u8 ret = 0;
    if (!isObject()) {
        ret = mGround ? mGround->splashCode : ret;
    }
    return ret;
}

f32 CrsGround::getSplashHeight() {
    f32 ret = -1000.0f;
    u8 splashCode = getSplashCode();
    switch(splashCode) {
    case 0:
        break;
    case 0xfe: {
        ret = GeoWater::getSupervisor()->searchWater(0)->getSurfaceHeight();
        break;
    }
    case 0xff: {
        TPoolSupervisor *pool = TMapObjPool::getSupervisor();
        JUT_ASSERT_MSG(pool, "NOT FOUND POOL");
        ret = pool->getHeight(mWorldPos.x, mWorldPos.z);
        break;
    }
    default: {
//#line 1603
        GeoSplash *splash = GeoSplash::getSupervisor()->search(splashCode);
        JUT_ASSERT_MSG(splash, "NOT FOUND SPLASH");
        ret = splash->getSurfaceHeight();
    }
    }
    return ret; 
}

int CrsGround::getSplashID() {
    int ret = 0;
    u8 splashCode = getSplashCode();
    switch(splashCode) {
    case 0:
        break;
    case 0xfe: {
        ret = 2;
        break;
    }
    case 0xff: {
        ret = 1;
        break;
    }
    default: {
//#line 1603
        GeoSplash *splash = GeoSplash::getSupervisor()->search(splashCode);
        JUT_ASSERT_MSG(splash, "NOT FOUND SPLASH");
        ret = splash->getSplashID();
    }
    }
    return ret; 
}

JugemPoint *CrsGround::getJugemPoint() {
    JugemPoint *ret = nullptr;
    switch (getAttribute()) {
    case Attr_5:
    case Attr_10:
    case Attr_14:
    case Attr_15:
    case Attr_Water:
    case Attr_55:
    case Attr_71: {
        ret = mCourse->searchJugemPoint(getAttrIndex());
        break;
    }
    case Attr_7: {
        if (getAttrIndex() != 0) {
            ret = mCourse->searchJugemPoint(getAttrIndex());
        }
        break;
    }
    }

    return ret;
}

JugemPoint *CrsGround::getCannonPoint() {
    JugemPoint *ret = nullptr;
    if (getAttribute() == Attr_Cannon) {
       ret = mCourse->searchJugemPoint(getAttrIndex()); 
    }
    return ret;
}

JugemPoint *CrsGround::getPipePoint() {
    JugemPoint *ret = nullptr;
    if (getAttribute() == Attr_Pipe) {
       ret = mCourse->searchJugemPoint(getAttrIndex()); 
    }
    return ret;
}

GeoCannon *CrsGround::getGeoCannon() {
    GeoCannon *ret = nullptr;
    if (getAttribute() == Attr_Cannon && GeoCannon::getSupervisor()) {
        ret = GeoCannon::getSupervisor()->searchCannon(getAttrIndex());
    }
    return ret;
}

f32 CrsGround::getWaterHeight() {
    f32 ret = -1000.0f;
    if (getAttribute() == Attr_Water && GeoWater::getSupervisor()) {
        GeoWater *water = GeoWater::getSupervisor()->searchWater(0);
        ret = water->getSurfaceHeight();
    }
    return ret;
}

bool CrsGround::isShaking() const {
    return mVelocity.squared() > 0.01f || mCourse->getCrsData()->getShaking() == 2;
}

bool CrsGround::isAttributeCollision() {
    bool ret = false;
    switch (getAttribute()) {
    case Attr_2:
    case Attr_18:
        ret = true;
        break;
    }
    return ret;
}

f32 CrsGround::getWallNormal(JGeometry::TVec3f *inVec1, JGeometry::TVec3f *inVec2) {
    
}

void CrsGround::addPullerVelocity() {
    if (getAttribute() == Attr_Puller) {
        GeoPuller *puller = GeoPuller::getSupervisor()->search(getAttrIndex());
        JGeometry::TVec3f pullerVel;
        puller->getVelocity(&pullerVel, mWorldPos);
        mVelocity.add(pullerVel);
    }
}
