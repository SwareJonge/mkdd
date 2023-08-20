#ifndef KARTMAT_H
#define KARTMAT_H

#include "Yamamoto/KartVec.h"

class KartMat : public KartVec
{
public:
    void ClearRotMatrix(Mtx);
    void SetPosePosMatrix(Mtx, Mtx, VecPtr);
    void SetPosMatrix(Mtx, VecPtr);
    void MulMatrixByVector(VecPtr, VecPtr, Mtx);
    void DevMatrixByVector(VecPtr, VecPtr, Mtx);
    void DevMatrixByVector2(JGeometry::TVec3f *, JGeometry::TVec3f *, Mtx);
    void DevCrdMatrixByVector(VecPtr, VecPtr, Mtx);
    void MulMatrix(Mtx, Mtx, Mtx);
    void RotYMatrix(Mtx, f32);
    void RotYMatrix33(Mtx, f32);
    void RotXMatrix(Mtx, f32);
    void RotZMatrix(Mtx, f32);
    void AddMatrix(Mtx, Mtx);
    void NormalizeMatrix(Mtx);
    void OmegaMatrix(Mtx, VecPtr, f32);

    // Inline/Unused
    // void CopyRotToRot(Mtx, Mtx);
    // void Copy33RotToRot(Mtx, Mtx);
    // void MulCrdMatrixByVector(VecPtr,VecPtr, Mtx);
    // void MulCrdMatrix(Mtx, Mtx, Mtx);
    // void RotZMatrix33(Mtx, f32);
    // void SubMatrix(Mtx, Mtx);
    // void OmegaMatrix(Mtx,VecPtr,VecPtr);
    // void CreateRotationAffineMtx(Mtx,VecPtr,VecPtr, f32);
};

#endif