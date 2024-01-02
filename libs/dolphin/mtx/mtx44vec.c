#include "dolphin/mtx.h"

// TODO: cleanup, documentation, check other builds
void PSMTX44MultVec(register const Mtx44 m, register const Vec *src, register Vec *dst)
{
    // clang-format off
    __asm {
        psq_l       f0, 0(src), 0, 0
        psq_l       f2, 0x30(m), 0, 0
        psq_l       f1, 8(src), 1, 0
        ps_mul      f4, f0, f2
        psq_l       f3, 0x38(m), 0, 0
        ps_madd     f5, f1, f3, f4
        ps_merge11  f12, f1, f1
        ps_sum0     f13, f5, f5, f5
        psq_l       f4, 0(m), 0, 0
        ps_merge00  f13, f13, f13
        psq_l       f5, 8(m), 0, 0
        ps_div      f13, f12, f13
        psq_l       f6, 0x10(m), 0, 0
        psq_l       f7, 0x18(m), 0, 0
        psq_l       f8, 0x20(m), 0, 0
        psq_l       f9, 0x28(m), 0, 0
        ps_mul      f4, f0, f4
        ps_madd     f2, f1, f5, f4
        ps_mul      f6, f0, f6
        ps_madd     f3, f1, f7, f6
        ps_mul      f8, f0, f8
        ps_sum0     f2, f2, f2, f2
        ps_madd     f9, f1, f9, f8
        ps_sum1     f2, f3, f2, f3
        ps_sum0     f3, f9, f9, f9
        ps_mul      f2, f2, f13
        psq_st      f2, 0(dst), 0, 0
        ps_mul      f3, f3, f13
        psq_st      f3, 8(dst), 1, 0
    }
    // clang-format on
}