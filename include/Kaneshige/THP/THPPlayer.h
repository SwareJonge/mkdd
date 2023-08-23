#ifndef _THP_THPPLAYER_H
#define _THP_THPPLAYER_H

#include <dolphin/dvd.h>

#include "Kaneshige/THP/THPBuffer.h"
#include "Kaneshige/THP/THPFile.h"
#include "Kaneshige/THP/THPInfo.h"

#include "types.h"

// TODO: Move to dolphin?

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct THPPlayer
{
    DVDFileInfo fileInfo;
    THPHeader header;
    THPFrameCompInfo compInfo;
    THPVideoInfo videoInfo;
    THPAudioInfo audioInfo;
    void *thpWork;
    BOOL open;
    u8 state;
    u8 internalState;
    u8 playFlag;
    u8 audioExist;
    s32 dvdError;
    s32 videoError;
    BOOL onMemory;
    u8 *movieData;
    s32 initOffset;
    s32 initReadSize;
    s32 initReadFrame;
    u32 fieldType;
    s64 retaceCount;
    s32 prevCount;
    s32 curCount;
    s32 videoDecodeCount;
    f32 curVolume;
    f32 targetVolume;
    f32 deltaVolume;
    s32 rampCount;
    s32 curAudioTrack;
    s32 curVideoNumber;
    s32 curAudioNumber;
    THPAudioBuffer *playAudioBuffer;
    THPReadBuffer readBuffer[10];
    THPTextureSet textureSet[3];
    THPAudioBuffer audioBuffer[6];
} THPPlayer; // Size: 0x1f0

extern THPPlayer ActivePlayer; // size: 0x220, aligned ?

u32 THPPlayerGetTotalFrame();

#ifdef __cplusplus
}
#endif

#endif
