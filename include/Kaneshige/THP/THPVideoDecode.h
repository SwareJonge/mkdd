#ifndef _THP_THPVIDEODECODE_H
#define _THP_THPVIDEODECODE_H

#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C"
{
#endif

BOOL CreateVideoDecodeThread(OSPriority, void *);
void VideoDecodeThreadStart();
void VideoDecodeThreadCancel();

#ifdef __cplusplus
}
#endif

#endif
