#ifndef SEQUENCEINFO_H
#define SEQUENCEINFO_H

#include "Kaneshige/KartInfo.h"
#include "types.h"

struct DemoKart
{ // most likely a placeholder struct
    ECharID char1;
    ECharID char2;
    EKartID kart;
};

class SequenceInfo
{
public:
    void rndDemo(u32);
    void getDemoKart(int no, ECharID &charID1, ECharID &charID2, EKartID &kartID)
    {
#line 413
        JUT_MINMAX_ASSERT(0, no, 8);

        charID1 = mDemoKart[no].char1;
        charID2 = mDemoKart[no].char2;
        kartID = mDemoKart[no].kart;
    }

    void setGhostFlag(u8 flags) { mGhostFlags = flags; }

private:
    u8 _0[0x34];
    u8 mGhostFlags;
    u8 _35[0x36c - 0x35];
    DemoKart mDemoKart[8];
    u8 _0x390[0x44c - 0x390];
};

extern SequenceInfo gSequenceInfo;

#endif