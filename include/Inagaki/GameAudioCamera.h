#ifndef GAMEAUDIOCAMERA_H
#define GAMEAUDIOCAMERA_H

#include "Inagaki/GameAudioAudience.h"
#include "Inagaki/GameAudioMain.h"
#include "JSystem/JAudio/System/JASHeap.h"
#include "Kaneshige/Course/CrsArea.h"
#include "dolphin/mtx.h"

namespace GameAudio {

class CameraInfo {
public:
    CameraInfo() {
        _0.x = 0.0f;
        _0.y = 0.0f;
        _0.z = -1.0f;
        _c = _0;

        Vec up;
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;
        Vec target;
        target.x = 0.0f;
        target.y = 0.0f;
        target.z = 0.0f;
        C_MTXLookAt(_18, &_0, &up,&target);
        mArea = new (JASDram, 0) CrsArea();
        mEchoRate = 0.0f;
    }

    void setCam(Vec v, Mtx m) {
        _c = _0;
        _0 = v;
        for (u32 i = 0; i < 3; i++) {
            for (u32 j = 0; j < 4; j++) {
                _18[i][j] = m[i][j];
            }
        }
    }

    void setCamMtx(Mtx m) {

    }

    void setEchoRate(f32 rate) {
        mEchoRate = rate;
    }

//private:
    Vec _0;
    Vec _c;
    Mtx _18;
    CrsArea *mArea;
    f32 mEchoRate;
}; // Size: 0x50;

class CameraMgr {
public:
    CameraMgr(u32, CustomAudience<4> *audience);
    void setCameraInfo(u32, Vec, Mtx,  CrsArea *);
    void setSceneMax(u32);

//private:
    CustomAudience<4> *mAudience;
    u32 _4;
    u32 _8;
    CameraInfo *mCamInfo;
}; // Size: 0x10

}

#endif
