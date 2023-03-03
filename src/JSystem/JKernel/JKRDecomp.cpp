#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRAramPiece.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"

// most copied from TP Decomp, i did put in some effort myself but since TP already had most matching, i opted to use that instead
JKRDecomp *JKRDecomp::sDecompObject;
OSMessage JKRDecomp::sMessageBuffer[8] = {0};
OSMessageQueue JKRDecomp::sMessageQueue = {0};

JKRDecomp *JKRDecomp::create(s32 priority) {
    if (!sDecompObject)
        sDecompObject = new (JKRHeap::getSystemHeap(), 0) JKRDecomp(priority);

    return sDecompObject;
}

JKRDecomp::JKRDecomp(s32 threadPriority) : JKRThread(0x4000, 16, threadPriority) {
    resume();
}

JKRDecomp::~JKRDecomp() {}

void *JKRDecomp::run() {
    
    
    OSInitMessageQueue(&sMessageQueue, sMessageBuffer, 8);
    for (;;) {
        OSMessage msg;
        OSReceiveMessage(&sMessageQueue, &msg, OS_MESSAGE_BLOCK);
        JKRDecompCommand *decompCmd = (JKRDecompCommand *)(msg);
        decode(decompCmd->mSrcBuffer, decompCmd->mDstBuffer, decompCmd->mSrcLength, decompCmd->mDstLength);
        if (decompCmd->_20 != 0) {
            if (decompCmd->_20 == 1)
                JKRAramPiece::sendCommand(decompCmd->mAramCmd);
            continue;
        }

        if (decompCmd->mCb) {
            (*decompCmd->mCb)((u32)decompCmd);
            continue;
        }
        if (decompCmd->_1C)
            OSSendMessage(decompCmd->_1C, (OSMessage)1, OS_MESSAGE_NOBLOCK);
        else
            OSSendMessage(&decompCmd->mMessageQueue, (OSMessage)1, OS_MESSAGE_NOBLOCK);
    }


}

JKRDecompCommand *JKRDecomp::prepareCommand(u8 *srcBuffer, u8 *dstBuffer, u32 srcLength,
                                            u32 dstLength,
                                            JKRDecompCommand::AsyncCallback callback)
{
    JKRDecompCommand *command = new (JKRHeap::getSystemHeap(), -4) JKRDecompCommand();
    command->mSrcBuffer = srcBuffer;
    command->mDstBuffer = dstBuffer;
    command->mSrcLength = srcLength;
    command->mDstLength = dstLength;
    command->mCb = callback;
    return command;
}

void JKRDecomp::sendCommand(JKRDecompCommand *decompCmd) {
    BOOL msgret = OSSendMessage(&sMessageQueue, decompCmd, 0);
    JUT_ASSERT_MSG(142, msgret, "Decomp MesgBuf FULL!");
}

JKRDecompCommand *JKRDecomp::orderAsync(u8 *srcBuffer, u8 *dstBuffer, u32 srcLength, u32 dstLength,
                                        JKRDecompCommand::AsyncCallback callback)
{
    JKRDecompCommand *command =
        prepareCommand(srcBuffer, dstBuffer, srcLength, dstLength, callback);
    sendCommand(command);
    return command;
}

bool JKRDecomp::sync(JKRDecompCommand *command, int isNonBlocking) {
    OSMessage message;
    bool result;
    if (isNonBlocking == JKRDECOMP_SYNC_BLOCKING) {
        OSReceiveMessage(&command->mMessageQueue, &message, OS_MESSAGE_BLOCK);
        result = true;
    }
    else {
        result = OSReceiveMessage(&command->mMessageQueue, &message, OS_MESSAGE_NOBLOCK) != FALSE;
    }

    return result;
}

bool JKRDecomp::orderSync(u8 *srcBuffer, u8 *dstBuffer, u32 srcLength, u32 dstLength) {
    JKRDecompCommand *command = orderAsync(srcBuffer, dstBuffer, srcLength, dstLength, nullptr);
    bool result = sync(command, JKRDECOMP_SYNC_BLOCKING);
    delete command;
    return result;
}

void JKRDecomp::decode(u8 *src, u8 *dst, u32 srcSize, u32 dstSize) {
    CompressionMethod compression = checkCompressed(src);
    if (compression == TYPE_YAY0)
        decodeSZP(src, dst, srcSize, dstSize);
    else if (compression == TYPE_YAZ0)
        decodeSZS(src, dst, srcSize, dstSize);
}

// doesn't match for release
void JKRDecomp::decodeSZP(u8 *src, u8 *dst, u32 srcLength, u32 dstLength)
{
    // s32 decodedSize;
    s32 srcChunkOffset;
    u32 count;
    s32 dstOffset;
    u32 length;
    s32 linkInfo;
    s32 offset;
    s32 i;
    // TODO: Replace inline with macro
    s32 decodedSize = read_big_endian_u32(src + 4);
    s32 linkTableOffset = read_big_endian_u32(src + 8);
    s32 srcDataOffset = read_big_endian_u32(src + 12);

    dstOffset = 0;
    u32 counter = 0;
    srcChunkOffset = 16;

    u32 chunkBits;
    if (srcLength == 0)
        return;
    if (dstLength > decodedSize)
        return;

    length = srcLength;
    do
    {
        if (counter == 0)
        {
            chunkBits = read_big_endian_u32(src + srcChunkOffset);
            srcChunkOffset += sizeof(u32);
            counter = sizeof(u32) * 8;
        }

        if (chunkBits & 0x80000000)
        {
            if (dstLength == 0)
            {
                dst[dstOffset] = src[srcDataOffset];
                length--;
                if (length == 0)
                    return;
            }
            else
            {
                dstLength--;
            }
            dstOffset++;
            srcDataOffset++;
        }
        else
        {
            linkInfo = src[linkTableOffset] << 8 | src[linkTableOffset + 1];
            linkTableOffset += sizeof(u16);

            offset = dstOffset - (linkInfo & 0xFFF);
            // rlwinm. r28,r29,0x14,0x1c,0x1f
            count = (linkInfo >> 12);
            if (count == 0)
            {
                count = (u32)src[srcDataOffset] + 0x12;
                srcDataOffset++;
            }
            else
                count += 2;

            if ((s32)count > decodedSize - dstOffset)
                count = decodedSize - dstOffset;

            for (i = 0; i < (s32)count; i++, dstOffset++, offset++)
            {
                if (dstLength == 0)
                {
                    dst[dstOffset] = dst[offset - 1];
                    length--;
                    if (length == 0)
                        return;
                }
                else
                    dstLength--;
            }
        }

        chunkBits <<= 1;
        counter--;
    } while (dstOffset < decodedSize);
}

void JKRDecomp::decodeSZS(u8 *src_buffer, u8 *dst_buffer, u32 srcSize, u32 dstSize) {

    u8 *decompEnd = dst_buffer + *(int *)(src_buffer + 4) - dstSize;    
    u8 *copyStart;
    s32 copyByteCount;    
    s32 chunkBitsLeft = 0;
    s32 chunkBits;

    if (srcSize == 0)
        return;
    if (dstSize > *(u32 *)src_buffer)
        return;
    
    u8 *curSrcPos = src_buffer + 0x10;
    do {


        if (chunkBitsLeft == 0) {
            chunkBits = *curSrcPos;
            chunkBitsLeft = 8;
            curSrcPos++;
        }
        if ((chunkBits & 0x80) != 0) {
            if (dstSize == 0)
            {
                *dst_buffer = *curSrcPos;
                srcSize--;
                dst_buffer++;
                if (srcSize == 0)
                    return;
            }
            else {
                dstSize--;
            }
            curSrcPos++;
        }
        else {
            u8 curVal = *curSrcPos;
            // load is inversed
            copyStart = dst_buffer - ((curVal & 0xF) << 8 | curSrcPos[1]);
            // copyByteCount = ;
            curSrcPos += 2;
            // instruction order differences
            if (curVal >> 4 == 0) {
                copyByteCount = *curSrcPos + 0x12;
                curSrcPos++;
            }
            else {
                copyByteCount = (curVal >> 4) + 2;
            }
            do {
                if (dstSize == 0) {
                    *dst_buffer = *(copyStart - 1);
                    srcSize--;
                    dst_buffer++;
                    if (srcSize == 0)
                        return;
                }
                else {
                    dstSize--;
                }
                copyByteCount--;
                copyStart++;
            } while (copyByteCount != 0);
        }
        chunkBits <<= 1;
        chunkBitsLeft--;
    } while (dst_buffer != decompEnd);
}

CompressionMethod JKRDecomp::checkCompressed(u8 *src)
{
    if ((src[0] == 'Y') && (src[1] == 'a') && (src[3] == '0'))
    {
        if (src[2] == 'y')
            return TYPE_YAY0;

        if (src[2] == 'z')
            return TYPE_YAZ0;
    }
    if ((src[0] == 'A') && (src[1] == 'S') && (src[2] == 'R'))
        return TYPE_ASR;

    return TYPE_NONE;
}

JKRDecompCommand::JKRDecompCommand() {
    OSInitMessageQueue(&mMessageQueue, &mMessageBuf, 1);
    mCb = nullptr;
    _1C = nullptr;
    mThis = this;
    _20 = 0;
}

JKRDecompCommand::~JKRDecompCommand() {}