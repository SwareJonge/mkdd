#include <dolphin/os.h>
#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "std/math.h"
#include "Sato/stMath.h"
#include "types.h"

stRandom *stRandom::sRndMgr[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
stRandom *stRandom::sMgr;

namespace {
    static double sLerpEpsilon = 0.0010000000474974513;
}

int stVecNormalize(TVec3f &vec) {
    int ret = 0;

    if (!vec.isZero())
    {
        vec.normalize();
        ret = 1;
    }

    return ret;
}

int stVecNormalize(TVec3f *vec)
{
    int ret = 0;

    if (!vec->isZero())
    {
        vec->normalize();
        ret = 1;
    }

    return ret;
}

void stClampVecMax(TVec3f &vec, f32 clampval)
{
    if (vec.length() > clampval)
    {
        stVecNormalize(vec);
        vec.scale((float)clampval);
    }
}

void stClampVecMin(TVec3f &vec, f32 clampval)
{
    if (vec.length() < clampval)
    {
        stVecNormalize(vec);
        vec.scale((f32)clampval);
    }
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

void stMakeDirectionMtx(TMtx34f *, const TVec3f &, char) {

}

void stMakeRMtx(Mtx mtx, const TVec3f &vec1, const TVec3f &vec2, const TVec3f &vec3)
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

    mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0f;
}

// Is it possible to actually clean this up?
void stQt2Mtx(Mtx mat, const Quaternion *q)
{
    f32 qx = q->x;
    f32 qy = q->y;
    f32 qz = q->z;
    f32 qw = q->w;

    f32 qxqx = qx * qx;
    f32 qyqy = qy * qy;
    f32 qzqz = qz * qz;
    f32 qwqw = qw * qw;

    f32 length = (qxqx) + (qyqy) + (qzqz) + (qwqw);
    f32 normal = length == 0.0f ? 1.0f : 2.0f / length;

    f32 qx_normal = qx * normal;
    f32 qy_normal = qy * normal;
    f32 qz_normal = qz * normal;

    f32 qw_qx_normal = qw * qx_normal;
    f32 qw_qy_normal = qw * qy_normal;
    f32 qw_qz_normal = qw * qz_normal;
    f32 qx_squared   = qx * qx_normal;
    f32 qx_qy_normal = qx * qy_normal;
    f32 qx_qz_normal = qx * qz_normal;
    f32 qy_squared   = qy * qy_normal;
    f32 qy_qz_normal = qy * qz_normal;
    f32 qz_squared   = qz * qz_normal;

    mat[0][0] = 1.0f - (qy_squared + qz_squared);
    mat[1][0] = qx_qy_normal + qw_qz_normal;
    mat[2][0] = qx_qz_normal - qw_qy_normal;

    mat[0][1] = qx_qy_normal - qw_qz_normal;
    mat[1][1] = 1.0f - (qz_squared + qx_squared);
    mat[2][1] = qy_qz_normal + qw_qx_normal;

    mat[0][2] = qw_qy_normal + qx_qz_normal;
    mat[1][2] = qy_qz_normal - qw_qx_normal;
    mat[2][2] = 1.0f - (qx_squared + qy_squared);

    mat[0][3] = mat[1][3] = mat[2][3] = 0.0f;
}

void stVec2QtUpdate(Quaternion &, Quaternion &, const TVec3f &, const TVec3f &){

}

void stVec2QtUpdate(Quaternion &q, const Vec &vec1, const Vec &vec2) {

}

void stVec2Qt(Quaternion &, const Vec &, const Vec &) {

}

void stMtx2Qt(Quaternion *, const Mtx) {

}

void stQtLerp(Quaternion *dst, const Quaternion *p, const Quaternion *q, float t)
{
    f64 negt = 1.0f - t;
    if (t >= 1.0f)
    {
        dst->x = q->x;
        dst->y = q->y;
        dst->z = q->z;
        dst->w = q->w;
    }
    else
    {
        f64 length = p->x * q->x + p->y * q->y + p->z * q->z + p->w * q->w;
        if (length < 0.0f)
            t = -t;

        dst->w = negt * p->w + (f64)t * q->w;
        dst->x = negt * p->x + (f64)t * q->x;
        dst->y = negt * p->y + (f64)t * q->y;
        dst->z = negt * p->z + (f64)t * q->z;
    }
}

// regswaps
void stQtNormalize(Quaternion *dst, const Quaternion *q) {

    f32 length = q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w;
    if (length != 0.0f) {
        f32 invlength = std::inv_sqrtf(length);

        dst->x = q->x * invlength;
        dst->y = q->y * invlength;
        dst->z = q->z * invlength;
        dst->w = q->w * invlength;
    }
}

void stMakePlaneParam(stPlaneParam &, TVec3f &, const TVec3f &) {

}

void stMakePlaneParam(stPlaneParam &, const TVec3f &, const TVec3f &, const TVec3f &) {

}

void stSearchInSurface(const TVec3f &, const TVec3f &, const TVec3f &){

}

void stSearchInSurface(const TVec3f &, const stPlaneParam &) {

}

void stCollideSurfaceAndSphere(const TVec3f &, float, const stPlaneParam &, float &){

}

f32 stCollideLineToPlaneIn(const TVec3f &, const TVec3f &, const stPlaneParam &){
}

// not matching
TVec3f stGetCollidePosFromT(const TVec3f &vec1, const TVec3f &vec2, float scalar)
{
    TVec3f pos;
    pos.sub(vec2, vec1);
    pos.scale(1.0f - scalar, pos);
    // setTVec3f((const f32 *)&pos, (f32 *)&vec1);

    return pos + vec1;
}
f32 stGetCollideDepthFromT(const TVec3f &vec1, const TVec3f &vec2, float scalar)
{
    TVec3f depth;
    depth.sub(vec1, vec2);
    depth.scale(scalar);
    return depth.length();
}

// Unused, but i'm sure it was this
void stMTXRotS16(Mtx matrix, char p2, short v) {
    f32 sin = JMASinShort(v);
    f32 cos = JMASCosShort(v);
    PSMTXRotTrig(matrix, p2, sin, cos);
}

void stMTXRotDeg(Mtx matrix, char p2, float x)
{
    stMTXRotS16(matrix, p2, x * 182.0f);
}

void stMTXRotRad(Mtx matrix, char p2, float x)
{
    stMTXRotS16(matrix, p2, x * 10435.0f);
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

void stRandom::getArbitUnitVec(TVec3f& dst, f32 p2, f32 p3) {
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));
    _0x6 = p3 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    _0x8 = JMASSin(_0x4);

    // dst.set(_0x8 * JMASCos(_0x6), _0x8 * JMASSin(_0x6), JMASCos(_0x4));
    dst.x = _0x8 * JMASCos(_0x6);
    dst.y = _0x8 * JMASSin(_0x6);
    dst.z = JMASCos(_0x4);
}

void stRandom::getArbitUnitVecSimple(TVec3f &dst, f32 p2) {
    getArbitUnitVec(dst, p2, 1.0f);
}

void stRandom::getArbitUnitVecXZ(TVec3f& dst, f32 p2) {
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