//#include "dolphin/vec.h"
#include "JSystem/JMath/JMath.h"
#include "types.h"

extern "C" {
    #include <ppcdis.h>
} 

void JMAEulerToQuat(s16 x, s16 y, s16 z, Quaternion *quat)
{
    f32 cosX = JMASCos(x / 2);
    f32 cosY = JMASCos(y / 2);
    f32 cosZ = JMASCos(z / 2);
    f32 sinX = JMASSin(x / 2);
    f32 sinY = JMASSin(y / 2);
    f32 sinZ = JMASSin(z / 2);

    quat->w = cosX * (cosY * cosZ) + sinX * (sinY * sinZ);
    quat->x = sinX * (cosY * cosZ) - cosX * (sinY * sinZ);
    quat->y = cosZ * (cosX * sinY) + sinZ * (sinX * cosY);
    quat->z = sinZ * (cosX * cosY) - cosZ * (sinX * sinY);
}

void JMAQuatLerp(const Quaternion *a, const Quaternion *b, f32 t, Quaternion *dst)
{
    if (JMathInlineQuat::PSQUATDotProduct(a, b) < 0) // fabricated, gonna be mad if asm was just inserted here
    {
        dst->x = -t * (a->x + b->x) + a->x;
        dst->y = -t * (a->y + b->y) + a->y;
        dst->z = -t * (a->z + b->z) + a->z;
        dst->w = -t * (a->w + b->w) + a->w;
        return;
    }
    dst->x = -t * (a->x - b->x) + a->x;
    dst->y = -t * (a->y - b->y) + a->y;
    dst->z = -t * (a->z - b->z) + a->z;
    dst->w = -t * (a->w - b->w) + a->w;
}

void JMAVECScaleAdd(register const Vec *vec1, register const Vec *vec2, register Vec *dst, register f32 scale)
{
    register f32 v1xy, v2xy, rxy, v1z, v2z, rz;
    __asm {
        // load vector XY of vector 1
        psq_l v1xy, 0(vec1), 0, 0
        // load vector Z of vector 2
        psq_l v2xy,  0(vec2), 0, 0
        // load vector Z of vector 1
        psq_l v1z,   8(vec1), 1, 0
        // multiply vector 1 XY and add vector 2 XY
        ps_madds0    rxy, v1xy, scale, v2xy
        // load vector Z of vector 2
        psq_l v2z,   8(vec2), 1, 0
        // multiply vector 1 Z and add  Vector 2 Z
        ps_madds0 rz, v1z,  scale, v2z
        // store result XY in dst
        psq_st rxy, 0(dst), 0, 0
        // store result Z in dst
        psq_st rz,  8(dst), 1, 0
    }
}

void JMAVECLerp(register const Vec *vec1, register const Vec *vec2, register Vec *dst, register f32 t)
{
    register f32 v1xy, v2xy, v1z, v2z;
    __asm {
        // load XY components of both vectors
        psq_l v1xy, 0(vec1), 0, 0
        psq_l v2xy, 0(vec2), 0, 0
        // subtract xy components of vec2 with vec1
        ps_sub v2xy, v2xy, v1xy
        // multiply result with t and subtract z of vec1
        ps_madds0 v2xy, v2xy, t, v1xy
        // store result xy in dst
        psq_st v2xy, 0(dst), 0, 0
        
        // load Z components of both vectors
        lfs v1z, 8(vec1)
        lfs v2z, 8(vec2)
        // subtract z component of vec2 with vec1
        fsubs v2z, v2z, v1z
        // multiply result with t and subtract z of vec1
        fmadds v2z, v2z, t, v1z
        // store result z in dst
        stfs v2z, 8(dst)
    }
    /*dst->x = (vec2->x - vec1->x) * t + vec1->x;
    dst->y = (vec2->y - vec1->y) * t + vec1->y;
    dst->z = (vec2->z - vec1->z) * t + vec1->z;*/
}

#if ADDED_1_2_5
void JMAMTXApplyScale(const Mtx src, Mtx dst, f32 x, f32 y, f32 z) {
    Mtx scale;
    PSMTXScale(scale, x, y, z);
    PSMTXConcat(src, scale, dst);
}
#else

#include "orderfloats/80417790_80417794.inc"

asm void JMAMTXApplyScale(const Mtx src, Mtx dst, f32 x, f32 y, f32 z)
{
#include "asm/80069364.s"
}

#endif