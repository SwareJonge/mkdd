#ifndef _DOLPHIN_GXTRANSFORM
#define _DOLPHIN_GXTRANSFORM

#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GX_PROJECTION_SZ 7

void GXSetProjection(f32 mtx[4][4], GXProjectionType type);
void GXLoadPosMtxImm(f32 mtx[3][4], u32 id);
void GXLoadNrmMtxImm(f32 mtx[3][4], u32 id);
void GXLoadTexMtxImm(f32 mtx[][4], u32 id, GXTexMtxType type);
void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz);
void GXSetCurrentMtx(u32 id);
void GXSetViewportJitter(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz, u32 field);
void GXSetScissorBoxOffset(s32 x_off, s32 y_off);
void GXSetScissor(u32 left, u32 top, u32 wd, u32 ht);
void GXGetScissor(u32 *x, u32 *y, u32 *w, u32 *h);
void GXSetClipMode(GXClipMode mode);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXTRANSFORM
