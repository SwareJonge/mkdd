#include "JSystem/JAudio/System/JASRegisterParam.h"
#include "JSystem/JUtility/JUTAssert.h"

JASRegisterParam::JASRegisterParam() {
    for (int i = 0; i < NORMAL_REGID_MAX; i++) {
        mRegs[i] = 0;
    }
}

void JASRegisterParam::init() {
    for (int i = 0; i < NORMAL_REGID_MAX; i++) {
        mRegs[i] = 0;
    }
}

void JASRegisterParam::write(JASRegisterParam::RegID regID, u32 val) {
    if (regID < NORMAL_REGID_MAX) {
        if (regID <= 2) {
            mRegs[regID] = (u8)val;
            return;
        }

        mRegs[regID] = val;
        return;
    }

    switch (regID) {
    case REGID32:
        mRegs[0] = (val >> 8) & 0xff;
        mRegs[1] = val & 0xff;
        break;
    case REGID33:
        mRegs[4] = val >> 16;
        mRegs[5] = val;
        break;
    case REGID34:
        mRegs[4] &= 0xff;
        mRegs[4] |= ((val & 0xff) << 8);
        break;
    case REGID35:
        mRegs[4] &= 0xff00;
        mRegs[4] |= val & 0xff;
        break;
    case REGID36:
        mRegs[5] &= 0xff;
        mRegs[5] |= (val & 0xff) << 8;
        break;
    case REGID37:
        mRegs[5] &= 0xff00;
        mRegs[5] |= val & 0xff;
        break;
    case REGID38:
        mRegs[6] = val >> 16;
        mRegs[7] = val;
        break;
    case REGID39:
        mRegs[8] = val >> 16;
        mRegs[9] = val;
        break;
    case REGID40:
        mRegs[10] = val >> 16;
        mRegs[11] = val;
        break;
    case REGID41:
        mRegs[12] = val >> 16;
        mRegs[13] = val;
        break;
    default:
#line 92
        JUT_WARNING_F2("%s", "Warning: Unsupported REG id");
        break;
    }
}

u32 JASRegisterParam::read(JASRegisterParam::RegID regID) {
    if (regID < 14) {
        return mRegs[regID];
    }
    u32 rv = 0;
    switch (regID) {
    case REGID32:
        rv = ((mRegs[0] & 0xff) << 8) | (mRegs[1] & 0xff) ;
        break;
    case REGID33:
        rv = mRegs[4];
        rv <<= 16;
        rv |= mRegs[5];
        break;
    case REGID34:
        rv = (u8)(mRegs[4] >> 8);
        break;
    case REGID35:
        rv = (u8)mRegs[4];
        break;
    case REGID36:
        rv = (u8)(mRegs[5] >> 8);
        break;
    case REGID37:
        rv = (u8)mRegs[5];
        break;
    case REGID38:
        rv = mRegs[6];
        rv <<= 16;
        rv |= mRegs[7];
        break;
    case REGID39:
        rv = mRegs[8];
        rv <<= 16;
        rv |= mRegs[9];
        break;
    case REGID40:
        rv = mRegs[10];
        rv <<= 16;
        rv |= mRegs[11];
        break;
    case REGID41:
        // !@bug I think this should get mRegs[12] and mRegs[13]. Looks like a copy paste bug.
        rv = mRegs[10];
        rv <<= 16;
        rv |= mRegs[11];
        break;
    default:
#line 160
        JUT_WARNING_F2("%s", "Warning: Unsupported REG id");
        break;
    }
    return rv;
}
