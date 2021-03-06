#ifndef CRSGROUND_H
#define CRSGROUND_H

#include "types.h"

#include "JSystem/JGeometry.h"

#include "Kaneshige/Course/Course.h"

class CrsGround
{
public:
    CrsGround(Course *course = nullptr);
    void reset();
    void search(JGeometry::TVec3<f32> const &);
    u8 getAddThickness(void) const;
    void getVelocity(JGeometry::TVec3<f32> *) const;

    s32 getMaterial() const;
    s32 getAttrIndex() const;
    s32 getAttribute() const;
    u8 getSpiralCode() const;
    u8 getSplashCode() const;
    f32 getSplashHeight();
    u8 getStaggerCode() const;
    u8 getCameraCode() const;

private:
    Course *mCourse;
    JGeometry::TVec3<f32> _0x4;
    JGeometry::TVec3<f32> _0x10;
    bool _0x1c;
    u8 _0x1d;
    void *mGround;
    void *mGeoObj;
    JGeometry::TVec3<f32> mNormal;
    JGeometry::TVec3<f32> mVelocity;
    JGeometry::TVec3<f32> _0x40;
    s32 _0x4c;
    s32 _0x50;
    s32 mMaterial;
    f32 mHeight;
    f32 _0x5C;
};

#endif