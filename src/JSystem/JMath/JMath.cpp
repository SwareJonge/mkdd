//#include "dolphin/vec.h"
#include "dolphin/mtx.h"
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

#if NO_PS // used in TP Debug
void JMAMTXApplyScale(const Mtx src, Mtx dst, f32 xS, f32 yS, f32 zS) {
    Mtx scale;
    MTXScale(scale, xScale, yScale, zScale);
    MTXConcat(src, scale, dst);
}
#else

void JMAMTXApplyScale(register const Mtx src, register Mtx dst, 
register f32 xScale, register f32 yScale, register f32 zScale)
{    
    register f32 scale, x, y, z;
    register f32 normal = 1.0f;
    // TODO: clean up, reorder some instructions and provide documentation
    __asm {
        psq_l x, 0(src), 0, 0
        psq_l y, 0x10(src), 0, 0
        ps_merge00 scale, xScale, yScale
        psq_l z, 0x20(src), 0, 0
        ps_mul x, x, scale
        ps_mul y, y, scale
        ps_mul z, z, scale
        psq_st x, 0(dst), 0, 0
        ps_merge00 scale, zScale, normal
        psq_l x, 0x8(src), 0, 0
        psq_st y, 0x10(dst), 0, 0
        psq_l y, 0x18(src), 0, 0
        ps_mul x, x, scale
        psq_st z, 0x20(dst), 0, 0
        psq_l z, 0x28(src), 0, 0
        ps_mul y, y, scale
        psq_st x, 0x8(dst), 0, 0
        ps_mul z, z, scale
        psq_st y, 0x18(dst), 0, 0
        psq_st z, 0x28(dst), 0, 0
    }
}

#endif