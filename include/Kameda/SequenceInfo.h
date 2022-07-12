#ifndef SEQUENCEINFO_H
#define SEQUENCEINFO_H

#include "Kaneshige/KartInfo.h"
#include "types.h"

struct DemoKart { // most likely a placeholder struct
    ECharID char1;
    ECharID char2;
    EKartID kart;
};

class SequenceInfo {
    public:
    void rndDemo(u32);
    void getDemoKart(int no, ECharID &charID1, ECharID &charID2, EKartID &kartID)
    {
        bool valid = false;
        if (no >= 0 && (no < 8))
            valid = true;

        if (!valid) {
            JUTAssertion::showAssert_f(JUTAssertion::getSDevice(), __FILE__, 413, "range over: %d <= no=%d < %d", 0, no, 8);
            OSPanic(__FILE__, 413, "Halt");
        }

        charID1 = mDemoKart[no].char1;
        charID2 = mDemoKart[no].char2;
        kartID = mDemoKart[no].kart;
    }

    private:
    u8 _0[0x36c];
    DemoKart mDemoKart[8];
    u8 _0x390[0x44c - 0x390];
};

extern SequenceInfo gSequenceInfo;

#endif