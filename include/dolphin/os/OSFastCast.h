#ifndef _DOLPHIN_OSFASTCAST
#define _DOLPHIN_OSFASTCAST

#ifdef __cplusplus
extern "C" {
#endif

#include "dolphin/types.h"

#define OS_GQR_F32 0x0000
#define OS_GQR_U8 0x0004
#define OS_GQR_U16 0x0005
#define OS_GQR_S8 0x0006
#define OS_GQR_S16 0x0007

#define OS_FASTCAST_U8 2
#define OS_FASTCAST_U16 3
#define OS_FASTCAST_S8 4
#define OS_FASTCAST_S16 5
// clang-format off
static inline void OSInitFastCast(void) {
#ifdef __MWERKS__
  asm
  {
        li      r3, OS_GQR_U8
        oris    r3, r3, OS_GQR_U8
        mtspr   GQR2, r3

        li      r3, OS_GQR_U16
        oris    r3, r3, OS_GQR_U16
        mtspr   GQR3, r3

        li      r3, OS_GQR_S8
        oris    r3, r3, OS_GQR_S8
        mtspr   GQR4, r3

        li      r3, OS_GQR_S16
        oris    r3, r3, OS_GQR_S16
        mtspr   GQR5, r3
  }
#else

#endif
}
// clang-format off


static inline s16 __OSf32tos16(register f32 inF)
{
	register s16 out;
	u32 tmp;
	register u32* tmpPtr = &tmp;
	// clang-format off
    asm {
        psq_st inF, 0(tmpPtr), 0x1, OS_FASTCAST_S16
        lha out, 0(tmpPtr)
    }
  // clang-format on

  return out;
}

static inline void OSf32tos16(f32 *f, s16 *out) { *out = __OSf32tos16(*f); }

static inline u8 __OSf32tou8(register f32 inF)
{
  register u8 out;
  u32 tmp;
  register u32 *tmpPtr = &tmp;
  // clang-format off
    asm {
        psq_st inF, 0(tmpPtr), 0x1, OS_FASTCAST_U8
        lbz out, 0(tmpPtr)
    }
  // clang-format on

  return out;
}

static inline void OSf32tou8(f32 *f, u8 *out) { *out = __OSf32tou8(*f); }

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSFASTCAST
