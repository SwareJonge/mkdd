#ifndef KARTCTRL_H
#define KARTCTRL_H

#include "types.h"
#include "Yamamoto/KartSound.h"
#include "Inagaki/KartSoundMgr.h"

class KartCtrl
{
public:
    static KartCtrl *getKartCtrl(); /*{
        return mCtrl;
    }*/

    KartSound *getKartSound(int idx)
    {
        return mKartSounds[idx];
    }

    GameAudio::KartSoundMgr *GetKartSoundMgr(int idx); /* {
        return getKartSound(idx)->mKartSoundMgr;
    }*/

    void GetBodyPos(int idx, JGeometry::TVec3<f32> *playerPos);

private:
    // static KartCtrl * mCtrl;
    u8 _0[0x1a0];
    KartSound *mKartSounds[8];
};

inline KartCtrl *GetKartCtrl() {
    return KartCtrl::getKartCtrl();
}

#endif