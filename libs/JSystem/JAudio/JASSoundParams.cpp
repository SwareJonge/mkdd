#include <JSystem/JAudio/JASSoundParams.h>

// UNUSED
void JASSoundParams::clamp()
{
    clampVolume();
    clampPitch();
    clampFxMix();
    clampPan();
    clampDolby();
}

// UNUSED
void JASSoundParams::print() const {}

// UNUSED
void JASSoundParams::combine(const JASSoundParams &other)
{
    mVolume = mVolume * other.mVolume;
    mPitch = mPitch * other.mPitch;
    mFxMix = mFxMix + other.mFxMix;
    mPan = mPan + other.mPan - 0.5f;
    mDolby = mDolby + other.mDolby;
}

void JASSoundParams::combine(const JASSoundParams &other1, const JASSoundParams &other2)
{
    mVolume = other1.mVolume * other2.mVolume;
    mPitch = other1.mPitch * other2.mPitch;
    mFxMix = other1.mFxMix + other2.mFxMix;
    mPan = other1.mPan + other2.mPan - 0.5f;
    mDolby = other1.mDolby + other2.mDolby;
}

// UNUSED
void JASSoundParams::combine(const JASSoundParams &other1, const JASSoundParams &other2, const JASSoundParams &other3)
{
    mVolume = other1.mVolume * other2.mVolume * other3.mVolume;
    mPitch = other1.mPitch * other2.mPitch * other3.mPitch;
    mFxMix = other1.mFxMix + other2.mFxMix + other3.mFxMix;
    mPan = other1.mPan + other2.mPan + other3.mPan - 0.5f;
    mDolby = other1.mDolby + other2.mDolby + other3.mDolby;
}

// UNUSED
void JASSoundParams::combine(const JASSoundParams &other1, const JASSoundParams &other2, const JASSoundParams &other3, const JASSoundParams &other4)
{
    mVolume = other1.mVolume * other2.mVolume * other3.mVolume * other4.mVolume;
    mPitch = other1.mPitch * other2.mPitch * other3.mPitch * other4.mPitch;
    mFxMix = other1.mFxMix + other2.mFxMix + other3.mFxMix + other4.mFxMix;
    mPan = other1.mPan + other2.mPan + other3.mPan + other4.mPan - 0.5f;
    mDolby = other1.mDolby + other2.mDolby + other3.mDolby + other4.mDolby;
}