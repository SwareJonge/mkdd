#include <dolphin/os.h>
#include "JSystem/JGeometry.h"
#include "JSystem/JMath/JMath.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "Sato/stMath.h"
#include "types.h"

void stRandom::createAllRandom() {
    for (u32 i = 0; i < 6; i++) {
        sRndMgr[i] = new stRandom();
    }
}

u32 stRandom::getRandomMax(u32 max) {
    return ((max + 1) * get_ufloat_1());
}

void stRandom::getArbitUnitVec(JGeometry::TVec3<f32>& dst, f32 p2, f32 p3) {
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));
    _0x6 = p3 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    _0x8 = JMASSin(_0x4);

    dst.x = _0x8 * JMASCos(_0x6);
    dst.y = _0x8 * JMASSin(_0x6);
    dst.z = JMASCos(_0x4);
}

// inline auto
void stRandom::getArbitUnitVecSimple(JGeometry::TVec3<f32> &dst, f32 p2) {
    getArbitUnitVec(dst, p2, 1.0f);
}

void stRandom::getArbitUnitVecXZ(JGeometry::TVec3<f32>& dst, f32 p2) {
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    dst.x = JMASSin(_0x4);
    dst.y = 0.0;
    dst.z = JMASCos(_0x4);
}

stRandom *stGetRnd() {
    return stRandom::sMgr;
}

stRandom *stGetRnd(u32 idx) {
    JUT_ASSERT_F(924, stRandom::sRndMgr[idx]->permission, "Random can\'t get :%d", idx);
    return stRandom::sRndMgr[idx];
}

void stSetRndPermission(u32 idx, bool perm) {
    stRandom::sRndMgr[idx]->permission = perm;
}