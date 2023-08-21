#ifndef KARTVEC_H
#define KARTVEC_H

#include "Yamamoto/KartMath.h"

class KartVec : public KartMath
{
public:
    void SetVector(VecPtr, f32, f32, f32);
    void AddVector(VecPtr, f32, f32, f32);
    void MulVector(VecPtr, f32, f32, f32);
    f32 VectorLength(VecPtr);
    f32 VectorLengthSqrtf(VecPtr);
    void OuterCalc(VecPtr, VecPtr, VecPtr);
    f32 InnerCalc(VecPtr, VecPtr);
    void VectorElement(JGeometry::TVec3f *, JGeometry::TVec3f *, JGeometry::TVec3f *);
    // Inline/Unused
    void SetZeroVector(VecPtr);
    // void AddVectorToVector(VecPtr, VecPtr);
    void SubVector(VecPtr, f32, f32, f32);
    // void SubVectorToVector(VecPtr, VecPtr);
    // void MulVectorToVector(VecPtr, VecPtr);
    void DevVector(VecPtr, f32, f32, f32);
    // void DevVectorToVector(VecPtr, VecPtr);
    void NormalizeVector(VecPtr);
};

#endif