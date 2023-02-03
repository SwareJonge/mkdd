#ifndef STMATH_H
#define STMATH_H

#include "types.h"

#include "JSystem/JMath/JMath.h"
#include "JSystem/JGeometry.h"

struct stRandom : public JMath::TRandom_fast_ {
    stRandom() : JMath::TRandom_fast_(0) {
        permission = true;
        sMgr = this;
    };

    static void createAllRandom();
    u32 getRandomMax(u32 max);
    void getArbitUnitVec(JGeometry::TVec3<f32> &, f32, f32);
    void getArbitUnitVecSimple(JGeometry::TVec3<f32> &, f32);
    void getArbitUnitVecXZ(JGeometry::TVec3<f32>&, f32);

    static stRandom *sRndMgr[6];
    static stRandom *sMgr; // not sure if this get's accesssed by something else, more research needed

    s16 _0x4; // sin
    s16 _0x6; // cos
    f32 _0x8; // angle?
    u8 _0xC[4]; // unknown
    u32 randomSeed;
    bool permission;
    // padding bytes
};

stRandom * stGetRnd(u32 idx);
void stSetRndPermission(u32 idx, bool perm);

#endif // !STMATH_H
