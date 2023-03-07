#include <dolphin/ar/ar.h>
#include <dolphin/ar/arq.h>
#include <dolphin/stl.h>
#include <JSystem/JKernel/JKRAram.h>
#include <JSystem/JKernel/JKRAramPiece.h>
#include <JSystem/JKernel/JKRExpHeap.h>
#include <JSystem/JUtility/JUTDbg.h>

// WIP
JKRAram* JKRAram::sAramObject;

JKRAram* JKRAram::create(u32 aram_audio_buffer_size, u32 aram_audio_graph_size,
                         s32 stream_priority, s32 decomp_priority, s32 piece_priority) {
    if (!sAramObject) {
        sAramObject = new (JKRHeap::getSystemHeap(), 0)
            JKRAram(aram_audio_buffer_size, aram_audio_graph_size, piece_priority);
    }

    JKRCreateAramStreamManager(stream_priority);
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
    OSReport("ARAM size = %08x\n", aramSize);
    mAudioAreaSize = bufSize;
    if(graphSize == 0xffffffff) {
        mGraphAreaSize = aramBase - bufSize - aramSize;
        mUserAreaSize = 0;
    }
    else {
        mGraphAreaSize = graphSize;
        mUserAreaSize = (aramSize - (bufSize + graphSize) - aramBase);
    }

    mAudioArea = ARAlloc(mAudioArea);
    mGraphArea = ARAlloc(mGraphAreaSize);

    if (mUserArea) { // ternary?
        mUserArea = ARAlloc(mUserAreaSize);
    } else {
        mUserArea = nullptr;
    }

    OSReport("ARAM audio area %08x: %08x\n", mAudioArea, mAudioAreaSize);
    OSReport("ARAM graph area %08x: %08x\n", mGraphArea, mGraphAreaSize);
    OSReport("ARAM  user area %08x: %08x\n", mUserArea, mUserAreaSize);

    mAramHeap = new (JKRHeap::getSystemHeap(), 0) JKRAramHeap(mGraphArea, mGraphAreaSize);

}

JKRAram::~JKRAram() {
    sAramObject = nullptr;
    if (mAramHeap)
        delete mAramHeap;
}

void* JKRAram::run(void) {
    int result;
    JKRAMCommand* command;
    JKRAramPiece::JKRAramCommand* message;
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
    JKRHeap* currentHeap = JKRHeap::getCurrentHeap();
    if (currentHeap->getHeapType() == 'EXPH' && groupId >= 0) {
        JKRExpHeap::CMemBlock* block = JKRExpHeap::CMemBlock::getBlock(data);
        block->newGroupId(groupId);
    }
}

// mainRamToAram

// aramToMainRam

// JKRDecompressFromAramToMainRam

// another decompSZS_subroutine

inline u32 nextSrcData_MIN(u32 A, u32 B) {
    if (A > B)
        return B;
    return A;
}

static u8* nextSrcData(u8* current) {
    u8* dest;
    u32 left = (u32)(szpEnd - current);
    if (!IS_NOT_ALIGNED(left, 0x20)) {
        dest = szpBuf;
    } else {
        dest = szpBuf + 0x20 - (left & 0x1f);
    }

    memcpy(dest, current, left);
    u32 transSize = nextSrcData_MIN(transLeft, szpEnd - (dest + left));
    JUT_ASSERT(1403, transSize > 0);

    JKRAramPcs(1, (u32)(srcAddress + srcOffset), (u32)(dest + left), ALIGN_NEXT(transSize, 0x20),
               NULL);
    srcOffset += transSize;
    transLeft -= transSize;

    if (transLeft == 0) {
        srcLimit = (dest + left) + transSize;
    }

    return dest;
}