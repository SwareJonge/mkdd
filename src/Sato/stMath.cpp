#include "types.h"
#include "dolphin/OS.h"
#include "JSystem/JGeometry.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "Sato/stMath.h"

void stRandom::createAllRandom() {
    for (u32 i = 0; i < 6; i++) {
        stRandom::sRndMgr[i] = new stRandom();
    }
}

u32 stRandom::getRandomMax(u32 max) {
    return ((max + 1) * get_ufloat_1());
}

f32 stRandom::getArbitUnitVec(JGeometry::TVec3<f32>& p1, f32 p2, f32 p3) {
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));
    _0x6 = p3 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    _0x8 = JMath::sincosTable_.sinShort(_0x4);
    // according to ghidra it shifts 2 bytes to the right and doe an AND operation with 0x3ff8
    p1.x = _0x8 * JMath::sincosTable_.cosShort(_0x6);
    f32 ret = _0x8;
    p1.y = _0x8 * JMath::sincosTable_.sinShort(_0x6);
    p1.z = JMath::sincosTable_.cosShort(_0x4);
    return ret;
}

f32 stRandom::getArbitUnitVecSimple(JGeometry::TVec3<f32>& p1, f32 p2) {
    f32 _1f = 1.0f; // for some reason this has to be declared as a variable

    _0x4 = p2 * (2.0f * (32767.0f * get_ufloat_1() - _1f));
    _0x6 = _1f * (2.0f * (32767.0f *  get_ufloat_1() - _1f));
    _0x8 = JMath::sincosTable_.sinShort(_0x4);

    p1.x = _0x8 * JMath::sincosTable_.cosShort(_0x6);
    f32 ret = _0x8;
    p1.y = _0x8 * JMath::sincosTable_.sinShort(_0x6);
    p1.z = JMath::sincosTable_.cosShort(_0x4);
    return ret;
}

f32 stRandom::getArbitUnitVecXZ(JGeometry::TVec3<f32>& p1, f32 p2) {
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    f32 ret = JMath::sincosTable_.sinShort(_0x4);
    p1.x = ret;
    p1.y = 0.0;
    p1.z = JMath::sincosTable_.cosShort(_0x4);
    return ret;
}

stRandom *stGetRnd(u32 idx) {
    JUT_ASSERT_F(924, stRandom::sRndMgr[idx]->permission, "Random can\'t get :%d", idx);
    return stRandom::sRndMgr[idx];
}

void stSetRndPermission(u32 idx, bool perm) {
    stRandom::sRndMgr[idx]->permission = perm; // i'm actually not sure at all if this is inlined, 
}