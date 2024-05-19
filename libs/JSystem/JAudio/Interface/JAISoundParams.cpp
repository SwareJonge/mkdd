#include "JSystem/JAudio/Interface/JAISoundParams.h"

void JAISoundParamsMove::moveVolume(f32 newValue, u32 fadeCount) 
{
#line 12
    JUT_ASSERT((newValue>=0.f))
    if (fadeCount == 0)
    {
        mParams.mVolume = newValue;
        mTransition.mVolume.mCount = 0;
    }
    else
    {
        mTransition.mVolume.set(newValue, mParams.mVolume, fadeCount);
    }
}

void JAISoundParamsMove::movePitch(f32 newValue, u32 fadeCount)
{
#line 27
    JUT_ASSERT((newValue>=0.f))
    if (fadeCount == 0)
    {
        mParams.mPitch = newValue;
        mTransition.mPitch.mCount = 0;
    }
    else
    {
        mTransition.mPitch.set(newValue, mParams.mPitch, fadeCount);
    }
}

void JAISoundParamsMove::moveFxMix(f32 newValue, u32 fadeCount)
{
    if (fadeCount == 0)
    {
        mParams.mFxMix = newValue;
        mTransition.mFxMix.mCount = 0;
    }
    else
    {
        mTransition.mFxMix.set(newValue, mParams.mFxMix, fadeCount);
    }
}

void JAISoundParamsMove::movePan(f32 newValue, u32 fadeCount)
{
    if (fadeCount == 0)
    {
        mParams.mPan = newValue;
        mTransition.mPan.mCount = 0;
    }
    else
    {
        mTransition.mPan.set(newValue, mParams.mPan, fadeCount);
    }
}

void JAISoundParamsMove::moveDolby(f32 newValue, u32 fadeCount)
{
    if (fadeCount == 0)
    {
        mParams.mDolby = newValue;
        mTransition.mDolby.mCount = 0;
    }
    else
    {
        mTransition.mDolby.set(newValue, mParams.mDolby, fadeCount);
    }
}

void JAISoundParamsMove::move(const JASSoundParams &params, u32 fadeCount)
{
    // UNUSED
}