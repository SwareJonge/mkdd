#ifndef KARTLINE_H
#define KARTLINE_H

#include <JSystem/JGeometry.h>

class Spline 
{
public:
    Spline() {}
    void getBezierPoint(JGeometry::TVec3f *, f32 t);
    
    void set(int idx, JGeometry::TVec3f const &point) 
    {
        if (idx >= 0 && idx < 4)
            mPoints[idx] = point;
    }

    void setAll(const JGeometry::TVec3f *points) {
        for(int i = 0; i < 4; i++) 
            set(i, points[i]);
    }

    JGeometry::TVec3f mPoints[4];
}; // Size: 0x30

#endif