#include <dolphin/ar/ar.h>
#include <dolphin/ar/arq.h>
#include <dolphin/stl.h>
#include <JSystem/JKernel/JKRAram.h>
#include <JSystem/JKernel/JKRAramPiece.h>
#include <JSystem/JKernel/JKRExpHeap.h>
#include <JSystem/JUtility/JUTDbg.h>

// WIP, nothing here has been compiled so probably not a lot matches
static OSMutex decompMutex;

static u8 *szpBuf;
static u8 *szpEnd;
static u8 *refBuf;
static u8 *refEnd;
static u8 *refCurrent;
static u32 srcOffset;
static u32 transLeft;
static u8 *srcLimit;
static u32 srcAddress;
static u32 fileOffset;
static u32 readCount;
static u32 maxDest;
static bool isInitMutex;
static u32 *tsPtr;
static u32 tsArea;

JKRAram* JKRAram::sAramObject;
u32 JKRAram::sSZSBufferSize = 0x400;

JKRAram *JKRAram::create(u32 aram_audio_buffer_size, u32 aram_audio_graph_size,
                         s32 streamPriority, s32 decomp_priority, s32 piece_priority)
{
    if (!sAramObject) {
        sAramObject = new (JKRGetSystemHeap(), 0)
            JKRAram(aram_audio_buffer_size, aram_audio_graph_size, piece_priority);
    }

    JKRCreateAramStreamManager(streamPriority);
    JKRCreateDecompManager(decomp_priority);
    sAramObject->resume();
    return sAramObject;
}

OSMessage JKRAram::sMessageBuffer[4] = {
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

OSMessageQueue JKRAram::sMessageQueue = {0};

JKRAram::JKRAram(u32 bufSize, u32 graphSize, s32 priority) : JKRThread(0x4000, 0x10, priority) {
    u32 aramBase = ARInit(mStackArray, ARRAY_SIZE(mStackArray));
    ARQInit();

    u32 aramSize = ARGetSize();
    JUT_REPORT_MSG("ARAM size = %08x\n", aramSize);
    mAudioMemorySize = bufSize;
    if(graphSize == 0xffffffff) {
        mGraphMemorySize = aramSize - bufSize - aramBase;
        mUserMemorySize = 0;
    }
    else {
        mGraphMemorySize = graphSize;
        mUserMemorySize = (aramSize - (bufSize + graphSize) - aramBase);
    }

    mAudioMemoryPtr = ARAlloc(mAudioMemoryPtr);
    mGraphMemoryPtr = ARAlloc(mGraphMemorySize);

    if (mUserMemoryPtr) { // ternary?
        mUserMemoryPtr = ARAlloc(mUserMemorySize);
    } else {
        mUserMemoryPtr = nullptr;
    }

    JUT_REPORT_MSG("ARAM audio area %08x: %08x\n", mAudioMemoryPtr, mAudioMemorySize);
    JUT_REPORT_MSG("ARAM graph area %08x: %08x\n", mGraphMemoryPtr, mGraphMemorySize);
    JUT_REPORT_MSG("ARAM  user area %08x: %08x\n", mUserMemoryPtr, mUserMemorySize);

    mAramHeap = new (JKRHeap::getSystemHeap(), 0) JKRAramHeap(mGraphMemoryPtr, mGraphMemorySize);
}

JKRAram::~JKRAram() {
    sAramObject = nullptr;
    if (mAramHeap)
        delete mAramHeap;
}

void* JKRAram::run() {
    int result;
    JKRAMCommand* command;
    JKRAramCommand* message;
    OSInitMessageQueue(&sMessageQueue, sMessageBuffer, 4);
    do {
        OSReceiveMessage(&sMessageQueue, (OSMessage*)&message, OS_MESSAGE_BLOCK);
        result = message->_00;
        command = (JKRAMCommand*)message->command;
        delete message;

        switch (result) {
        case 1:
            JKRAramPiece::startDMA(command);
            break;
        }
    } while (true);
}

void JKRAram::checkOkAddress(u8* addr, u32 size, JKRAramBlock* block, u32 param_4) {
    if (!IS_ALIGNED((u32)addr, 0x20) && !IS_ALIGNED(size, 0x20)) {
        JUT_PANIC(219, ":::address not 32Byte aligned.");
    }
}

void JKRAram::changeGroupIdIfNeed(u8* data, int groupId) {
    JKRHeap *currentHeap = JKRGetCurrentHeap();
    if (currentHeap->getHeapType() == 'EXPH' && groupId >= 0) {
        JKRExpHeap::CMemBlock *block = (JKRExpHeap::CMemBlock *)(data + -0x10);
        block->newGroupId(groupId);
    }
}

// i really need/figure out better parameter names
JKRAramBlock *JKRAram::mainRamToAram(u8 *buf, u32 bufSize, u32 alignedSize, JKRExpandSwitch expandSwitch, u32 fileSize, JKRHeap *heap, int id, u32 *pSize)
{
    JKRAramBlock *block = nullptr;
    checkOkAddress(buf, bufSize, nullptr, nullptr);
    if (expandSwitch == Switch_1)
    {
        expandSwitch = (JKRExpandSwitch)(JKRCheckCompressed_noASR(buf) != TYPE_NONE); // generates extra mr
    }
    if (expandSwitch == Switch_1)
    {
        u32 expandSize = JKRDecompExpandSize(buf);
        if (fileSize == 0 || fileSize > expandSize)
        {
            fileSize = ALIGN_NEXT(expandSize, 32);
        }
        if (bufSize == 0)
        {
            block = JKRAllocFromAram(fileSize, JKRAramHeap::AM_Head);
            if (block == nullptr)
                return nullptr;

            block->newGroupID(decideAramGroupId(id));
            bufSize = block->getAddress();
        }
        if (alignedSize == 0 || alignedSize > expandSize)
            alignedSize = ALIGN_NEXT(expandSize, 32);

        if (alignedSize > fileSize)
            alignedSize = fileSize;

        u8 *allocatedMem = (u8 *)JKRAllocFromHeap(heap, fileSize, -32);
        if (allocatedMem == nullptr)
        {
            if (block != nullptr)
            {
                JKRFreeToAram(block);
            }
            block = nullptr;
        }
        else
        {
            JKRDecompress(buf, allocatedMem, fileSize, 0);
            JKRAramPcs(0, (u32)allocatedMem, bufSize, alignedSize, block);
            JKRFreeToHeap(heap, allocatedMem);
            block = block == nullptr ? (JKRAramBlock *)-1 : block; // not sure why it's not
            if (pSize != nullptr)
            {
                *pSize = alignedSize;
            }
        }
    }
    else
    {
        if (fileSize != 0 && alignedSize > fileSize)
            alignedSize = fileSize;
        if (bufSize == 0)
        {
            block = JKRAllocFromAram(alignedSize, JKRAramHeap::AM_Head);
            block->newGroupID(decideAramGroupId(id));
            if (block == nullptr)
                return nullptr;

            bufSize = block->getAddress();
        }

        JKRAramPcs(0, (u32)buf, bufSize, alignedSize, block);
        block = block == nullptr ? (JKRAramBlock *)-1 : block; // not sure why it's not
        if (pSize != nullptr)
            *pSize = alignedSize;
    }
    return block;
}

u8 *JKRAram::aramToMainRam(u32, u8 *, u32, JKRExpandSwitch, u32, JKRHeap *, s32, u32 *)
{
}

// aramSync__7JKRAramFP12JKRAMCommandi is unused but does generate a string

static int JKRDecompressFromAramToMainRam(u32 src, void *dst, u32 srcLength, u32 dstLength, u32 offset, u32 *resourceSize)
{
    BOOL interrupts = OSDisableInterrupts();
    if (isInitMutex == false)
    {
        OSInitMutex(&decompMutex);
        isInitMutex = true;
    }
    OSRestoreInterrupts(interrupts);
    OSLockMutex(&decompMutex);

    u32 szsBufferSize = JKRAram::getSZSBufferSize();
    szpBuf = (u8 *)JKRAllocFromSysHeap(szsBufferSize, 32);
    JUT_ASSERT(1114, szpBuf != 0);

    szpEnd = szpBuf + szsBufferSize;
    if (offset != 0)
    {
        refBuf = (u8 *)JKRAllocFromSysHeap(0x1120, 0);
        JUT_ASSERT(918, refBuf != 0);
        refEnd = refBuf + 0x1120;
        refCurrent = refBuf;
    }
    else
    {
        refBuf = nullptr;
    }
    srcAddress = src;
    srcOffset = 0;
    transLeft = (srcLength != 0) ? srcLength : -1;
    fileOffset = offset;
    readCount = 0;
    maxDest = dstLength;
    tsPtr = (resourceSize != 0) ? resourceSize : &tsArea;
    *tsPtr = 0;

    decompSZS_subroutine(firstSrcData(), (u8 *)dst);
    JKRFree(szpBuf);
    if (refBuf)
    {
        JKRFree(refBuf);
    }

    DCStoreRangeNoSync(dst, *tsPtr);
    OSUnlockMutex(&decompMutex);

    return 0;
}

int decompSZS_subroutine(u8 *src, u8 *dest)
{
    u8 *endPtr;
    s32 validBitCount = 0;
    s32 currCodeByte = 0;
    u32 ts = 0;

    if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0')
    {
        return -1;
    }

    SYaz0Header *header = (SYaz0Header *)src;
    endPtr = dest + (header->length - fileOffset);
    if (endPtr > dest + maxDest)
    {
        endPtr = dest + maxDest;
    }

    src += 0x10;
    do
    {
        if (validBitCount == 0)
        {
            if ((src > srcLimit) && transLeft)
            {
                src = nextSrcData(src);
            }
            currCodeByte = *src;
            validBitCount = 8;
            src++;
        }
        if (currCodeByte & 0x80)
        {
            if (fileOffset != 0)
            {
                if (readCount >= fileOffset)
                {
                    *dest = *src;
                    dest++;
                    ts++;
                    if (dest == endPtr)
                    {
                        break;
                    }
                }
                *(refCurrent++) = *src;
                if (refCurrent == refEnd)
                {
                    refCurrent = refBuf;
                }
                src++;
            }
            else
            {
                *dest = *src;
                dest++;
                src++;
                ts++;
                if (dest == endPtr)
                {
                    break;
                }
            }
            readCount++;
        }
        else
        {
            u32 dist = src[1] | (src[0] & 0x0f) << 8;
            s32 numBytes = src[0] >> 4;
            src += 2;
            u8 *copySource;
            if (fileOffset != 0)
            {
                copySource = refCurrent - dist - 1;
                if (copySource < refBuf)
                {
                    copySource += refEnd - refBuf;
                }
            }
            else
            {
                copySource = dest - dist - 1;
            }
            if (numBytes == 0)
            {
                numBytes = *src + 0x12;
                src += 1;
            }
            else
            {
                numBytes += 2;
            }
            if (fileOffset != 0)
            {
                do
                {
                    if (readCount >= fileOffset)
                    {
                        *dest = *copySource;
                        dest++;
                        ts++;
                        if (dest == endPtr)
                        {
                            break;
                        }
                    }
                    *(refCurrent++) = *copySource;
                    if (refCurrent == refEnd)
                    {
                        refCurrent = refBuf;
                    }
                    copySource++;
                    if (copySource == refEnd)
                    {
                        copySource = refBuf;
                    }
                    readCount++;
                    numBytes--;
                } while (numBytes != 0);
            }
            else
            {
                do
                {
                    *dest = *copySource;
                    dest++;
                    ts++;
                    if (dest == endPtr)
                    {
                        break;
                    }
                    readCount++;
                    numBytes--;
                    copySource++;
                } while (numBytes != 0);
            }
        }
        currCodeByte <<= 1;
        validBitCount--;
    } while (dest < endPtr);
    *tsPtr = ts;
    return 0;
}

u8 *firstSrcData()
{
    srcLimit = szpEnd - 0x19;
    u8 *buf = szpBuf;
    u32 max = (szpEnd - szpBuf);
    u32 transSize = MIN(transLeft, max);

    JKRAramPcs(1, srcAddress + srcOffset, (u32)buf, ALIGN_NEXT(transSize, 32), nullptr);

    srcOffset += transSize;
    transLeft -= transSize;

    if (transLeft == 0)
    {
        srcLimit = buf + transSize;
    }
    return buf;
}

u8 *nextSrcData(u8 *current)
{
    u8 *dest;
    u32 left = (u32)(szpEnd - current);
    if (IS_NOT_ALIGNED(left, 0x20))
        dest = szpBuf + 0x20 - (left & (0x20 - 1));
    else
        dest = szpBuf;

    memcpy(dest, current, left);
    u32 transSize = (u32)(szpEnd - (dest + left));
    if (transSize > transLeft)
        transSize = transLeft;
    JUT_ASSERT(1403, transSize > 0);

    JKRAramPcs(1, (u32)(srcAddress + srcOffset), ((u32)dest + left), ALIGN_NEXT(transSize, 0x20),
               nullptr);
    srcOffset += transSize;
    transLeft -= transSize;

    if (transLeft == 0)
        srcLimit = (dest + left) + transSize;

    return dest;
}