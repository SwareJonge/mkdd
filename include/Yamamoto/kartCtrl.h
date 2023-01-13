#ifndef KARTCTRL_H
#define KARTCTRL_H

#include "types.h"
#include "Yamamoto/KartSound.h"
#include "Inagaki/KartSoundMgr.h"

class KartCam;

class KartCtrl
{
public:
    static void makeKartCtrl();
    static KartCtrl *getKartCtrl() { return mCtrl; }
    KartCam * getKartCam(int idx) { return mKartCams[idx]; }
    KartSound *getKartSound(int idx) { return mKartSounds[idx]; }
    GameAudio::KartSoundMgr *GetKartSoundMgr(int idx) { return getKartSound(idx)->mKartSoundMgr; }
    void GetBodyPos(int idx, JGeometry::TVec3<f32> *playerPos);
    void DynamicsInit(bool);

private:
    static KartCtrl * mCtrl;
    u8 _0[0x1a0];
    KartSound *mKartSounds[8]; // 0x1a0
    u8 _1c0[0x200 - 0x1c0];
    KartCam * mKartCams[8]; // 0x200;
    u8 _220[4];
    u32 mBitfield; // 224
    u8 _228[4];
    int mKartCount;
};

inline KartCtrl *GetKartCtrl() {
    return KartCtrl::getKartCtrl();
}

#endif