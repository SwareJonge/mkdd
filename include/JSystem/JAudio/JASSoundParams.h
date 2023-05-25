#ifndef JSYSTEM_JASSOUNDPARAMS_H
#define JSYSTEM_JASSOUNDPARAMS_H

#include "types.h"

class JASSoundParams {
    public:
    JASSoundParams() { init(); }

    void init() {
        initVolume();
        initPitch();
        initFxMix();
        initPan();
        initDolby();
    }

    void initVolume() { mVolume = 1.0f; }
    void initFxMix() { mFxMix = 0.0f; }
    void initPitch() { mPitch = 1.0f; }
    void initPan() { mPan = 0.5f; }
    void initDolby() { mDolby = 0.0f; }

    private:
    f32 mVolume;
    f32 mFxMix;
    f32 mPitch;
    f32 mPan;
    f32 mDolby;
};

#endif