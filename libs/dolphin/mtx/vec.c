#include "types.h"
#include "dolphin/mtx.h"

#define R_RET fp1
#define FP2 fp2
#define FP3 fp3
#define FP4 fp4
#define FP5 fp5
#define FP6 fp6
#define FP7 fp7
#define FP8 fp8
#define FP9 fp9
#define FP10 fp10
#define FP11 fp11
#define FP12 fp12
#define FP13 fp13

void C_VECAdd(void)
{
    // UNUSED FUNCTION
}

ASM void PSVECAdd(const register Vec *vec1, const register Vec *vec2, register Vec *ret)
{
#ifdef __MWERKS__ // clang-format off
    nofralloc;
    psq_l     FP2,  0(vec1), 0, 0;
    psq_l     FP4,  0(vec2), 0, 0;
    ps_add    FP6, FP2, FP4;
    psq_st    FP6,  0(ret), 0, 0;
    psq_l     FP3,   8(vec1), 1, 0;
    psq_l     FP5,   8(vec2), 1, 0;
    ps_add    FP7, FP3, FP5;
    psq_st    FP7,   8(ret), 1, 0;
    blr
#endif // clang-format on
}

void C_VECSubtract(void)
{
    // UNUSED FUNCTION
}

ASM void PSVECSubtract(const register Vec *vec1, const register Vec *vec2, register Vec *ret)
{
#ifdef __MWERKS__ // clang-format off
    nofralloc;
    psq_l     FP2,  0(vec1), 0, 0;
    psq_l     FP4,  0(vec2), 0, 0;
    ps_sub    FP6, FP2, FP4;
    psq_st    FP6, 0(ret), 0, 0;
    psq_l     FP3,   8(vec1), 1, 0;
    psq_l     FP5,   8(vec2), 1, 0;
    ps_sub    FP7, FP3, FP5;
    psq_st    FP7,  8(ret), 1, 0;
    blr
#endif // clang-format on
}

void C_VECScale(void)
{
    // UNUSED FUNCTION
}

void PSVECScale(register const Vec *src, register Vec *dst, register f32 scale)
{
    // clang-format off
    register f32 vxy, vz, rxy, rz;
    __asm {		
        psq_l       vxy, 0(src), 0, 0
        psq_l       vz,  8(src), 1, 0
        ps_muls0    rxy, vxy, scale
        psq_st      rxy, 0(dst), 0, 0
        ps_muls0    rz,  vz,  scale                        
        psq_st      rz,  8(dst), 1, 0
    }
    // clang-format on
}

void C_VECNormalize(void)
{
    // UNUSED FUNCTION
}

void PSVECNormalize(const register Vec *vec1, register Vec *ret)
{
#ifdef __MWERKS__ // clang-format off
    register f32 half  = 0.5f;
    register f32 three = 3.0f;
    register f32 xx_zz, xx_yy;
    register f32 square_sum;
    register f32 ret_sqrt;
    register f32 n_0, n_1;
    asm {
        psq_l       FP2, 0(vec1), 0, 0;
        ps_mul      xx_yy, FP2, FP2;
        psq_l       FP3, 8(vec1), 1, 0;
        ps_madd     xx_zz, FP3, FP3, xx_yy;
        ps_sum0     square_sum, xx_zz, FP3, xx_yy;
        frsqrte     ret_sqrt, square_sum;
        fmuls       n_0, ret_sqrt, ret_sqrt;
        fmuls       n_1, ret_sqrt, half;
        fnmsubs     n_0, n_0, square_sum, three;
        fmuls       ret_sqrt, n_0, n_1;
        ps_muls0    FP2, FP2, ret_sqrt;
        psq_st      FP2, 0(ret), 0, 0;
        ps_muls0    FP3, FP3, ret_sqrt;
        psq_st      FP3, 8(ret), 1, 0;
    }
#endif // clang-format on
}

void C_VECSquareMag(void)
{
    // UNUSED FUNCTION
}

f32 PSVECSquareMag(const Vec *v)
{
    // UNUSED FUNCTION
}

void C_VECMag(void)
{
    // UNUSED FUNCTION
}

f32 PSVECMag(const register Vec *v)
{
    register f32 v_xy, v_zz, square_mag;
    register f32 ret_mag, n_0, n_1;
    register f32 three, half, zero;
    half = 0.5f;
    #ifdef __MWERKS__ // clang-format off
    asm {
        psq_l       v_xy, 0(v), 0, 0
        ps_mul      v_xy, v_xy, v_xy
        lfs         v_zz, 8(v)
        fsubs       zero, half, half
        ps_madd     square_mag, v_zz, v_zz, v_xy
        ps_sum0     square_mag, square_mag, v_xy, v_xy
        fcmpu       cr0, square_mag, zero
        beq-        __exit
        frsqrte     ret_mag, square_mag
    }
    #endif // clang-format on
    three = 3.0f;
    #ifdef __MWERKS__ // clang-format off
    asm {
        fmuls       n_0, ret_mag, ret_mag
        fmuls       n_1, ret_mag, half
        fnmsubs     n_0, n_0, square_mag, three
        fmuls       ret_mag, n_0, n_1
        fmuls       square_mag, square_mag, ret_mag
    __exit:
    }
    #endif // clang-format on
    return square_mag;
}

void C_VECDotProduct(void)
{
    // UNUSED FUNCTION
}

ASM f32 PSVECDotProduct(register const Vec *a, register const Vec *b)
{
#ifdef __MWERKS__ // clang-format off
    nofralloc;
    psq_l     f2, 4(a), 0, 0;
    psq_l     f3, 4(b), 0, 0;
    ps_mul    f2, f2, f3;
    psq_l     f5, 0(a), 0, 0;
    psq_l     f4, 0(b), 0, 0;
    ps_madd   f3, f5, f4, f2;
    ps_sum0   R_RET, f3, f2, f2;
    blr
#endif // clang-format on
}

void C_VECCrossProduct(void)
{
    // UNUSED FUNCTION
}

ASM void PSVECCrossProduct(const register Vec *vec1, const register Vec *vec2, register Vec *ret)
{
#ifdef __MWERKS__ // clang-format off
    nofralloc;
    psq_l       fp1, 0(vec2), 0, 0
    lfs         fp2, 8(vec1)
    psq_l       fp0, 0(vec1), 0, 0
    ps_merge10  fp6, fp1, fp1
    lfs         fp3, 8(vec2)
    ps_mul      fp4, fp1, fp2
    ps_muls0    fp7, fp1, fp0
    ps_msub     fp5, fp0, fp3, fp4
    ps_msub     fp8, fp0, fp6, fp7
    ps_merge11  fp9, fp5, fp5
    ps_merge01  fp10, fp5, fp8
    psq_st      fp9, 0(ret), 1, 0
    ps_neg      fp10, fp10
    psq_st      fp10, 4(ret), 0, 0
    blr;
#endif // clang-format on
}

void C_VECHalfAngle(void)
{
    // UNUSED FUNCTION
}

void C_VECReflect(void)
{
    // UNUSED FUNCTION
}

void C_VECSquareDistance(void)
{
    // UNUSED FUNCTION
}

f32 PSVECSquareDistance(const Vec *a, const Vec *b)
{
    // UNUSED FUNCTION
}

void C_VECDistance(void)
{
    // UNUSED FUNCTION
}

// TODO: cleanup?
ASM f32 PSVECDistance(register const Vec *a, register const Vec *b)
{    
#ifdef __MWERKS__ // clang-format off
    nofralloc;
    psq_l     f0, 4(a), 0, 0;
    psq_l     f1, 4(b), 0, 0;
    ps_sub    f2, f0, f1;
    psq_l     f0, 0(a), 0, 0;
    psq_l     f1, 0(b), 0, 0;
    ps_mul    f2, f2, f2;
    ps_sub    f0, f0, f1;
    lfs       f3, 0.5f;
    
    ps_madd   f1, f0, f0, f2;
    fsubs     f0, f3, f3;
    ps_sum0   f1, f1, f2, f2;
    fcmpu     cr0, f0, f1;
    beq       exit;
    lfs       f4, 3.0f;
    frsqrte   f0, f1;
    fmuls     f2, f0, f0;
    fmuls     f0, f0, f3;
    fnmsubs   f2, f2, f1, f4;
    fmuls     f0, f2, f0;
    fmuls     R_RET, f1, f0;
exit:
    blr
#endif // clang-format on
}