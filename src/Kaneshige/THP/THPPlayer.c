#include <string.h>
#include <dolphin/ai.h>
#include <dolphin/os.h>
#include <dolphin/vi.h>
#include <JSystem/JAudio/System/JASDriver.h>
#include <JSystem/JUtility/JUTDbg.h>

#include "Inagaki/GameAudioMain.h"
#include "Kaneshige/THP/THPPlayer.h"
#include "Kaneshige/THP/THPDraw.h"
#include "Kaneshige/THP/THPRead.h"
#include "Kaneshige/THP/THPAudioDecode.h"
#include "Kaneshige/THP/THPVideoDecode.h"

// TODO: make most stuff C compliant

static u32 WorkBuffer[16] ALIGN(32); // TODO, figure out if this is a struct
static OSMessageQueue PrepareReadyQueue;
static OSMessageQueue UsedTextureSetQueue;
static OSMessage UsedTextureSetMessage[4];
static s16 SoundBuffer[2][1120] ALIGN(32); // unsure if correct, could be a struct

THPPlayer ActivePlayer;

BOOL Initialized;
void *PrepareReadyMessage;
VIRetraceCallback OldVIPostCallback;
s32 SoundBufferIndex;
s16 *LastAudioBuffer;
s16 *CurAudioBuffer;

// clang-format off
u16 VolumeTable[128] = {
    0, 2, 8, 18, 32, 50, 73, 99, 130,
    164, 203, 245, 292, 343, 398, 457, 520,
    587, 658, 733, 812, 895, 983, 1074, 1170,
    1269, 1373, 1481, 1592, 1708, 1828, 1952, 2080,
    2212, 2348, 2488, 2632, 2781, 2933, 3090, 3250,
    3415, 3583, 3756, 3933, 4114, 4298, 4487, 4680,
    4877, 5079, 5284, 5493, 5706, 5924, 6145, 6371,
    6600, 6834, 7072, 7313, 7559, 7809, 8063, 8321,
    8583, 8849, 9119, 9394, 9672, 9954, 10241, 10531,
    10826, 11125, 11427, 11734, 12045, 12360, 12679, 13002,
    13329, 13660, 13995, 14335, 14678, 15025, 15377, 15732,
    16092, 16456, 16823, 17195, 17571, 17951, 18335, 18723,
    19115, 19511, 19911, 20316, 20724, 21136, 21553, 21974,
    22398, 22827, 23260, 23696, 24137, 24582, 25031, 25484,
    25941, 26402, 26868, 27337, 27810, 28288, 28769, 29255,
    29744, 30238, 30736, 31238, 31744, 32254, -32768
};
// clang-format on

static s16 *audioCallback(s32 p1);
static void MixAudio(s16 *, u32);

static BOOL WaitUntilPrepare();
static void InitAllMessageQueue();
static OSMessage PopUsedTextureSet();
static void PushUsedTextureSet(OSMessage msg);
static void PlayControl(u32);

extern BOOL THPInit();

static s16 *audioCallback(s32 p1)
{
    if (ActivePlayer.open == FALSE || ActivePlayer.internalState != 2 || ActivePlayer.audioExist == 0)
    {
        return NULL;
    }

    BOOL enable = OSEnableInterrupts();
    SoundBufferIndex ^= 1;
    MixAudio(SoundBuffer[SoundBufferIndex], p1);
    OSRestoreInterrupts(enable);

    return SoundBuffer[SoundBufferIndex];
}

static void initAudio()
{
    JASDriver::registerMixCallback(audioCallback, MixMode_InterLeave);
}

static void quitAudio()
{
    JASDriver::registerMixCallback(nullptr, MixMode_Mono);
}

BOOL THPPlayerInit()
{

    memset(&ActivePlayer, 0, sizeof(THPPlayer));
    LCEnable();
    OSInitMessageQueue(&UsedTextureSetQueue, UsedTextureSetMessage, 3);
    if (THPInit() == FALSE)
    {
        return FALSE;
    }

    BOOL enable = OSEnableInterrupts();
    SoundBufferIndex = 0;
    LastAudioBuffer = NULL;
    CurAudioBuffer = NULL;
    initAudio();
    OSRestoreInterrupts(enable);
    memset(&SoundBuffer, 0, 0x1180);
    DCFlushRange(&SoundBuffer, 0x1180);
    Initialized = TRUE;

    return TRUE;
}

void THPPlayerQuit()
{
    LCDisable();
    quitAudio();
    Initialized = FALSE;
}

BOOL THPPlayerOpen(const char *fileName, BOOL onMemory)
{
    u32 readOffset;
    int i;

    if (Initialized == FALSE)
    {
        return FALSE;
    }

    if (ActivePlayer.open)
    {
        return FALSE;
    }

    memset(&ActivePlayer.videoInfo, 0, sizeof(THPVideoInfo));
    memset(&ActivePlayer.audioInfo, 0, sizeof(THPAudioInfo));

    if (DVDOpen(fileName, &ActivePlayer.fileInfo) == FALSE)
    {
        return FALSE;
    }

    if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 64, 0, 2) < 0)
    {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    memcpy(&ActivePlayer.header.magic, WorkBuffer, sizeof(THPHeader));

    if (strcmp(ActivePlayer.header.magic, "THP") != 0)
    {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    if (ActivePlayer.header.version != 0x11000)
    {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    readOffset = ActivePlayer.header.compInfoDataOffsets;

    if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 32, readOffset, 2) < 0)
    {
        DVDClose(&ActivePlayer.fileInfo);
        return FALSE;
    }

    memcpy(&ActivePlayer.compInfo, WorkBuffer, sizeof(THPFrameCompInfo));
    readOffset += sizeof(THPFrameCompInfo);
    ActivePlayer.audioExist = 0;

    for (i = 0; i < ActivePlayer.compInfo.numComponents; i++)
    {
        switch (ActivePlayer.compInfo.frameComp[i])
        {
        case 0:
        {
            if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 32, readOffset, 2) < 0)
            {
                DVDClose(&ActivePlayer.fileInfo);
                return FALSE;
            }

            memcpy(&ActivePlayer.videoInfo, WorkBuffer, sizeof(THPVideoInfo));
            if (ActivePlayer.videoInfo.videoType != 0)
            {
                ActivePlayer.videoInfo.videoType = 2;
            }
            readOffset += sizeof(THPVideoInfo);
            break;
        }
        case 1:
        {
            if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 32, readOffset, 2) < 0)
            {
                DVDClose(&ActivePlayer.fileInfo);
                return FALSE;
            }
            memcpy(&ActivePlayer.audioInfo, WorkBuffer, sizeof(THPAudioInfo));
            readOffset += sizeof(THPAudioInfo);
            ActivePlayer.audioExist = 1;
            break;
        }

        default:
            return FALSE;
        }
    }

    ActivePlayer.internalState = 0;
    ActivePlayer.state = 0;
    ActivePlayer.playFlag = 0;
    ActivePlayer.onMemory = onMemory;
    ActivePlayer.open = TRUE;

    ActivePlayer.curVolume = GameAudio::Main::getAudio()->getTHPOptionVolume() * 127.0f;
    f32 curVol = ActivePlayer.curVolume;
    bool validVolume = false;
    if (curVol >= 0.0f && curVol <= 127.0f)
    {
        validVolume = true;
    }
#line 365
    JUT_ASSERT_F(validVolume, "INVALID VOLUME(%f)", curVol);

    ActivePlayer.targetVolume = ActivePlayer.curVolume;
    ActivePlayer.rampCount = 0;

    return TRUE;
}

BOOL THPPlayerClose()
{
    if (ActivePlayer.open && ActivePlayer.state == 0)
    {
        ActivePlayer.open = FALSE;
        DVDClose(&ActivePlayer.fileInfo);
        return TRUE;
    }

    return FALSE;
}

u32 THPPlayerCalcNeedMemory()
{
    if(ActivePlayer.open) {
        u32 size = ActivePlayer.onMemory ? ALIGN_NEXT(ActivePlayer.header.movieDataSize, 32) : ALIGN_NEXT(ActivePlayer.header.bufsize, 32) * 10;
        
        size += ALIGN_NEXT(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize, 32) * 3;
        size += ALIGN_NEXT(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize / 4, 32) * 3;
        size += ALIGN_NEXT(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize / 4, 32) * 3;

        if(ActivePlayer.audioExist) {
            size += ALIGN_NEXT(ActivePlayer.header.audioMaxSamples * 4, 32) * 6;
        }
        return size + 0x1000;
    }
    return 0;
}

BOOL THPPlayerSetBuffer(u8 *data)
{
    u8 *workPtr;
    u32 ySampleSize;
    u32 uvSampleSize;
    s32 i;
    if(ActivePlayer.open && ActivePlayer.state == 0) {
        u8 *workPtr = data;
        if(ActivePlayer.onMemory) {
            ActivePlayer.movieData = data;
            workPtr += ActivePlayer.header.movieDataSize;
        }
        else {
            for (i = 0; i < ARRAY_SIZE(ActivePlayer.readBuffer); i++)
            {
                ActivePlayer.readBuffer[i].ptr = workPtr;
                workPtr += ALIGN_NEXT(ActivePlayer.header.bufsize, 32);
            }
        }

        ySampleSize = ALIGN_NEXT(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize, 32);
        uvSampleSize = ALIGN_NEXT(ActivePlayer.videoInfo.xSize * ActivePlayer.videoInfo.ySize / 4, 32);

        for (i = 0; i < ARRAY_SIZE(ActivePlayer.textureSet); i++)
        {
            ActivePlayer.textureSet[i].ytexture = workPtr;

            DCInvalidateRange(workPtr, ySampleSize);
            workPtr += ySampleSize;

            ActivePlayer.textureSet[i].utexture = workPtr;
            DCInvalidateRange(workPtr, uvSampleSize);
            workPtr += uvSampleSize;
            
            ActivePlayer.textureSet[i].vtexture = workPtr;
            DCInvalidateRange(workPtr, uvSampleSize);
            workPtr += uvSampleSize;
        }

        if(ActivePlayer.audioExist) {
            for(i = 0; i < ARRAY_SIZE(ActivePlayer.audioBuffer); i++) {
                ActivePlayer.audioBuffer[i].buffer = (s16 *)workPtr;
                ActivePlayer.audioBuffer[i].curPtr = (s16 *)workPtr;
                ActivePlayer.audioBuffer[i].validSample = 0;
                workPtr += ALIGN_NEXT(ActivePlayer.header.audioMaxSamples * 4, 32);
            }
        }

        ActivePlayer.thpWork = workPtr;
        return TRUE;
    }

    return FALSE;
}

void PrepareReady(int msg)
{
    OSSendMessage(&PrepareReadyQueue, (OSMessage)msg, OS_MESSAGE_BLOCK);
}

BOOL THPPlayerPrepare(s32 frame, u8 flag, s32 audioTrack)
{
    if (ActivePlayer.open && ActivePlayer.state == 0) {
        if (frame > 0)
        {
            if (ActivePlayer.header.offsetDataOffsets == 0)
                return FALSE;

            if (ActivePlayer.header.numFrames > frame)
            {
                if (DVDReadPrio(&ActivePlayer.fileInfo, WorkBuffer, 0x20, ActivePlayer.header.offsetDataOffsets + (frame - 1) * 4, 2) < 0)
                    return FALSE;

                ActivePlayer.initOffset = ActivePlayer.header.movieDataOffsets + WorkBuffer[0];
                ActivePlayer.initReadSize = WorkBuffer[1] - WorkBuffer[0];
                ActivePlayer.initReadFrame = frame;
            }
            else {
                return FALSE;
            }
        }

        else {
            ActivePlayer.initOffset = ActivePlayer.header.movieDataOffsets;
            ActivePlayer.initReadSize = ActivePlayer.header.firstFrameSize;
            ActivePlayer.initReadFrame = frame;
        }

        if (ActivePlayer.audioExist) {
            if (audioTrack < 0 || audioTrack >= ActivePlayer.audioInfo.sndNumTracks)
                return FALSE;
            ActivePlayer.curAudioTrack = audioTrack;
        }

        ActivePlayer.playFlag = flag & 1;
        ActivePlayer.videoDecodeCount = 0;

        if(ActivePlayer.onMemory) {
            if (DVDReadPrio(&ActivePlayer.fileInfo, ActivePlayer.movieData, ActivePlayer.header.movieDataSize, ActivePlayer.header.movieDataOffsets, 2) < 0) {
                return FALSE;
            }

            u8 *threadData = ActivePlayer.movieData + ActivePlayer.initOffset - ActivePlayer.header.movieDataOffsets;
            CreateVideoDecodeThread(22, threadData);

            if (ActivePlayer.audioExist)
                CreateAudioDecodeThread(21, threadData);
        }
        else {
            CreateVideoDecodeThread(22, 0);
            if (ActivePlayer.audioExist)
                CreateAudioDecodeThread(21, NULL);
            CreateReadThread(20);
        }

        ActivePlayer.curVideoNumber = -1;
        ActivePlayer.curAudioNumber = 0;
        
        InitAllMessageQueue();
        VideoDecodeThreadStart();

        if (ActivePlayer.audioExist)
            AudioDecodeThreadStart();
        if (ActivePlayer.onMemory == 0)
            ReadThreadStart();

        if (WaitUntilPrepare() == FALSE)
            return FALSE;

        ActivePlayer.state = 1;
        ActivePlayer.internalState = 0;
        ActivePlayer.dispTextureSet = NULL;
        ActivePlayer.playAudioBuffer = NULL;

        if (ActivePlayer.videoInfo.videoType == 1)
            ActivePlayer.fieldType = 0;
        else if (ActivePlayer.videoInfo.videoType == 2)
            ActivePlayer.fieldType = 1;

        OldVIPostCallback = VISetPostRetraceCallback(PlayControl);

        return TRUE;
    }
    
    return FALSE;
}

BOOL THPPlayerPlay()
{
    if (ActivePlayer.open && (ActivePlayer.state == 1 || ActivePlayer.state == 4))
    {
        ActivePlayer.state = 2;
        ActivePlayer.prevCount = 0;
        ActivePlayer.curCount = 0;
        ActivePlayer.retaceCount = -1;
        return TRUE;
    }
    return FALSE;
}

void THPPlayerStop()
{
    if(ActivePlayer.open && ActivePlayer.state != 0) {
        ActivePlayer.internalState = 0;
        ActivePlayer.state = 0;
        VISetPostRetraceCallback(OldVIPostCallback);
        if (ActivePlayer.onMemory == 0)
        {
            DVDCancel(&ActivePlayer.fileInfo);
            ReadThreadCancel();
        }

        VideoDecodeThreadCancel();
        if(ActivePlayer.audioExist) {
            AudioDecodeThreadCancel();
            quitAudio();
        }

        while(PopUsedTextureSet() != 0) {}

        ActivePlayer.rampCount = 0;
        ActivePlayer.curVolume = ActivePlayer.targetVolume;
        ActivePlayer.dvdError = 0;
        ActivePlayer.videoError = 0;
    }
}

BOOL THPPlayerPause()
{
    if (ActivePlayer.open && ActivePlayer.state == 2)
    {
        ActivePlayer.internalState = 4;
        ActivePlayer.state = 4;
        return TRUE;
    }
    return FALSE;
}
s32 THPPlayerDrawCurrentFrame(GXRenderModeObj *rmode, s32 x, s32 y, s32 z, s32 w)
{
    s32 frame = -1;
    if (ActivePlayer.open && ActivePlayer.state != 0 && ActivePlayer.dispTextureSet != NULL)
    {
        THPGXYuv2RgbSetup(rmode);
        THPGXYuv2RgbDraw(ActivePlayer.dispTextureSet->ytexture,
                         ActivePlayer.dispTextureSet->utexture,
                         ActivePlayer.dispTextureSet->vtexture,
                         x, y, ActivePlayer.videoInfo.xSize, ActivePlayer.videoInfo.ySize, z, w);
        THPGXRestore();
        frame = (ActivePlayer.dispTextureSet->frameNumber + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
    }
    return frame;
}

// might be THPVideoInfo as parameter
BOOL THPPlayerGetVideoInfo(void *dst)
{
    if (ActivePlayer.open)
    {
        memcpy(dst, &ActivePlayer.videoInfo, sizeof(THPVideoInfo));
        return TRUE;
    }

    return FALSE;
}

u32 THPPlayerGetTotalFrame()
{
    if (ActivePlayer.open != 0)
    {
        return ActivePlayer.header.numFrames;
    }
    return 0;
}

u8 THPPlayerGetState()
{
    return ActivePlayer.state;
}

static void PushUsedTextureSet(OSMessage msg)
{
    OSSendMessage(&UsedTextureSetQueue, msg, OS_MESSAGE_NOBLOCK);
}

void THPPlayerPostDrawDone() {
    if(Initialized) {
        OSMessage msg;
        while(TRUE)
        {
            msg = PopUsedTextureSet();
            if(msg == NULL) {
                break;
            }
            PushUsedTextureSet(msg);
            
        } 
    }
}

BOOL THPPlayerSetVolume(s32 vol, s32 duration)
{
    if(ActivePlayer.open && ActivePlayer.audioExist) {
        u32 numSamples = AIGetDSPSampleRate() == 0 ? 32 : 48;
        
        // clamp volume
        if (vol > 127)
            vol = 127;
        if(vol < 0)
            vol = 0;

        // clamp duration
        if(duration > 60000)
            duration = 60000;
        if (duration < 0)
            duration = 0;

        BOOL interrupt = OSDisableInterrupts();

        ActivePlayer.targetVolume = vol;
        if(duration != 0) {
            ActivePlayer.rampCount = numSamples * duration;
            ActivePlayer.deltaVolume = (ActivePlayer.targetVolume - ActivePlayer.curVolume) / ActivePlayer.rampCount;
        }
        else {
            ActivePlayer.rampCount = 0;
            ActivePlayer.curVolume = ActivePlayer.targetVolume;
        }
        OSRestoreInterrupts(interrupt);

        return TRUE;

    } 
    return FALSE;
}

#include <JSystem/JAudio/System/JASFakeMatch.h>