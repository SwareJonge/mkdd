#ifndef _JSYSTEM_J3D_J3DUCLIPPER_H
#define _JSYSTEM_J3D_J3DUCLIPPER_H

#include "JSystem/J3D/J3DMath.h"

class J3DModel;
class JUTDbPrint;

struct J3DUClipper
{
    J3DUClipper(f32 fovY, f32 aspect, f32 near, f32 far);
    J3DUClipper() { init(); }

    virtual ~J3DUClipper() {} // _08

    void init();
    void calcViewFrustum();
    int clip(const Mtx transMtx, Vec point, f32 buffer) const;
    int clip(const Mtx transMtx, Vec *minCorner, Vec *maxCorner) const;

    void setFovy(f32 fovy) { mFovY = fovy; }
    void setAspect(f32 aspect) { mAspect = aspect; }
    void setNear(f32 near) { mNear = near; }
    void setFar(f32 far) { mFar = far; }

    // Inline/Unused
    void report() const;
    void debugPrint(JUTDbPrint *) const;
    void clipBySphere(J3DModel *, f32) const;
    void clipByBox(J3DModel *) const;

    // _00 = VTBL
    Vec mLeftPlane;   // _04
    Vec mRightPlane;  // _10
    Vec mTopPlane;    // _1C
    Vec mBottomPlane; // _28
    u8 _34[0x18];     // _34, unknown
    f32 mFovY;        // _4C
    f32 mAspect;      // _50
    f32 mNear;        // _54
    f32 mFar;         // _58
};

#endif
