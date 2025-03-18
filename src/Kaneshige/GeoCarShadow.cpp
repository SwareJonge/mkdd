#include "Kaneshige/Objects/GeoCarShadow.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

GeoCarShadow::GeoCarShadow() : GeographyObj(0x1003) {
    setObjFlagSimpleDraw();
    mShadowScale.set(1.0f, 1.0f, 1.0f);
    reset();
}

GeoCarShadow::GeoCarShadow(u32 id) : GeographyObj(id) {
    setObjFlagSimpleDraw();
    mShadowScale.set(1.0f, 1.0f, 1.0f);
    reset();
}

void GeoCarShadow::reset() {
    GeographyObj::resetObject();
    GeographyObj::clrObjFlagCheckGeoHitting();
    GeographyObj::clrObjFlagCheckItemHitting();
    GeographyObj::clrAllCheckKartHitFlag();

    mFrDir.set(0.0f, 0.0f, 1.0f);
    mUpDir.set(0.0f, 1.0f, 0.0f);
    mLfDir.set(1.0f, 0.0f, 0.0f);
}

const char * GeoCarShadow::getBmdFileName() {
    static const char *cBmdName = "/Objects/Car_Public1Shadow.bmd";
    return cBmdName;
}

void GeoCarShadow::setCarPose(const JGeometry::TVec3f &dir, const JGeometry::TVec3f &pos) {
    mPos.set(pos);

    CrsGround ground;
    ground.search(mPos);
    mPos.y = 20.0f + ground.getHeight();

    ground.getNormal(&mUpDir);

    mLfDir.cross(mUpDir, dir);
    mLfDir.normalize();

    mFrDir.cross(mLfDir, mUpDir);
    mRotMtx.setXYZDir(mLfDir, mUpDir, mFrDir);
}

GeoBombShadow::GeoBombShadow() : GeoCarShadow(0x1001){}

GeoKinoShadow::GeoKinoShadow() : GeoCarShadow(0x1002) {}

GeoHanaShadow::GeoHanaShadow() : GeoCarShadow(0x1000) {}

GeoHanaBodyShadow::GeoHanaBodyShadow() : GeoCarShadow(0xfff) {}
