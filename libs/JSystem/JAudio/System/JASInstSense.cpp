#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JUtility/JUTAssert.h"

void JASInstSense::setParams(int trigger, int centerkey, f32 x, f32 y) {
#line 65
    JUT_ASSERT(trigger >= 0);
    JUT_ASSERT(trigger < 256);
    _08 = trigger;

    JUT_ASSERT(centerkey >= 0);
    JUT_ASSERT(centerkey < 256);
    _09 = centerkey;

    _0C = x;
    _10 = y;
}

void JASInstSense::effect(int x, int y, JASSoundParams *params) const {
    f32 val = 1; // i need ghidra
    val *= x;
    val += y;

    switch(mType) {
    case 0:
        params->mVolume *= val;
        break;
    case 1:
        params->mPitch *= val;
        break;
    case 2:
        params->mPan += val - 0.5;
        break;
    case 3:
        params->mFxMix += val;
        break;
    case 4:
        params->mDolby += val;
        break;
    default:
#line 56
        JUT_ASSERT(0);
        break;
    }
}
