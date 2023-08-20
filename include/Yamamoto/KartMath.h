#ifndef KARTMATH_H
#define KARTMATH_H

#include <JSystem/JGeometry.h>

class KartMath
{
public:
    f32 SpeedySqrtf(register f32);
    void CrossToPolar(JGeometry::TVec3<f32> *, JGeometry::TVec3f *, f32 *, f32 *, f32 *);
    void PolarToCross(VecPtr, VecPtr, f32, f32, f32);
    f32 lu(Mtx33, int *);
    void solve(Mtx33, f32 *, int *, f32 *, int);
    f32 Gauss(Mtx33, f32 *, f32 *);
    f32 LimmtNumber(f32, f32);
    f32 fcnvge(f32, f32, f32, f32);
    void ChaseFnumber(f32 *, f32, f32);
    void ChaseFcnvge(f32 *, u16 *, f32, u16);
    void ChaseFcnvge4(f32 *, u16 *, f32, u16);
    // Inline/Unused
    // void GetDir(JGeometry::TVec3f *, JGeometry::TVec3f *);
    // void cnvge(int, int, int, int);
};

#endif