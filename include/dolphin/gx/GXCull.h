#ifndef _DOLPHIN_GXCULL
#define _DOLPHIN_GXCULL

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXSetCullMode(GXCullMode mode);
void GXSetCoPlanar(GXBool enable);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_GXCULL
