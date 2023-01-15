#ifndef PAD_H
#define PAD_H

#include "types.h"

extern "C" {

#define PAD_STOP_RUMBLE 0
#define PAD_START_RUMBLE 1
#define PAD_STOP_HARD 2

    typedef void (*PADSamplingCallback)();

    struct PADStatus
    {
        u16 mButton;      // _0
        u8 mStickX;       // _2
        u8 mStickY;       // _3
        u8 mSubStickX;    // C Stick X (_4)
        u8 mSubStickY;    // C Stick Y (_5)
        u8 mTriggerLeft;  // Left trigger pressure (_6)
        u8 mTriggerRight; // Right trigger pressure (_7)
        u8 mAnalogA;      // _8
        u8 mAnalogB;      // _9
        u8 mCurError;     // Some odd number based on some constant (_10)
    };

    bool PADReset(u32 mask);
    bool PADRecalibrate(u32 mask);
    void PADClamp(PADStatus* padStat);
    bool PADInit();
    u32 PADRead(PADStatus* padStat);
    void PADControlMotor(u32 channel, u32 command);
    void PADSetAnalogMode(u32 mode);
    void PADSetSpec(u32 padModel);
}

#endif