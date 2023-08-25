#ifndef _THP_THPAUDIODECODE_H
#define _THP_THPAUDIODECODE_H

#include <dolphin/os/OSThread.h>

#ifdef __cplusplus
extern "C"
{
#endif

BOOL CreateAudioDecodeThread(OSPriority, void *);
void AudioDecodeThreadStart();
void AudioDecodeThreadCancel();
void PushFreeAudioBuffer(OSMessage msg);
void *PopDecodedAudioBuffer(OSMessage msg);

#ifdef __cplusplus
}
#endif

#endif
