#include <dolphin/thp.h>
#include "Kaneshige/Movie/THPDraw.h"
#include "Kaneshige/THP/THPRead.h"
#include "Kaneshige/THP/THPVideoDecode.h"
#include "Kaneshige/THP/THPPlayer.h"

#define STACK_SIZE 4096
#define BUFFER_COUNT 3

static OSThread VideoDecodeThread;
static u8 VideoDecodeThreadStack[STACK_SIZE];
static OSMessageQueue FreeTextureSetQueue;
static OSMessageQueue DecodedTextureSetQueue;
static OSMessage FreeTextureSetMessage[BUFFER_COUNT];
static OSMessage DecodedTextureSetMessage[BUFFER_COUNT];

static BOOL VideoDecodeThreadCreated;
static BOOL First;

static void *VideoDecoderForOnMemory(void *arg);
static void *VideoDecoder(void *arg);
static u32 VideoDecode(THPReadBuffer *readBuffer);

BOOL CreateVideoDecodeThread(OSPriority prio, void *arg)
{
    BOOL res;
    if (arg)
    {
        res = OSCreateThread(&VideoDecodeThread, VideoDecoderForOnMemory, arg, VideoDecodeThreadStack + STACK_SIZE, STACK_SIZE, prio, 1);
        if (res == FALSE)
            return FALSE;
    }
    else
    {
        res = OSCreateThread(&VideoDecodeThread, VideoDecoder, NULL, VideoDecodeThreadStack + STACK_SIZE, STACK_SIZE, prio, 1);
        if (res == FALSE)
            return FALSE;
    }

    OSInitMessageQueue(&FreeTextureSetQueue, FreeTextureSetMessage, BUFFER_COUNT);
    OSInitMessageQueue(&DecodedTextureSetQueue, DecodedTextureSetMessage, BUFFER_COUNT);
    VideoDecodeThreadCreated = TRUE;
    First = TRUE;
    return TRUE;
}

void VideoDecodeThreadStart()
{
    if (VideoDecodeThreadCreated)
        OSResumeThread(&VideoDecodeThread);
}

void VideoDecodeThreadCancel()
{
    if (VideoDecodeThreadCreated)
    {
        OSCancelThread(&VideoDecodeThread);
        VideoDecodeThreadCreated = FALSE;
    }
}

void *VideoDecoder(void *arg)
{
    THPReadBuffer *thpBuffer;
    u32 decodedFrame = 0;

    while (TRUE)
    {
        if (ActivePlayer.audioExist)
        {
            for (; decodedFrame != 0; decodedFrame--)
            {
                thpBuffer = (THPReadBuffer *)PopReadedBuffer2();
                s32 curFrame = (thpBuffer->frameNumber + ActivePlayer.initReadFrame);
                s32 remaining = (curFrame % ActivePlayer.header.numFrames);
                if (remaining == (ActivePlayer.header.numFrames - 1) && (ActivePlayer.playFlag & 1) == 0)
                    VideoDecode(thpBuffer);

                PushFreeReadBuffer(thpBuffer);
                ActivePlayer.videoDecodeCount++;
            }
        }

        if (ActivePlayer.audioExist)
            thpBuffer = (THPReadBuffer *)PopReadedBuffer2();
        else
            thpBuffer = (THPReadBuffer *)PopReadedBuffer();

        decodedFrame = VideoDecode(thpBuffer);
        PushFreeReadBuffer(thpBuffer);
    }
}

void *VideoDecoderForOnMemory(void *arg)
{
    s32 readSize;
    s32 frame;
    u32 i;
    THPReadBuffer readBuffer;

    frame = 0;
    i = 0;
    readSize = ActivePlayer.initReadSize;

    readBuffer.ptr = (u8 *)arg;
    while (TRUE)
    {
        if (ActivePlayer.audioExist)
        {
            while (i--)
            {
                ActivePlayer.videoDecodeCount++;
                s32 remaining = (frame + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
                if (remaining == ActivePlayer.header.numFrames - 1)
                {
                    if ((ActivePlayer.playFlag & 1) == 0)
                        break;

                    readSize = *(s32 *)readBuffer.ptr;
                    readBuffer.ptr = ActivePlayer.movieData;
                }
                else
                {
                    s32 size = *(s32 *)readBuffer.ptr;
                    readBuffer.ptr += readSize;
                    readSize = size;
                }
                frame++;
            }
        }

        readBuffer.frameNumber = frame;

        i = VideoDecode(&readBuffer);

        s32 remaining = (frame + ActivePlayer.initReadFrame) % ActivePlayer.header.numFrames;
        if (remaining == ActivePlayer.header.numFrames - 1)
        {
            if ((ActivePlayer.playFlag & 1))
            {
                readSize = *(s32 *)readBuffer.ptr;
                readBuffer.ptr = ActivePlayer.movieData;
            }
            else
            {
                OSSuspendThread(&VideoDecodeThread);
            }
        }
        else
        {
            s32 size = *(s32 *)readBuffer.ptr;
            readBuffer.ptr += readSize;
            readSize = size;
        }

        frame++;
    }
}

u32 VideoDecode(THPReadBuffer *readBuffer)
{
    THPTextureSet *textureSet;
    s32 i;
    u32 decodedFrame;
    u32 *tileOffsets;
    u8 *tile;

    decodedFrame = 0;
    tileOffsets = (u32 *)(readBuffer->ptr + 8);
    tile = &readBuffer->ptr[ActivePlayer.compInfo.numComponents * 4] + 8;
    textureSet = (THPTextureSet *)PopFreeTextureSet();

    for (i = 0; i < ActivePlayer.compInfo.numComponents; i++)
    {
        switch (ActivePlayer.compInfo.frameComp[i])
        {
        case 0:
        {
            ActivePlayer.videoError = THPVideoDecode(tile, textureSet->ytexture, textureSet->utexture, textureSet->vtexture, ActivePlayer.thpWork);
            if (ActivePlayer.videoError)
            {
                if (First)
                {
                    PrepareReady(FALSE);
                    First = FALSE;
                }
                OSSuspendThread(&VideoDecodeThread);
            }
            textureSet->frameNumber = readBuffer->frameNumber;
            PushDecodedTextureSet(textureSet);
            s32 decCnt = ActivePlayer.videoDecodeCount;
            s32 vidNum = ActivePlayer.curVideoNumber + 1;
            decodedFrame = (vidNum > decCnt ? vidNum - decCnt : 0);
            ActivePlayer.videoDecodeCount++;
        }
        }

        tile += *tileOffsets;
        tileOffsets++;
    }

    if (First)
    {
        PrepareReady(TRUE);
        First = FALSE;
    }

    return decodedFrame;
}

void *PopFreeTextureSet()
{
    void *tex;
    OSReceiveMessage(&FreeTextureSetQueue, &tex, 1);
    return tex;
}

void PushFreeTextureSet(void *tex)
{
    OSSendMessage(&FreeTextureSetQueue, tex, 0);
}

void *PopDecodedTextureSet(s32 flags)
{
    void *tex;
    if (OSReceiveMessage(&DecodedTextureSetQueue, &tex, flags) == TRUE)
    {
        return tex;
    }
    return NULL;
}

void PushDecodedTextureSet(void *tex)
{
    OSSendMessage(&DecodedTextureSetQueue, tex, OS_MESSAGE_BLOCK);
}
