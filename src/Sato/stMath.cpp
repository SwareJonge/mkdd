#include <dolphin/os.h>
#include "JSystem/JGeometry.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "std/math.h"
#include "Sato/stMath.h"
#include "types.h"

stRandom *stRandom::sRndMgr[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
stRandom *stRandom::sMgr;

namespace
{
    static double sLerpEpsilon = 0.0010000000474974513;
}

using namespace JGeometry;

int stVecNormalize(TVec3f &vec)
{
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
    return stVecNormalize(*vec);
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

    if (x > 0.0f)
    {
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

void stMakeDirectionMtx(TPos3f *dirMtx, const TVec3f &vec, char axis)
{
    TVec3f dir;
    TVec3f normal;

    if (axis == 'Z')
    {
        normal.set(0.0f, 1.0f, 0.0f);
    }
    else
    {
        normal.set(0.0f, 0.0f, 1.0f);
    }

    dir.cross(normal, vec);
    stVecNormalize(&dir);

    normal.cross(vec, dir);
    stVecNormalize(&normal);

    if (axis == 'Z')
    {
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
    else
    {
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
    float qx_squared = qx * qx_normal;
    float qx_qy_normal = qx * qy_normal;
    float qx_qz_normal = qx * qz_normal;
    float qy_squared = qy * qy_normal;
    float qy_qz_normal = qy * qz_normal;
    float qz_squared = qz * qz_normal;

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
void stQtMult(Quaternion *dst, const Quaternion *a, const Quaternion *b)
{
    f32 x = (a->x * b->w) + (a->w * b->x) + (a->y * b->z) - (a->z * b->y);
    f32 y = (a->y * b->w) + (a->w * b->y) + (a->z * b->x) - (a->x * b->z);
    f32 z = (a->z * b->w) + (a->w * b->z) + (a->x * b->y) - (a->y * b->x);
    f32 w = (a->w * b->w) - (a->x * b->x) - (a->y * b->y) - (a->z * b->z);
    dst->x = x;
    dst->y = y;
    dst->z = z;
    dst->w = w;
}

void stVec2QtUpdate(Quaternion &p, Quaternion &q, const TVec3f &dir, const TVec3f &vec)
{
    TVec3f crossp;
    crossp.cross(dir, vec);

    float length = stspeedy_sqrtf(2.0f * (1.0f + dir.dot(vec)));
    if (length > 0.0f)
    {
        Quaternion a;
        a.x = crossp.x / length;
        a.y = crossp.y / length;
        a.z = crossp.z / length;
        a.w = (0.5f * length);

        Quaternion c;
        stQtMult(&c, &a, &p);
        stQtNormalize(&q, &c);
    }
}

void stVec2QtUpdate(Quaternion &q, const Vec &dir, const Vec &vec)
{
    Vec cross;
    PSVECCrossProduct(&dir, &vec, &cross);
    float dot = PSVECDotProduct(&dir, &vec);

    float length = stspeedy_sqrtf(2.0f * (1.0f + dot));

    if (length > sLerpEpsilon)
    {
        Quaternion a;
        a.x = cross.x / length;
        a.y = cross.y / length;
        a.z = cross.z / length;
        a.w = (0.5f * length);

        Quaternion c;
        stQtMult(&c, &a, &q);

        stQtNormalize(&q, &c);
    }
}

bool stVec2Qt(Quaternion &q, const Vec &dir, const Vec &vec)
{
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

// TODO: match it in a way that makes sense
// https://decomp.me/scratch/2u8r1
void stMtx2Qt(Quaternion *q, const Mtx m)
{
    f32 x = m[0][0];
    f32 y = m[1][1];
    f32 z = m[2][2];
    f32 length = x + y + z;
    if (length > 0.0f)
    {
        length = std::sqrtf(1.0f + length);
        f32 t = 0.5f / length;
        q->w = 0.5f * length;
        q->x = t * (m[2][1] - m[1][2]);
        q->y = t * (m[0][2] - m[2][0]);
        q->z = t * (m[1][0] - m[0][1]);
    }
    else
    {
        f32 direction = x;
        if (y > x)
        {
            direction = m[1][1];
        }
        else if (z > x)
        {
            direction = m[2][2];
        }

        if (direction == x)
        {
            x = stspeedy_sqrtf(1.0f + (x - (y + z)));
            f32 t = 0.5f / x;
            q->x = 0.5f * x;
            q->y = t * (m[0][1] + m[1][0]);
            q->z = t * (m[2][0] + m[0][2]);
            q->w = t * (m[2][1] - m[1][2]);
        }
        else if (direction == y)
        {
            x = stspeedy_sqrtf(1.0f + (y - (z + x)));
            f32 t = 0.5f / x;
            q->y = 0.5f * x;
            q->z = t * (m[1][2] + m[2][1]);
            q->x = t * (m[0][1] + m[1][0]);
            q->w = t * (m[0][2] - m[2][0]);
        }
        else
        {
            x = stspeedy_sqrtf(1.0f + (z - (x + y)));
            f32 t = 0.5f / x;
            q->z = 0.5f * x;
            q->x = t * (m[2][0] + m[0][2]);
            q->y = t * (m[1][2] + m[2][1]);
            q->w = t * (m[1][0] - m[0][1]);
        }
    }
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

f32 stQtNormalize(Quaternion *dst, const Quaternion *q)
{
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

int stMakePlaneParam(stPlaneParam &planeParam, TVec3f &vec1, const TVec3f &vec2)
{
    vec1.normalize();
    if (vec1.length() == 0.0f)
        return 0;

    planeParam.x = vec1.x;
    planeParam.y = vec1.y;
    planeParam.z = vec1.z;
    planeParam.direction = -vec1.dot(vec2);
    return 1;
}

int stMakePlaneParam(stPlaneParam &planeParam, const TVec3f &vec1, const TVec3f &vec2, const TVec3f &vec3)
{
    TVec3f crossp;

    crossp.cross(vec1, vec2);
    return stMakePlaneParam(planeParam, crossp, vec3);
}

int stSearchInSurface(const TVec3f &vec1, const TVec3f &vec2, const TVec3f &vec3)
{
    int ret = 0;
    if (vec2.x * (vec1.x - vec3.x) + vec2.y * (vec1.y - vec3.y) + vec2.z * (vec1.z - vec3.z) <=
        0.0f)
    {
        ret = 1;
    }
    return ret;
}

int stSearchInSurface(const TVec3f &vec, const stPlaneParam &planeparam)
{
    int ret = 0;
    if (((planeparam.x * vec.x) + (planeparam.y * vec.y) + (planeparam.z * vec.z) + planeparam.direction) <= 0.0f)
    {
        ret = 1;
    }
    return ret;
}

int stCollideSurfaceAndSphere(const TVec3f &vec, float radius, const stPlaneParam &planeparam, float &retVal)
{
    int ret = 0;

    f32 length = ((planeparam.x * vec.x) + (planeparam.y * vec.y) + (planeparam.z * vec.z) + planeparam.direction);
    if (length > 0.0f)
    {
        if (length < radius)
        {
            ret = 1;
            retVal = radius - length;
        }
        else
        {
            retVal = -1.0f;
        }
    }
    else
    {
        ret = 1;
        retVal = radius - length;
    }

    return ret;
}

float stCollideLineToPlaneIn(const TVec3f &vec1, const TVec3f &vec2, const stPlaneParam &planeparam)
{
    TVec3f diff;
    TVec3f planePos;
    planePos.set(planeparam.x, planeparam.y, planeparam.z);

    diff.sub(vec2, vec1);

    if (planePos.dot(diff) >= 0.0f)
    {
        return -1.0f;
    }

    float f = planePos.x * vec1.x + planePos.y * vec1.y + planePos.z * vec1.z;
    float f2 = planePos.x * vec2.x + planePos.y * vec2.y + planePos.z * vec2.z;

    if (f == f2)
    {
        return -1.0f;
    }
    return -(f2 + planeparam.direction) / (f - f2);
}

TVec3f stGetCollidePosFromT(const TVec3f &vec1, const TVec3f &vec2, float scalar)
{
    TVec3f diff;
    TVec3f collidePos;

    diff.sub(vec2, vec1);
    diff.scale(1.0f - scalar, diff);
    collidePos.add(vec1, diff);

    return collidePos;
}

float stGetCollideDepthFromT(const TVec3f &vec1, const TVec3f &vec2, float scalar)
{
    TVec3f depth;
    depth.sub(vec1, vec2);
    depth.scale(scalar);
    return depth.length();
}

// Unused
void stMTXRotS16(Mtx matrix, char axis, short v)
{
    float sin = JMASinShort(v);
    float cos = JMACosShort(v);
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

void stRandom::createAllRandom()
{
    for (u32 i = 0; i < 6; i++)
    {
        sRndMgr[i] = new stRandom();
    }
}

// Unused nor auto inlined
void stRandom::deleteAllRandom()
{
    for (u32 i = 0; i < 6; i++)
    {
        delete sRndMgr[i];
    }
}

u32 stRandom::getRandomMax(u32 max)
{
    return ((max + 1) * get_ufloat_1());
}

void stRandom::getArbitUnitVec(TVec3f &dst, float p2, float p3)
{
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));
    _0x6 = p3 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    _0x8 = JMASSin(_0x4);

    // dst.set(_0x8 * JMASCos(_0x6), _0x8 * JMASSin(_0x6), JMASCos(_0x4));
    dst.x = _0x8 * JMASCos(_0x6);
    dst.y = _0x8 * JMASSin(_0x6);
    dst.z = JMASCos(_0x4);
}

void stRandom::getArbitUnitVecSimple(TVec3f &dst, float p2)
{
    getArbitUnitVec(dst, p2, 1.0f);
}

void stRandom::getArbitUnitVecXZ(TVec3f &dst, float p2)
{
    _0x4 = p2 * (32767.0f * (2.0f * get_ufloat_1() - 1.0f));

    // dst.set(JMASSin(_0x4), 0.0f, JMASCos(_0x4));
    dst.x = JMASSin(_0x4);
    dst.y = 0.0;
    dst.z = JMASCos(_0x4);
}

// Unused
stRandom *stGetRnd()
{
    return stRandom::sMgr;
}

stRandom *stGetRnd(u32 idx)
{
#line 924
    JUT_ASSERT_F(stRandom::sRndMgr[idx]->permission, "Random can\'t get :%d", idx);
    return stRandom::sRndMgr[idx];
}

void stSetRndPermission(u32 idx, bool perm)
{
    stRandom::sRndMgr[idx]->permission = perm;
}
