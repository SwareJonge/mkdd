#ifndef _DOLPHIN_GXSTRUCT
#define _DOLPHIN_GXSTRUCT

#include <dolphin/gx/GXEnum.h>
#include <dolphin/vi.h>
#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXRenderModeObj {
  VITVMode viTVmode;
  u16 fbWidth;
  u16 efbHeight;
  u16 xfbHeight;
  u16 viXOrigin;
  u16 viYOrigin;
  u16 viWidth;
  u16 viHeight;
  VIXFBMode xFBmode;
  u8 field_rendering;
  u8 aa;
  u8 sample_pattern[12][2];
  u8 vfilter[7];
} GXRenderModeObj;

typedef struct _GXColor {
  u8 r;
  u8 g;
  u8 b;
  u8 a;
} GXColor;

typedef struct _GXTexObj {
  u32 dummy[8];
} GXTexObj;

typedef struct _GXTlutObj {
  u32 dummy[3];
} GXTlutObj;

typedef struct _GXLightObj {
  u32 dummy[16];
} GXLightObj;

typedef struct _GXVtxDescList {
  GXAttr attr;
  GXAttrType type;
} GXVtxDescList;

typedef struct _GXVtxAttrFmtList {
	GXAttr attr;
  GXCompCnt cnt;
  GXCompType type;
	u8 frac;
} GXVtxAttrFmtList;

typedef struct _GXColorS10 {
  s16 r;
  s16 g;
  s16 b;
  s16 a;
} GXColorS10;

typedef struct _GXTexRegion {
  u32 dummy[4];
} GXTexRegion;

typedef struct _GXTlutRegion {
  u32 dummy[4];
} GXTlutRegion;

typedef struct _GXFogAdjTable {
	u16 fogVals[10]; // _00
} GXFogAdjTable;

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXSTRUCT
