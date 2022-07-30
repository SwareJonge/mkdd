#ifndef CRSAREA_H
#define CRSAREA_H

#include "types.h"

#include "JSystem/JGeometry.h"

class CrsArea
{
public:
    CrsArea();
    void searchCamera(JGeometry::TVec3<f32> const &);
    void searchLight(JGeometry::TVec3<f32> const &);
    void searchRoof(JGeometry::TVec3<f32> const &);
    void searchShadow(JGeometry::TVec3<f32> const &);

    s32 getCameraID() const;
    f32 getEchoRate();
    s16 getLightParamID();
    f32 getRate() const {
        return mRate;
    }
    f32 getShadowRate() const {
        return getRate();
    }

private:
    JGeometry::TVec3<f32> _0; // documented as "pos" in ghidra
    void *mArea;
    f32 mRate;
};

#endif