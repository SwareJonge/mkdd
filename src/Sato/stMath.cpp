#include <dolphin/os.h>
#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "Sato/stMath.h"
#include "types.h"

stRandom *stRandom::sRndMgr[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
stRandom *stRandom::sMgr;

namespace {
    static double sLerpEpsilon = 0.0010000000474974513;
}

int stVecNormalize(TVec3<float> &vec) {
    int ret = 0;

    if (!vec.isZero())
    {
        vec.normalize();
        ret = 1;
    }

    return ret;
}

int stVecNormalize(TVec3<float> *vec)
{
    int ret = 0;

    if (!vec->isZero())
    {
        vec->normalize();
        ret = 1;
    }

    return ret;
}

void stClampVecMax(TVec3<float> &, float max) {

}

void stClampVecMin(TVec3<float> &, float min) {

}

// might use JMAFastSqrt instead, who knows
f32 stspeedy_sqrtf(register f32 x)
{
    register f32 recip;

    if(x > 0.0f) {
        asm {
            frsqrte recip, x
        }
        return recip * x;
    }
    return x;
}

f32 stLength2(float x, float z)
{
    f32 x_square = x * x;
    f32 z_square = z * z;

    return stspeedy_sqrtf(x_square + z_square);
}

void stMakeDirectionMtx(TMtx34f *, const TVec3<float> &, char) {

}

void stMakeRMtx(Mtx mtx, const TVec3<float> &vec1, const TVec3<float> &vec2, const TVec3<float> &vec3)
{
    mtx[0][0] = vec1.x;
    mtx[1][0] = vec1.y;
    mtx[2][0] = vec1.z;

    mtx[0][1] = vec2.x;
    mtx[1][1] = vec2.y;
    mtx[2][1] = vec2.z;

    mtx[0][2] = vec3.x;
    mtx[1][2] = vec3.y;
    mtx[2][2] = vec3.z;

    mtx[2][3] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[0][3] = 0.0f;
}

void stQt2Mtx(Mtx, const Quaternion *){

}

void stVec2QtUpdate(Quaternion &, Quaternion &, const TVec3<float> &, const TVec3<float> &){

}

void stVec2QtUpdate(Quaternion &, const Vec &, const Vec &) {

}

void stVec2Qt(Quaternion &, const Vec &, const Vec &) {

}

void stMtx2Qt(Quaternion *, const Mtx) {

}

void stQtLerp(Quaternion *, const Quaternion *, const Quaternion *, float) {

}

void stQtNormalize(Quaternion *, const Quaternion *){

}

void stMakePlaneParam(stPlaneParam &, TVec3<float> &, const TVec3<float> &) {

}

void stMakePlaneParam(stPlaneParam &, const TVec3<float> &, const TVec3<float> &, const TVec3<float> &) {

}

void stSearchInSurface(const TVec3<float> &, const TVec3<float> &, const TVec3<float> &){

}

void stSearchInSurface(const TVec3<float> &, const stPlaneParam &) {

}

void stCollideSurfaceAndSphere(const TVec3<float> &, float, const stPlaneParam &, float &){

}

void stCollideLineToPlaneIn(const TVec3<float> &, const TVec3<float> &, const stPlaneParam &){

}

void stGetCollidePosFromT(const TVec3<float> &, const TVec3<float> &, float){

}

void stGetCollideDepthFromT(const TVec3<float> &, const TVec3<float> &, float){

}

// Unused, but i'm sure it was this
void stMTXRotS16(Mtx matrix, char p2, short v) {
    f32 sin = JMASinShort(v);
    f32 cos = JMASCosShort(v);
    PSMTXRotTrig(matrix, p2, sin, cos);
}

void stMTXRotDeg(Mtx matrix, char p2, float x)
{
    f32 deg = x * 182.0f;
    stMTXRotS16(matrix, p2, deg);
}

void stMTXRotRad(Mtx matrix, char p2, float x)
{
    f32 rad = x * 10435.0f;
    stMTXRotS16(matrix, p2, rad);
}

void stRandom::createAllRandom() {
    for (u32 i = 0; i < 6; i++) {
        sRndMgr[i] = new stRandom();
    }
}

// Unused, however most likely is something like this
void stRandom::deleteAllRandom()
{
    for (u32 i = 0; i < 6; i++) {
        delete sRndMgr[i];
    }
    delete sMgr;
}

u32 stRandom::getRandomMax(u32 max) {
    return ((max + 1) * get_ufloat_1());
}

void stRandom::getArbitUnitVec(TVec3<f32>& dst, f32 p2, f32 p3) {
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));
    _0x6 = p3 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    _0x8 = JMASSin(_0x4);

    // dst.set(_0x8 * JMASCos(_0x6), _0x8 * JMASSin(_0x6), JMASCos(_0x4));
    dst.x = _0x8 * JMASCos(_0x6);
    dst.y = _0x8 * JMASSin(_0x6);
    dst.z = JMASCos(_0x4);
}

void stRandom::getArbitUnitVecSimple(TVec3<f32> &dst, f32 p2) {
    getArbitUnitVec(dst, p2, 1.0f);
}

void stRandom::getArbitUnitVecXZ(TVec3<f32>& dst, f32 p2) {
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    // dst.set(JMASSin(_0x4), 0.0f, JMASCos(_0x4));
    dst.x = JMASSin(_0x4);
    dst.y = 0.0;
    dst.z = JMASCos(_0x4);
}

// Unused
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