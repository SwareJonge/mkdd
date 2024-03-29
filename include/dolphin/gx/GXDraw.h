#ifndef _DOLPHIN_GXDRAW
#define _DOLPHIN_GXDRAW

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXDrawSphere(u8 numMajor, u8 numMinor);
void GXDrawCylinder(u8 numEdges);
void GXDrawCube();

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXDRAW
