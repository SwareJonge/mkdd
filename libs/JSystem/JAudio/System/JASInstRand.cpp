#include <JSystem/JAudio/System/JASInst.h>
#include <JSystem/JUtility/JUTDbg.h>

// probably fakematch
void JASInstRand::effect(int, int, JASSoundParams *params) const
{
    static JMath::TRandom_fast_ oRandom(0);
    f32 val = (FLT_EPSILON); // has to be declared here to prevent regswaps
    val = getY(1.0f - val, oRandom.get_ufloat_1() * 2.0f, 1.0f);
    val *= mCeiling;
    val += mFloor;

    switch(mType) {
    case 0:
        params->mVolume *= val;
        break;
    case 1:
        params->mPitch *= val;
        break;
    case 2:
        params->mPan += val - 0.5; // yes, they forgot the f
        break;
    case 3:
        params->mFxMix += val;
        break;
    case 4:
        params->mDolby += val;
        break;
    default:
#line 42
        JUT_ASSERT(0);
        break;
    }
}
