#ifndef _DOLPHIN_THP_H
#define _DOLPHIN_THP_H

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL THPInit();
s32 THPVideoDecode(void *file, void *tileY, void *tileU, void *tileV, void *work);

#ifdef __cplusplus
}
#endif

#endif