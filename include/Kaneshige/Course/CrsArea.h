#ifndef CRSAREA_H
#define CRSAREA_H

#include "types.h"

#include "JSystem/JGeometry.h"

#include "Kaneshige/Course/Course.h"

class CrsArea
{
public:
    CrsArea();

    bool enableLightParam();
    void getRoofPosition(JGeometry::TVec3f *out);
    int getCameraID() const;
    f32 getEchoRate();
    s16 getLightParamID();
    s16 getShadowID() const;    

    void reset();

    void search(u8 id, JGeometry::TVec3<f32> const &p2);
    void searchShadow(JGeometry::TVec3<f32> const &p1) { search(0, p1); }
    void searchCamera(JGeometry::TVec3<f32> const &p1) { search(1, p1); }
    void searchRoof(JGeometry::TVec3<f32> const &p1) { search(2, p1); }
    void searchLight(JGeometry::TVec3<f32> const &p1) { search(7, p1); }

    bool isInside() const { return mArea != nullptr; }
    f32 getRate() const { return mRate; }
    f32 getShadowRate() const { return getRate(); }

private:
    JGeometry::TVec3<f32> mPos;
    Course::Area *mArea;
    f32 mRate;
};

#endif