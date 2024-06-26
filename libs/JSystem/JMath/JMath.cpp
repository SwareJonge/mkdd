#include "JSystem/JMath/JMath.h"
#include "types.h"

void JMAEulerToQuat(s16 roll, s16 pitch, s16 yaw, Quaternion *q)
{
    f32 cr = JMASCos(roll / 2);
    f32 cp = JMASCos(pitch / 2);
    f32 cy = JMASCos(yaw / 2);

    f32 sr = JMASSin(roll / 2);
    f32 sp = JMASSin(pitch / 2);
    f32 sy = JMASSin(yaw / 2);

    f32 cpy = (cp * cy);
    f32 spy = (sp * sy);

    q->w = cr * cpy + sr * spy;
    q->x = sr * cpy - cr * spy;
    q->y = cy * (cr * sp) + sy * (sr * cp);
    q->z = sy * (cr * cp) - cy * (sr * sp);
}

void JMAQuatLerp(register const Quaternion *p, register const Quaternion *q, f32 t, Quaternion *dst) 
{
    register f32 pxy, pzw, qxy, qzw;
    register f32 dp;
    __asm // compute dot product
    {
        // clang-format off
        psq_l       pxy, 0(p), 0, 0
        psq_l       qxy, 0(q), 0, 0
        ps_mul      dp, pxy, qxy
        
        psq_l       pzw, 8(p), 0, 0
        psq_l       qzw, 8(q), 0, 0
        ps_madd     dp, pzw, qzw, dp
        
        ps_sum0     dp, dp, dp, dp
        // clang-format on
    }

    if (dp < 0.0)
    {
        dst->x = -t * (p->x + q->x) + p->x;
        dst->y = -t * (p->y + q->y) + p->y;
        dst->z = -t * (p->z + q->z) + p->z;
        dst->w = -t * (p->w + q->w) + p->w;
    }
    else {
        dst->x = -t * (p->x - q->x) + p->x;
        dst->y = -t * (p->y - q->y) + p->y;
        dst->z = -t * (p->z - q->z) + p->z;
        dst->w = -t * (p->w - q->w) + p->w;
    }
}

void JMAFastVECNormalize(register const Vec *src, register Vec *dst)
{
    register f32 vxy, rxy, vz, length;
    __asm {
        // clang-format off
        psq_l vxy, 0(src), 0, 0
        ps_mul rxy, vxy, vxy
        lfs vz, src->z
        ps_madd length, vz, vz, rxy
        ps_sum0 length, length, rxy, rxy
        frsqrte length, length
        ps_muls0 vxy, vxy, length;
        psq_st vxy, 0(dst), 0, 0
        fmuls vz, vz, length
        stfs vz, dst->z
        // clang-format on
    }
}

void JMAVECScaleAdd(register const Vec *vec1, register const Vec *vec2, register Vec *dst, register f32 scale)
{
    register f32 v1xy, v2xy, rxy, v1z, v2z, rz;
    // clang-format off
    __asm {
        // load vector XY of vector 1
        psq_l v1xy, 0(vec1), 0, 0
        // load vector Z of vector 2
        psq_l v2xy,  0(vec2), 0, 0
        // load vector Z of vector 1
        psq_l v1z,   8(vec1), 1, 0
        // load vector Z of vector 2
        psq_l v2z,   8(vec2), 1, 0
        // multiply vector 1 XY and add vector 2 XY
        ps_madds0    rxy, v1xy, scale, v2xy
        // multiply vector 1 Z and add  Vector 2 Z
        ps_madds0 rz, v1z,  scale, v2z
        // store result XY in dst
        psq_st rxy, 0(dst), 0, 0
        // store result Z in dst
        psq_st rz,  8(dst), 1, 0
    }
    // clang-format on
}

void JMAVECLerp(register const Vec *vec1, register const Vec *vec2, register Vec *dst, register f32 t)
{
    register f32 v1xy, v2xy, v1z, v2z;
    // clang-format off
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
    // clang-format on

    /*dst->x = (vec2->x - vec1->x) * t + vec1->x;
    dst->y = (vec2->y - vec1->y) * t + vec1->y;
    dst->z = (vec2->z - vec1->z) * t + vec1->z;*/
}

#if NO_PS // used in TP Debug
void JMAMTXApplyScale(const Mtx src, Mtx dst, f32 xScale, f32 yScale, f32 zScale) {
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
    // clang-format off
    __asm {
        // scale first 2 components
        psq_l x, 0(src), 0, 0
        psq_l y, 0x10(src), 0, 0
        psq_l z, 0x20(src), 0, 0
        ps_merge00 scale, xScale, yScale        
        ps_mul x, x, scale
        ps_mul y, y, scale
        ps_mul z, z, scale        
        psq_st x, 0(dst), 0, 0        
        psq_st y, 0x10(dst), 0, 0
        psq_st z, 0x20(dst), 0, 0

        // scale last 2 components
        psq_l x, 0x8(src), 0, 0
        psq_l y, 0x18(src), 0, 0       
        psq_l z, 0x28(src), 0, 0
        ps_merge00 scale, zScale, normal
        ps_mul x, x, scale
        ps_mul y, y, scale        
        ps_mul z, z, scale
        psq_st x, 0x8(dst), 0, 0
        psq_st y, 0x18(dst), 0, 0
        psq_st z, 0x28(dst), 0, 0
    }
    // clang-format on

    /*
    dst[0][0] = src[0][0] * xScale;
    dst[0][1] = src[0][1] * yScale;
    dst[0][2] = src[0][2] * zScale;

    dst[1][0] = src[1][0] * xScale;
    dst[1][1] = src[1][1] * yScale;
    dst[1][2] = src[1][2] * zScale;

    dst[2][0] = src[2][0] * zScale;
    dst[2][1] = src[2][1] * zScale;
    dst[2][2] = src[2][2] * zScale;

    dst[3][0] = src[3][0] * normal;
    dst[3][1] = src[3][1] * normal;
    dst[3][2] = src[3][2] * normal;
    */
}

#endif