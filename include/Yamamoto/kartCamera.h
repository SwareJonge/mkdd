#ifndef KARTCAMERA_H
#define KARTCAMERA_H

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

    // this class is divided in 2 files?
    void SetTargetNum(u8);

    // Inlines
    const Mtx &GetMtx() {
        return mMtx;
    }
    void GetHeight(); // KartCam.h // 0x801afd7c
    void GetWidth(); // KartCam.h // 0x801afd84
    void GetPosh(); // KartCam.h // 0x801afd8c
    void GetPosv(); // KartCam.h // 0x801afd94
private:
    u8 _00[0x80];
    Mtx mMtx;
};

#endif // KARTCAMERA_H