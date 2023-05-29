#ifndef JSYSTEM_JASSOUNDPARAMS_H
#define JSYSTEM_JASSOUNDPARAMS_H

#include "types.h"

struct JASSoundParams
{
    JASSoundParams() { init(); }

    void clamp();
    void combine(const JASSoundParams &other);
    void combine(const JASSoundParams &other1, const JASSoundParams &other2);
    void combine(const JASSoundParams &other1, const JASSoundParams &other2, const JASSoundParams &other3);
    void combine(const JASSoundParams &other1, const JASSoundParams &other2, const JASSoundParams &other3, const JASSoundParams &other4);
    void print() const;

    void init()
    {
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

    void clampVolume()
    {
        if (mVolume < 0.0f)
            mVolume = 0.0f;
        else if (mVolume > 1.0f)
            mVolume = 1.0f;
    }
    void clampFxMix(void)
    {
        if (mFxMix < 0.0f)
            mFxMix = 0.0f;
        else if (mFxMix > 1.0f)
            mFxMix = 1.0f;
    }
    void clampPitch()
    {
        if (mPitch < 0.0f)
            mPitch = 0.0f;
        else if (mPitch > 8.0f)
            mPitch = 8.0f;
    }
    void clampPan()
    {
        if (mPan < 0.0f)
            mPan = 0.0f;
        else if (mPan > 1.0f)
            mPan = 1.0f;
    }
    void clampDolby()
    {
        if (mDolby < 0.0f)
            mDolby = 0.0f;
        else if (mDolby > 1.0f)
            mDolby = 1.0f;
    }

    f32 mVolume;
    f32 mFxMix;
    f32 mPitch;
    f32 mPan;
    f32 mDolby;
};

#endif