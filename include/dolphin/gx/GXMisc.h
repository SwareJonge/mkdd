#ifndef RVL_SDK_GX_MISC_H
#define RVL_SDK_GX_MISC_H
#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C"
{
#endif

void GXSetMisc(GXMiscToken token, u32 val);
void GXFlush();
void GXResetWriteGatherPipe();
void GXPeekARGB(u16 x, u16 y, u32 *color);

void GXAbortFrame();

#ifdef __cplusplus
}
#endif
#endif