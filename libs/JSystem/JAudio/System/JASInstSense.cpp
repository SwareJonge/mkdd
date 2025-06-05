#include "JSystem/JAudio/System/JASInst.h"
#include "JSystem/JUtility/JUTAssert.h"

void JASInstSense::effect(int trigger1, int trigger2, JASSoundParams *params) const {
    int trigger = 0;
	switch (mTrigger) {
	case 1:
		trigger = trigger2;
		break;
	case 2:
		trigger = trigger1;
		break;
	}
	f32 y;
	if (mCenterKey == 0x7f || mCenterKey == 0)
		y = _0C + trigger * (_10 - _0C) / 127.0f;
	else if (trigger < mCenterKey)
		y = _0C + (1.0f - _0C) * (trigger / (f32)mCenterKey);
	else
		y = (_10 - 1.0f) * ((trigger - mCenterKey) / (f32)(0x7f - mCenterKey)) + 1.0f;
    // part above could be getY, old function from TInstSense

    switch(mType) {
    case 0:
        params->mVolume *= y;
        break;
    case 1:
        params->mPitch *= y;
        break;
    case 2:
        params->mPan += y - 0.5; // yes, they forgot the f
        break;
    case 3:
        params->mFxMix += y;
        break;
    case 4:
        params->mDolby += y;
        break;
    default:
#line 56
        JUT_ASSERT(0);
        break;
    }
}

void JASInstSense::setParams(int trigger, int centerkey, f32 p3, f32 p4) {
#line 65
    JUT_ASSERT(trigger >= 0);
    JUT_ASSERT(trigger < 256);
    mTrigger = trigger;

    JUT_ASSERT(centerkey >= 0);
    JUT_ASSERT(centerkey < 256);
    mCenterKey = centerkey;

    _0C = p3;
    _10 = p4;
}
