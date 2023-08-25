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
void PushFreeTextureSet(OSMessage msg);
void *PopDecodedTextureSet(OSMessage msg);
#ifdef __cplusplus
}
#endif

#endif
