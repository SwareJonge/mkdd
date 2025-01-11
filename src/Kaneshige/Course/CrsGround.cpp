#include "Kaneshige/Course/CrsGround.h"
#include "Kaneshige/RaceMgr.h"

CrsGround::CrsGround(Course *course) {
    mCourse = RaceMgr::getManager()->getCourse();
    reset();
}

void CrsGround::reset() {}

void CrsGround::search(const JGeometry::TVec3f &) {}

void CrsGround::search(const JGeometry::TVec3f &, const JGeometry::TVec3f &) {}

void CrsGround::search(GeographyObj *, const JGeometry::TVec3f &, const JGeometry::TVec3f &) {}

void CrsGround::searchBase(GeographyObj *, const JGeometry::TVec3f &, const JGeometry::TVec3f &, unsigned long) {}

void CrsGround::search(const JGeometry::TVec3f &, const JGeometry::TVec3f &, float, bool) {}

void CrsGround::roundDepth(const JGeometry::TVec3f &) {}

void CrsGround::checkGridBySphere(float *, const CrsData::SColInfoSphere &, unsigned long, float, float, float) {}

bool CrsGround::checkPolygonCover(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, float, const CrsData::Ground *, float *) {}

void CrsGround::checkPolygonCoverMain(const JGeometry::TVec3f &, const CrsData::SColInfoSphere &, unsigned long, float, float, float, const CrsData::Ground *) {}

void CrsGround::isInsideGrid(const JGeometry::TVec3f &, float, float, float, float) {}

void CrsGround::searchGridIndex(int *, int *, Course *, const JGeometry::TVec3f &, float, float, float, float, int, int) {}

f32 CrsGround::getHeight() const { return mHeight; }

void CrsGround::getNormal(JGeometry::TVec3f *out) const { out->set(mNormal); }

u8 CrsGround::getCameraCode() const {}

void CrsGround::isItemInvalGround() const {}

u8 CrsGround::getAddThickness() const {}

u8 CrsGround::getStaggerCode() const {}

u8 CrsGround::getSpiralCode() const {}

u8 CrsGround::getSplashCode() const {}

f32 CrsGround::getSplashHeight() {}

void CrsGround::getSplashID() {}

void CrsGround::getJugemPoint() {}

void CrsGround::getCannonPoint() {}

JugemPoint *CrsGround::getPipePoint() {}

void CrsGround::getGeoCannon() {}

f32 CrsGround::getWaterHeight() {}

void CrsGround::isShaking() const {}

void CrsGround::isAttributeCollision() {}

void CrsGround::getWallNormal(JGeometry::TVec3f *, JGeometry::TVec3f *) {}

void CrsGround::addPullerVelocity() {}
