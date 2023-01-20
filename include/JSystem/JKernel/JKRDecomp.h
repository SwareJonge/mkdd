#ifndef JSYSTEM_JKRDECOMP_H
#define JSYSTEM_JKRDECOMP_H

#include "dolphin/OS.h"
#include "JSystem/JKernel/JKRThread.h"
#include "types.h"

enum CompressionMethod {
    TYPE_UNK,
    TYPE_YAY0,
    TYPE_YAZ0,
    TYPE_ASR
};

class JKRDecomp : public JKRThread {
public:
    static void create();

    JKRDecomp(s32);
    virtual ~JKRDecomp();
    virtual void *run();

    static void sendCommand(JKRDecompCommand *);
    static int orderSync(u8 *, u8 *, u32, u32);
    static void decode(u8 *, u8 *, u32, u32);
    static void decodeSZP(u8 *, u8 *, u32, u32);
    static void decodeSZS(u8 *, u8 *, u32, u32);
    static CompressionMethod checkCompressed(unsigned char *);

    static OSMessage sMessageBuffer[8];
    static OSMessageQueue sMessageQueue;
    static JKRDecomp *sDecompObject;
private:
    OSThread * mThread;

};

struct JKRDecompCommand {
    typedef void DecompCmdCb();

    JKRDecompCommand();
    ~JKRDecompCommand();

    u8 _0[4]; // 
    u8 *mSrc; // 4
    u8 *mDst; // 8
    u32 mCompressedSize; // C
    u32 mDecompressedSize; // 10
    DecompCmdCb *mCb;
    JKRDecompCommand *mChild; // 18
    OSMessageQueue * _1C;
    u32 _20;
    void* mAramCmd; // 24
    OSMessageQueue mMessageQueue; // 28
    OSMessage mMessageBuf;
};

#endif