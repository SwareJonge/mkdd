#ifndef JAUDIO_JAISOUNDPARAMS_H
#define JAUDIO_JAISOUNDPARAMS_H

#include "JSystem/JUtility/JUTAssert.h"

#include "JSystem/JAudio/System/JASSoundParams.h"

struct JAISoundParamsProperty
{
    void init()
    {
        _0 = 1.0f;
        _4 = 0.0f;
        _8 = 0.0f; // 1.0f in TP
    }

    f32 _0;
    f32 _4;
    f32 _8;
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

struct JAISoundParamsMove : public JASSoundParams // not entirely sure if this actually inherits, however it makes access to it a bit cleaner
{
    JAISoundParamsMove() {}

    void init()
    {
        JASSoundParams::init();
        mTransition.init();
    }

    void calc() { mTransition.apply(this); }

    void moveVolume(f32 newValue, u32 fadeCount);
    void movePitch(f32 newValue, u32 fadeCount);
    void moveFxMix(f32 newValue, u32 fadeCount);
    void movePan(f32 newValue, u32 fadeCount);
    void moveDolby(f32 newValue, u32 fadeCount);
    void move(const JASSoundParams &params, u32 fadeCount);

    JAISoundParamsTransition mTransition;
}; // Size: 0x50

struct JAISoundParams
{
    JAISoundParams() : mMove() {}
    void mixOutAll(const JASSoundParams &inParams, JASSoundParams *outParams, f32 intensity);

    void init()
    {
        mMove.init();
        mProperty.init();
    }

    JAISoundParamsProperty mProperty;
    JAISoundParamsMove mMove;
}; // Size: 0x5C

#endif
