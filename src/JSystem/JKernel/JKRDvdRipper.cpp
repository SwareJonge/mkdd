#include "dolphin/OS.h"
#include "dolphin/stl.h"
#include "dolphin/vi.h"
#include "JSystem/JKernel/JKRArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JKernel/JKRDvdRipper.h"

// WIP, not matching

u8 *firstSrcData();
u8 *nextSrcData(u8*);
int decompSZS_subroutine(unsigned char *, unsigned char *);

static OSMutex decompMutex;

static u8 *szpBuf;
static u8 *szpEnd;
static u8 *refBuf;
static u8 *refEnd;
static u8 *refCurrent;
static u32 srcOffset;
static long transLeft;
static u8 *srcLimit;
static JKRDvdFile *srcFile;
static u32 fileOffset;
static u32 readCount;
static u32 maxDest;
static bool isInitMutex;
static u32 *tsPtr;
static u32 tsArea;

JSUList<JKRDMCommand> JKRDvdRipper::sDvdAsyncList;

namespace JKRDvdRipper {
    bool errorRetry;
    const s32 sSZSBufferSize = 0x400;

    void *loadToMainRAM(const char *fileName, u8 *ptr, JKRExpandSwitch expSwitch, u32 p4, JKRHeap *heap, EAllocDirection allocDirection, u32 startOffset, int *pCompression, u32 *p9) {
        JKRDvdFile dvdFile;
        if (!dvdFile.open(fileName)) {
            return nullptr;
        }
        else {
            return loadToMainRAM(&dvdFile, ptr, expSwitch, p4, heap, allocDirection, startOffset, pCompression, p9);
        }
    }

    void *loadToMainRAM(s32 entryNum, u8 *ptr, JKRExpandSwitch expSwitch, u32 p4, JKRHeap *heap, EAllocDirection allocDirection, u32 startOffset, int *pCompression, u32 *p9) {
        JKRDvdFile dvdFile;
        if (!dvdFile.open(entryNum)) {
            return nullptr;
        }
        else {
            return loadToMainRAM(&dvdFile, ptr, expSwitch, p4, heap, allocDirection, startOffset, pCompression, p9);
        }
    }

    // Not matching, huge size mismatch
    void *loadToMainRAM(JKRDvdFile * jkrDvdFile, u8 * file, JKRExpandSwitch expandSwitch, u32 fileSize, JKRHeap * heap, EAllocDirection allocDirection, u32 startOffset, int * pCompression, u32 *p9) {

        bool hasAllocated = false;
        CompressionMethod compression = TYPE_NONE;
        u8 * mem = nullptr;
        u32 fileSizeAligned = ALIGN_NEXT(jkrDvdFile->getFileSize(), 32);

        u32 uncompressedSize; // investigate, could also be compressed Size

        if (expandSwitch == Switch_1)
        {
            u8 buffer[0x40];
            u8 *bufPtr = (u8 *)ALIGN_NEXT((u32)buffer, 0x20);
            while(true) {
                int readBytes = DVDReadPrio(&jkrDvdFile->mDvdFileInfo, bufPtr, 0x20, 0, 2);
                if (readBytes >= 0)
                    break;

                if (readBytes == -3 || errorRetry == 0) {
                    return nullptr;
                }
                VIWaitForRetrace();
            }
            DCInvalidateRange(bufPtr, 0x20);

            compression = JKRDecomp::checkCompressed(bufPtr);
            if (compression == TYPE_ASR) {
                compression = TYPE_NONE;
            }

            uncompressedSize = read_big_endian_u32(buffer + 4);
        }

        if(pCompression)
            *pCompression = (int)compression;

        if(expandSwitch == Switch_1 && compression != TYPE_NONE) {
            if (fileSize != 0 && fileSize < uncompressedSize) {
                uncompressedSize = fileSize;
            }
            if(file == nullptr) {
                file = (u8 *)JKRHeap::alloc(uncompressedSize, allocDirection == ALLOC_DIR_TOP ? 32 : -32, heap);
                hasAllocated = true;
                if(file == nullptr) {
                    return nullptr;
                }
                if (compression == TYPE_YAY0) {
                    mem = (u8*)JKRHeap::alloc(uncompressedSize, 32, heap);
                    if (mem == nullptr && hasAllocated) {
                        JKRHeap::free(file, nullptr);
                        return nullptr;
                    }
                }
            }
            else {
                if(file == nullptr) { 
                    u32 size = uncompressedSize - startOffset;
                    if(fileSize != 0 && fileSize < size)
                        size = fileSize; // probably a ternary

                    file = (u8 *)JKRHeap::alloc(uncompressedSize, allocDirection == ALLOC_DIR_TOP ? 32 : -32, heap);
                    hasAllocated = true;
                }
                if(file == nullptr)
                    return nullptr;
            }
        }
        if(compression == TYPE_NONE) {
            int readBytes = 0;
            if(fileSize != 0) {
                u8 buffer[0x40]; // maybe create struct
                u8 *bufPtr = (u8 *)ALIGN_NEXT((u32)buffer, 32);
                while (true)
                {
                    readBytes = DVDReadPrio(&jkrDvdFile->mDvdFileInfo, buffer, 32, 0, 2);
                    if (readBytes >= 0)
                        break;

                    if (readBytes == -3 || errorRetry == 0) {
                        return nullptr;
                    }
                    VIWaitForRetrace();
                }
                DCInvalidateRange(buffer, 32);

                compression = JKRDecomp::checkCompressed(buffer);
                if (compression == TYPE_ASR) {
                    compression = TYPE_NONE; // Wtf?
                }
            }
            if (compression == 0 || expandSwitch == Switch_2 || expandSwitch == Switch_0)
            {
                u32 size = uncompressedSize - startOffset;
                if (fileSize != 0 && fileSize < size)
                    size = fileSize; // probably a ternary

                while (true)
                {
                    readBytes = DVDReadPrio(&jkrDvdFile->mDvdFileInfo, file, size, startOffset, 2);
                    if (readBytes >= 0)
                        break;

                    if (readBytes == -3 || errorRetry == 0)
                    {
                        if (hasAllocated)
                            JKRHeap::free(file, nullptr);
                        return nullptr;
                    }
                    VIWaitForRetrace();
                }
            }
            else if (compression == TYPE_YAZ0) {
                JKRDecompressFromDVD(jkrDvdFile, file, uncompressedSize, fileSize, 0, startOffset, p9);
            }
            else {
                JUT_PANIC(323, "Sorry, not applied for SZP archive.");
            }
        }
        else if(compression == TYPE_YAY0) {
            if(startOffset != 0)
                JUT_PANIC(333, "Not support SZP with offset read");

            while (true)
            {
                int readBytes = DVDReadPrio(&jkrDvdFile->mDvdFileInfo, mem, uncompressedSize, 0, 2);
                if (readBytes >= 0) 
                    break;

                if (readBytes == -3 || errorRetry == 0)
                {
                    if (hasAllocated)
                        JKRHeap::free(file, nullptr);

                    JKRHeap::free(mem, nullptr);
                    return nullptr;
                }
                VIWaitForRetrace();
            }
            DCInvalidateRange(mem, fileSizeAligned);
            JKRDecomp::orderSync(mem, file, uncompressedSize, startOffset);
            JKRHeap::free(mem, nullptr);
            if(p9) {
                *p9 = uncompressedSize;
            }
            return file;
        }
        else if(compression == TYPE_YAY0) {
            if (JKRDecompressFromDVD(jkrDvdFile, file, fileSizeAligned, uncompressedSize, startOffset, 0, p9)) {
                if(hasAllocated)
                    JKRHeap::free(file, nullptr);
                
                file = nullptr;
            }
        }
        else {
            if (hasAllocated)
                JKRHeap::free(file, nullptr);

            file = nullptr;
        }
        return file;
    }
}

// doesn't match
int JKRDecompressFromDVD(JKRDvdFile *file, void *p2, unsigned long p3, unsigned long inMaxDest, unsigned long inFileOffset,
                         unsigned long inSrcOffset, unsigned long *inTsPtr)
{
    int interrupts = OSDisableInterrupts();
    if (isInitMutex == false)
    {
        OSInitMutex(&decompMutex);
        isInitMutex = true;
    }
    OSRestoreInterrupts(interrupts);
    OSLockMutex(&decompMutex);
    szpBuf = (u8 *)JKRHeap::sSystemHeap->alloc(JKRDvdRipper::sSZSBufferSize, -0x20);
    szpEnd = szpBuf + JKRDvdRipper::sSZSBufferSize;
    if (inFileOffset != 0)
    {
        refBuf = (u8 *)JKRHeap::sSystemHeap->alloc(0x1120, -4);
        refEnd = refBuf + 0x1120;
        refCurrent = refBuf;
    }
    else
    {
        refBuf = nullptr;
    }
    transLeft = p3 - inSrcOffset;
    readCount = 0;
    if (inTsPtr == 0)
    {
        inTsPtr = &tsArea;
    }
    srcOffset = inSrcOffset;
    srcFile = file;
    fileOffset = inFileOffset;
    maxDest = inMaxDest;
    tsPtr = inTsPtr;
    *inTsPtr = 0;
    u8 *data = firstSrcData();
    int result = (data != nullptr) ? decompSZS_subroutine(data, (u8 *)p2) : -1;
    JKRHeap::free(szpBuf, nullptr);
    if (refBuf)
    {
        JKRHeap::free(refBuf, nullptr);
    }
    DCStoreRangeNoSync(p2, *tsPtr);
    OSUnlockMutex(&decompMutex);
    return result;
}

// Might match, however i expect regswaps here
int decompSZS_subroutine(u8 *src, u8 *dest)
{
    int validBitCount = 0;
    u32 currCodeByte = 0;
    u32 ts = 0;

    if (src[0] != 'Y' || src[1] != 'a' || src[2] != 'z' || src[3] != '0')
    {
        return -1;
    }

    SYaz0Header *header = (SYaz0Header *)src;
    u8 *endPtr = dest + (header->length - fileOffset);
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
                if (!src)
                {
                    return -1;
                }
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
            u32 dist = ((src[0] & 0x0f) << 8) | src[1];
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

// doesn't match, target = 0xC4 current: 0xBC(maybe try different build flags)
u8 *firstSrcData()
{
    srcLimit = szpEnd - 0x19;
    u32 byteCount = MIN(transLeft, (u32)(szpEnd - szpBuf));
    // u32 byteCount;
    // if (transLeft < byteCount) {
    // 	byteCount = transLeft;
    // }
    while (true)
    {
        int result = DVDReadPrio(&srcFile->mDvdFileInfo, szpBuf, byteCount, srcOffset, 2);
        if (0 <= result)
        {
            break;
        }
        if (result == -3 || !JKRDvdRipper::isErrorRetry())
        {
            return nullptr;
        }
        VIWaitForRetrace();
    }
    DCInvalidateRange(szpBuf, byteCount);
    srcOffset += byteCount;
    transLeft -= byteCount;
    return szpBuf;
}

// Target: 0x138 Current: 0xF4
u8 *nextSrcData(u8 *param_0)
{
    u32 size = szpEnd - param_0;
    u8 *dest = IS_NOT_ALIGNED(size, 0x20) ? szpBuf + 0x20 - (size & (0x20 - 1)) : szpBuf;
    memcpy(dest, param_0, size);
    u8 *end = dest + size;
    u32 transSize = szpEnd - end;
    if (transSize > transLeft)
    {
        transSize = transLeft;
    }
    while (true)
    {
        s32 result = DVDReadPrio(&srcFile->mDvdFileInfo, end, transSize, srcOffset, 2);
        if (result >= 0)
        {
            break;
        }
        // bug: supposed to call isErrorRetry, but didn't
        if (result == -3 || !JKRDvdRipper::isErrorRetry)
        {
            return NULL;
        }
        VIWaitForRetrace();
    }
    DCInvalidateRange(end, transSize);
    srcOffset += transSize;
    transLeft -= transSize;
    if (transLeft == 0)
    {
        srcLimit = end + transSize;
    }
    return dest;
}