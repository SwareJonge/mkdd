#ifndef KARTCAMERA_H
#define KARTCAMERA_H

#include <JSystem/J3DU/J3DUClipper.h>
#include "types.h"


class KartBody;
class KartCtrl;

class KartCam {
public:
    KartCam(KartBody *, KartCtrl *, u8, u8);
    void DegubCamera(char *, u8);
    void SetStartDemoPort();
    void SetAwardDemoPort();
    void SetOneScreenPort();
    void SetSubScreenPort();
    void SetTwoScreenPort(u8);
    void SetFourScreenPort(u8);
    void DoPort(int);
    void Init(bool);
    void DoInit();
    void DoResetInit();
    void DoRaceCtrl();
    void DoCameraMode();
    void DoCamera();

    JGeometry::TVec3f *GetCameraPos();

    // this class is divided in 2 files?
    JGeometry::TVec3f *SetTargetNum(u8);

    J3DUClipper *GetClipper() { return &mClipper; }

    // Inlines
    const Mtx &GetMtx() {
        return mMtx;
    }
    f32 GetHeight() { return mHeight; } // KartCam.h // 0x801afd7c
    f32 GetWidth() { return mWidth; } // KartCam.h // 0x801afd84
    int GetPosh() { return mPosH; } // KartCam.h // 0x801afd8c
    int GetPosv() { return mPosV; } // KartCam.h // 0x801afd94
private:
    KartBody *mBody;
    KartCtrl *mCtrl;
    J3DUClipper mClipper;
    u8 _64[0x80 - 0x64];
    Mtx mMtx;
    u8 _B0[0xEC - 0xB0];
    int mPosV;
    int mPosH;
    u8 _F4[0x128 - 0xF4];
    f32 mWidth;
    f32 mHeight;
};

#endif // KARTCAMERA_H