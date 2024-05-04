#ifndef KARTCANNON_H
#define KARTCANNON_H

#include <JSystem/JGeometry/Vec.h>

#include "types.h"

class KartCannon
{
public:
    KartCannon() {}
    void Init(int);            // 0x8030273c
    void MakeCannon();         // 0x80302788
    void DoKeep();             // 0x80302abc
    void SetPose();            // 0x80302b30
    void DoCatch();            // 0x80302f20
    void DoLaunch();           // 0x80302fac
    void DoFlight();           // 0x80303058
    void DoTouchDown();        // 0x803031cc
    void DoLanding();          // 0x80303294
    void DoCheckEnd();         // 0x8030330c
    void DoRainbowLaunch();    // 0x80303474
    void DoRainbowFlight();    // 0x80303520
    void DoRainbowTouchDown(); // 0x803036c8
    void DoRainbowLanding();   // 0x80303794
    void DoRainbowCheckEnd();  // 0x80303810
    void DoCannonCrl();        // 0x80303900
    void DoDonkyCannonCrl();   // 0x80303934
    void DoRainbowCannonCrl(); // 0x80303b80
    void DoAfterCannonCrl();   // 0x80303dd4
private:
    u8 _0[0x1c];           //
    JGeometry::TVec3f _1c; //
    JGeometry::TVec3f _28; //
    JGeometry::TVec3f _34; //
};
#endif // KARTCANNON_H