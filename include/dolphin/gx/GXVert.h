#ifndef _DOLPHIN_GXVERT
#define _DOLPHIN_GXVERT

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GXFIFO_ADDR 0xCC008000

typedef union {
  u8 u8;
  u16 u16;
  u32 u32;
  u64 u64;
  s8 s8;
  s16 s16;
  s32 s32;
  s64 s64;
  f32 f32;
  f64 f64;
} PPCWGPipe;

#ifdef __MWERKS__
/*volatile*/ PPCWGPipe GXWGFifo : GXFIFO_ADDR;
#else
#define GXWGFifo (*(volatile PPCWGPipe*)GXFIFO_ADDR)
#endif

static inline void GXPosition2f32(f32 x, f32 y) {
  GXWGFifo.f32 = x;
  GXWGFifo.f32 = y;
}

static inline void GXPosition3s16(s16 x, s16 y, s16 z) {
  GXWGFifo.s16 = x;
  GXWGFifo.s16 = y;
  GXWGFifo.s16 = z;
}

static inline void GXPosition3f32(f32 x, f32 y, f32 z) {
  GXWGFifo.f32 = x;
  GXWGFifo.f32 = y;
  GXWGFifo.f32 = z;
}

static inline void GXNormal3f32(f32 x, f32 y, f32 z) {
  GXWGFifo.f32 = x;
  GXWGFifo.f32 = y;
  GXWGFifo.f32 = z;
}

static inline void GXColor1u32(u32 v) {
  GXWGFifo.u32 = v;
}

static inline void GXColor4u8(u8 r, u8 g, u8 b, u8 a) {
  GXWGFifo.u8 = r;
  GXWGFifo.u8 = g;
  GXWGFifo.u8 = b;
  GXWGFifo.u8 = a;
}

static inline void GXTexCoord2s16(s16 u, s16 v) {
  GXWGFifo.s16 = u;
  GXWGFifo.s16 = v;
}

static inline void GXTexCoord2u16(u16 u, u16 v)
{
  GXWGFifo.u16 = u;
  GXWGFifo.u16 = v;
}

static inline void GXPosition2u16(u16 x, u16 y)
{
  GXWGFifo.u16 = x;
  GXWGFifo.u16 = y;
}

static inline void GXTexCoord2f32(f32 u, f32 v) {
  GXWGFifo.f32 = u;
  GXWGFifo.f32 = v;
}

static inline void GXTexCoord2u8(u8 u, u8 v)
{
  GXWGFifo.u8 = u;
  GXWGFifo.u8 = v;
}

static inline void GXPosition1x8(u8 index) {
  GXWGFifo.u8 = index;
}

static inline void GXEnd(void) {}

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXVERT
