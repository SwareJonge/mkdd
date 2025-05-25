#include "JSystem/JAudio/Interface/JAISoundParams.h"

void JAISoundParamsMove::moveVolume(f32 newValue, u32 fadeCount) 
{
#line 12
    JUT_ASSERT((newValue>=0.f))
    if (fadeCount == 0)
    {
        mVolume = newValue;
        mTransition.mVolume.mCount = 0;
    }
    else
    {
        mTransition.mVolume.set(newValue, mVolume, fadeCount);
    }
}

void JAISoundParamsMove::movePitch(f32 newValue, u32 fadeCount)
{
#line 27
    JUT_ASSERT((newValue>=0.f))
    if (fadeCount == 0)
    {
        mPitch = newValue;
        mTransition.mPitch.mCount = 0;
    }
    else
    {
        mTransition.mPitch.set(newValue, mPitch, fadeCount);
    }
}

void JAISoundParamsMove::moveFxMix(f32 newValue, u32 fadeCount)
{
    if (fadeCount == 0)
    {
        mFxMix = newValue;
        mTransition.mFxMix.mCount = 0;
    }
    else
    {
        mTransition.mFxMix.set(newValue, mFxMix, fadeCount);
    }
}

void JAISoundParamsMove::movePan(f32 newValue, u32 fadeCount)
{
    if (fadeCount == 0)
    {
        mPan = newValue;
        mTransition.mPan.mCount = 0;
    }
    else
    {
        mTransition.mPan.set(newValue, mPan, fadeCount);
    }
}

void JAISoundParamsMove::moveDolby(f32 newValue, u32 fadeCount)
{
    if (fadeCount == 0)
    {
        mDolby = newValue;
        mTransition.mDolby.mCount = 0;
    }
    else
    {
        mTransition.mDolby.set(newValue, mDolby, fadeCount);
    }
}

void JAISoundParamsMove::move(const JASSoundParams &params, u32 fadeCount)
{
    // UNUSED
}
