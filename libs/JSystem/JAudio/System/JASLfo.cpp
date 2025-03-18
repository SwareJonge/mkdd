#include "JSystem/JAudio/System/JASLfo.h"
#include "JSystem/JMath/JMath.h"

JASLfo JASLfo::sFreeRunLfo;

JASLfo::JASLfo() {
    _0 = 0;
    _4 = 1;
    _8 = 0.0f;
    mDepth = 0.0f;
    mPitch = 1.0f;
    mDelay = 0;
    mCounter = 0;
}

f32 JASLfo::getValue() const {
    if (mCounter != 0)
        return 0.0f;

    if (mDepth == 0.0f)
        return 0.0f;

    return JMASSinLap(_8) * mDepth;
}

void JASLfo::incCounter(f32 x) {
    if (mCounter != 0) {
        mCounter -= 1;
        return;
    }
   
    _8 += x * (mPitch * 0.013888889f);  // ~(1/72)
    if (_8 >= 1.0f) {
        _8 -= 1.0f;
    }
}

void JASLfo::resetCounter() {
    mCounter = mDelay;
    switch (_4) {
    case 0:
        _8 = sFreeRunLfo._8;
        break;
    case 1:
        _8 = 0.0f;
        break;
    }
}
