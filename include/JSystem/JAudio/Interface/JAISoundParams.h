#ifndef JAISOUNDPARAMS_H
#define JAISOUNDPARAMS_H

#include "JSystem/JAudio/System/JASSoundParams.h"

struct JAISoundParamsProperty
{
    void init()
    {
        _0 = 1.0f;
        _4 = 0.0f;
        field_0x8 = 1.0f;
    }

    f32 _0;
    f32 _4;
    f32 field_0x8;
}; // Size: 0xC

struct JAISoundParamsTransition
{
    struct TTransition
    {
        void zero()
        {
            _0 = 0.0f;
            mCount = 0;
            _4 = 0.0f;
        }

        void set(f32 newValue, f32 intensity, u32 fadeCount)
        {
            mCount = fadeCount;
            _0 = (newValue - intensity) / mCount;
            _4 = newValue;
        }

        f32 apply(f32 param)
        {
            if (mCount > 1)
            {
                mCount--;
                param += _0;
            }
            else
            {
                if (mCount == 1)
                {
                    mCount = 0;
                    param = _4;
                }
            }
            return param;
        }

        f32 _0;
        f32 _4;
        u32 mCount;
    }; // Size: 0xC

    void init()
    {
        mVolume.zero();
        mPitch.zero();
        mFxMix.zero();
        mPan.zero();
        mDolby.zero();
    }

    void apply(JASSoundParams *pParams)
    {
        pParams->mVolume = mVolume.apply(pParams->mVolume);
        pParams->mPitch = mPitch.apply(pParams->mPitch);
        pParams->mFxMix = mFxMix.apply(pParams->mFxMix);
        pParams->mDolby = mDolby.apply(pParams->mDolby);
        pParams->mPan = mPan.apply(pParams->mPan);
    }

    TTransition mVolume;
    TTransition mPitch;
    TTransition mFxMix;
    TTransition mPan;
    TTransition mDolby;
}; // Size: 0x3C

struct JAISoundParamsMove
{
    JAISoundParamsMove() : mParams() {}

    void init()
    {
        mParams.init();
        mTransition.init();
    }

    void calc() { mTransition.apply(&mParams); }

    void moveVolume(f32, u32);
    void movePitch(f32, u32);
    void moveFxMix(f32, u32);
    void movePan(f32, u32);
    void moveDolby(f32, u32);

    JASSoundParams mParams;
    JAISoundParamsTransition mTransition;
}; // Size: 0x50

struct JAISoundParams
{
    JAISoundParams() : mMove() {}
    void mixOutAll(JASSoundParams const &, JASSoundParams *, f32);

    void init()
    {
        mMove.init();
        mProperty.init();
    }

    JAISoundParamsProperty mProperty;
    JAISoundParamsMove mMove;
}; // Size: 0x5C

#endif /* JAISOUNDPARAMS_H */