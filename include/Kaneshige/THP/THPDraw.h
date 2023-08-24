#ifndef _THP_THPDRAW_H
#define _THP_THPDRAW_H

#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C"
{
#endif

void THPGXYuv2RgbSetup(GXRenderModeObj *rmode);
void THPGXYuv2RgbDraw(u8 *yTexture, u8 *uTexture, u8 *vTexture, s16, s16, s16 xSize, s16 ySize, s16, s16);
void THPGXRestore();

#ifdef __cplusplus
}
#endif

#endif
