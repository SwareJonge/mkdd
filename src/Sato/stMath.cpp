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

void stClampVecMax(TVec3f &vec, float clampval)
{
    if (vec.length() > clampval)
    {
        stVecNormalize(vec);
        vec.scale((float)clampval);
    }
}

void stClampVecMin(TVec3f &vec, float clampval)
{
    if (vec.length() < clampval)
    {
        stVecNormalize(vec);
        vec.scale((float)clampval);
    }
}

// might use JMAFastSqrt instead, who knows
float stspeedy_sqrtf(register float x)
{
    register float recip;

    if(x > 0.0f) {
        __asm { frsqrte recip, x }
        return recip * x;
    }
    return x;
}

float stLength2(float x, float z)
{
    float x_square = x * x;
    float z_square = z * z;

    return stspeedy_sqrtf(x_square + z_square);
}

void stMakeDirectionMtx(TMtx34f *dirMtx, const TVec3f &vec, char axis) {
    TVec3f dir;
    TVec3f normal;
    
    if(axis == 'Z') {
        normal.set(0.0f, 1.0f, 0.0f);
    }
    else {
        normal.set(0.0f, 0.0f, 1.0f);
    }
    
    dir.cross(normal, vec);
    stVecNormalize(&dir);

    normal.cross(vec, dir);
    stVecNormalize(&normal);

    if (axis == 'Z') {
        dirMtx->mMtx[0][0] = dir.x;
        dirMtx->mMtx[1][0] = dir.y;
        dirMtx->mMtx[2][0] = dir.z;

        dirMtx->mMtx[0][1] = normal.x;
        dirMtx->mMtx[1][1] = normal.y;
        dirMtx->mMtx[2][1] = normal.z;

        dirMtx->mMtx[0][2] = vec.x;
        dirMtx->mMtx[1][2] = vec.y;
        dirMtx->mMtx[2][2] = vec.z;
    }
    else {
        dir.negate();
        dirMtx->mMtx[0][0] = dir.x;
        dirMtx->mMtx[1][0] = dir.y;
        dirMtx->mMtx[2][0] = dir.z;

        dirMtx->mMtx[0][1] = vec.x;
        dirMtx->mMtx[1][1] = vec.y;
        dirMtx->mMtx[2][1] = vec.z;

        dirMtx->mMtx[0][2] = normal.x;
        dirMtx->mMtx[1][2] = normal.y;
        dirMtx->mMtx[2][2] = normal.z;
    }

    dirMtx->mMtx[0][3] = 0.0f;
    dirMtx->mMtx[1][3] = 0.0f;
    dirMtx->mMtx[2][3] = 0.0f;
}

void stMakeRMtx(Mtx m, const TVec3f &vec1, const TVec3f &vec2, const TVec3f &vec3)
{
    m[0][0] = vec1.x;
    m[1][0] = vec1.y;
    m[2][0] = vec1.z;

    m[0][1] = vec2.x;
    m[1][1] = vec2.y;
    m[2][1] = vec2.z;

    m[0][2] = vec3.x;
    m[1][2] = vec3.y;
    m[2][2] = vec3.z;

    m[0][3] = m[1][3] = m[2][3] = 0.0f;
}

// Is it possible to actually clean this up?
void stQt2Mtx(Mtx m, const Quaternion *q)
{
    float qx = q->x;
    float qy = q->y;
    float qz = q->z;
    float qw = q->w;

    float qxqx = qx * qx;
    float qyqy = qy * qy;
    float qzqz = qz * qz;
    float qwqw = qw * qw;

    float length = (qxqx) + (qyqy) + (qzqz) + (qwqw);
    float normal = length == 0.0f ? 1.0f : 2.0f / length;

    float qx_normal = qx * normal;
    float qy_normal = qy * normal;
    float qz_normal = qz * normal;

    float qw_qx_normal = qw * qx_normal;
    float qw_qy_normal = qw * qy_normal;
    float qw_qz_normal = qw * qz_normal;
    float qx_squared   = qx * qx_normal;
    float qx_qy_normal = qx * qy_normal;
    float qx_qz_normal = qx * qz_normal;
    float qy_squared   = qy * qy_normal;
    float qy_qz_normal = qy * qz_normal;
    float qz_squared   = qz * qz_normal;

    m[0][0] = 1.0f - (qy_squared + qz_squared);
    m[1][0] = qx_qy_normal + qw_qz_normal;
    m[2][0] = qx_qz_normal - qw_qy_normal;

    m[0][1] = qx_qy_normal - qw_qz_normal;
    m[1][1] = 1.0f - (qz_squared + qx_squared);
    m[2][1] = qy_qz_normal + qw_qx_normal;

    m[0][2] = qw_qy_normal + qx_qz_normal;
    m[1][2] = qy_qz_normal - qw_qx_normal;
    m[2][2] = 1.0f - (qx_squared + qy_squared);

    m[0][3] = m[1][3] = m[2][3] = 0.0f;
}

// Unused
float stQtMult(Quaternion *dst, const Quaternion *p, const Quaternion *q) {

}

void stVec2QtUpdate(Quaternion &, Quaternion &, const TVec3f &, const TVec3f &){

}

void stVec2QtUpdate(Quaternion &q, const Vec &vec1, const Vec &vec2) {

}

bool stVec2Qt(Quaternion &q, const Vec &dir, const Vec &vec) {
    Vec cross;
    bool ret = false;
    PSVECCrossProduct(&dir, &vec, &cross);
    float dot = PSVECDotProduct(&dir, &vec);

    float length = stspeedy_sqrtf(2.0f * (1.0f + dot));

    if (length > 0.0f)
    {
        q.x = cross.x / length;
        q.y = cross.y / length;
        q.z = cross.z / length;
        q.w = 0.5f * length;
        ret = true;
    }

    return ret;
}

void stMtx2Qt(Quaternion *q, const Mtx m) {
    f32 x = m[0][0];
    f32 y = m[1][0];
    f32 z = m[2][0];
    f32 length = x * y * z;


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
        if (length < 0.0)
            t = -t;

        dst->w = negt * p->w + (f64)t * q->w;
        dst->x = negt * p->x + (f64)t * q->x;
        dst->y = negt * p->y + (f64)t * q->y;
        dst->z = negt * p->z + (f64)t * q->z;
    }
}

f32 stQtNormalize(Quaternion *dst, const Quaternion *q) {
    float length = (q->x * q->x + q->y * q->y + q->z * q->z + q->w * q->w);
    if (length != 0.0f)
    {
        float mag = 1.0f / std::sqrtf(length);

        dst->x = q->x * mag;
        dst->y = q->y * mag;
        dst->z = q->z * mag;
        dst->w = q->w * mag;
    }
    return length;
}

int stMakePlaneParam(stPlaneParam &planeParam, TVec3f &vec1, const TVec3f &vec2) {
    vec1.normalize();
    if (vec1.length() == 0.0f)
        return 0;
    
    planeParam.x = vec1.x;
    planeParam.y = vec1.y;
    planeParam.z = vec1.z;
    planeParam.angle = -vec1.dot(vec2);
    return 1;
}

int stMakePlaneParam(stPlaneParam &planeParam, const TVec3f &vec1, const TVec3f &vec2, const TVec3f &vec3)
{
    TVec3f crossp;

    crossp.cross(vec1, vec2);
    return stMakePlaneParam(planeParam, crossp, vec3);
}

void stSearchInSurface(const TVec3f &, const TVec3f &, const TVec3f &){

}

void stSearchInSurface(const TVec3f &, const stPlaneParam &) {

}

void stCollideSurfaceAndSphere(const TVec3f &, float, const stPlaneParam &, float &){

}

float stCollideLineToPlaneIn(const TVec3f &, const TVec3f &, const stPlaneParam &){

}

TVec3f stGetCollidePosFromT(const TVec3f &vec1, const TVec3f &vec2, float scalar)
{
    TVec3f pos;    
    TVec3f scaledpos;

    pos.sub(vec2, vec1);
    pos.scale(1.0f - scalar, pos);
    scaledpos.add(vec1, pos);

    return scaledpos;
}

float stGetCollideDepthFromT(const TVec3f &vec1, const TVec3f &vec2, float scalar)
{
    TVec3f depth;
    depth.sub(vec1, vec2);
    depth.scale(scalar);
    return depth.length();
}

// Unused, but i'm sure it was this
void stMTXRotS16(Mtx matrix, char axis, short v) {
    float sin = JMASinShort(v);
    float cos = JMASCosShort(v);
    PSMTXRotTrig(matrix, axis, sin, cos);
}

void stMTXRotDeg(Mtx matrix, char axis, float x)
{
    stMTXRotS16(matrix, axis, x * 182.0f);
}

void stMTXRotRad(Mtx matrix, char axis, float x)
{
    stMTXRotS16(matrix, axis, x * 10435.0f);
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
}

u32 stRandom::getRandomMax(u32 max) {
    return ((max + 1) * get_ufloat_1());
}

void stRandom::getArbitUnitVec(TVec3f& dst, float p2, float p3) {
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));
    _0x6 = p3 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    _0x8 = JMASSin(_0x4);

    // dst.set(_0x8 * JMASCos(_0x6), _0x8 * JMASSin(_0x6), JMASCos(_0x4));
    dst.x = _0x8 * JMASCos(_0x6);
    dst.y = _0x8 * JMASSin(_0x6);
    dst.z = JMASCos(_0x4);
}

void stRandom::getArbitUnitVecSimple(TVec3f &dst, float p2) {
    getArbitUnitVec(dst, p2, 1.0f);
}

void stRandom::getArbitUnitVecXZ(TVec3f& dst, float p2) {
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