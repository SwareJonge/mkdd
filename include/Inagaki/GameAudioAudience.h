#ifndef GAMEAUDIOAUDIENCE_H
#define GAMEAUDIOAUDIENCE_H


#include "JSystem/JAudio/JAUAudience.h"
namespace GameAudio {

template <int N>
class CustomAudience : public JAUDopplerAudience<N> {
public:
    CustomAudience();
    ~CustomAudience(); // 08, override
    JAIAudible *newAudible(const JGeometry::TVec3f &p1, JAISoundID soundID, const JGeometry::TVec3f *p3, u32 channel); // 0C, override

    void init(int i) { // fabricated
        mEchoRate[i] = 0.0f;
        if (i < N) // ???
            smCameraVolume[i] = 1.0f;
    }

    void setEchoRate(int i, f32 rate) {
        mEchoRate[i] = rate;
    }

    static f32 smCameraVolume[N];

    u8 _1a4[0x1dc - 0x1a4];
    f32 mEchoRate[N];
}; // Size: 0x1ec(probably variable)

}

#endif
