#include "JSystem/JUtility/JUTDbg.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JKernel/JKRDecomp.h"

// WIP, missing classes

JKRDecomp::JKRDecomp(s32 threadPriority) : JKRThread(0x4000, 16, threadPriority) {
    OSResumeThread(mThread);
}

JKRDecomp::~JKRDecomp() {}

void * JKRDecomp::run() {
    OSMessage msg;
    JKRDecompCommand *decompCmd;
    OSInitMessageQueue(&sMessageQueue, sMessageBuffer, 8);
    while (true) {
        while(true) {
            OSReceiveMessage(&sMessageQueue, &msg, OS_MESSAGE_BLOCK);
            decompCmd = reinterpret_cast<JKRDecompCommand*>(&msg);
            decode(decompCmd->mSrc, decompCmd->mDst, decompCmd->mCompressedSize, decompCmd->mDecompressedSize);
            if (decompCmd->_20 == 0)
                break;
            if (decompCmd->_20 == 1) {
                decompCmd->_24->sendCommand();
            }
        }
        if (decompCmd->mCb == nullptr)
            break;
        decompCmd->mCb();
    }
    if (decompCmd->_1C == nullptr) {
        OSSendMessage(&decompCmd->mMessageQueue, (void*)1, 0);
    }
    else {
        OSSendMessage(decompCmd->_1C, (void *)1, 0);
    }
}

void JKRDecomp::sendCommand(JKRDecompCommand * decompCmd) {
    BOOL msgret = OSSendMessage(&sMessageQueue, decompCmd, 0);
    JUT_ASSERT_MSG(142, msgret, "Decomp MesgBuf FULL!");
}

int JKRDecomp::orderSync(u8 * src, u8 * dst, u32 srcSize, u32 dstSize) {
    JKRDecompCommand *decompCmd = new(JKRHeap::getSystemHeap(), -4);
    decompCmd->mSrc = src;
    decompCmd->mDst = dst;
    decompCmd->mCompressedSize = srcSize;
    decompCmd->mDecompressedSize = dstSize;
    decompCmd->mCb = nullptr; 
    sendCommand(decompCmd);

    OSMessage* msg;
    OSReceiveMessage(&decompCmd->mMessageQueue, msg, OS_MESSAGE_BLOCK);
    delete decompCmd;
}

void JKRDecomp::decode(u8 *src, u8 *dst, u32 srcSize, u32 dstSize) {
    switch(checkCompressed(src)) {
        case TYPE_YAY0:
            decodeSZP(src, dst, srcSize, dstSize);
        case TYPE_YAZ0:
            decodeSZS(src, dst, srcSize, dstSize);
    }
}
