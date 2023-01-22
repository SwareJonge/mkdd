#ifndef JSYSTEM_JKRDECOMP_H
#define JSYSTEM_JKRDECOMP_H

#include "dolphin/OS.h"
#include "JSystem/JKernel/JKRThread.h"
#include "types.h"

enum CompressionMethod {
    TYPE_NONE,
    TYPE_YAY0,
    TYPE_YAZ0,
    TYPE_ASR
};

#define JKRDECOMP_SYNC_BLOCKING 0
#define JKRDECOMP_SYNC_NON_BLOCKING 1

class JKRAMCommand;
struct JKRDecompCommand {
    typedef void (*AsyncCallback)(u32);

    JKRDecompCommand();
    ~JKRDecompCommand();

    u8 _0[4];       // perhaps file magic
    u8 *mSrcBuffer; // 4
    u8 *mDstBuffer; // 8
    u32 mSrcLength; // C
    u32 mDstLength; // 10
    AsyncCallback mCb;
    JKRDecompCommand *mThis; // 18
    OSMessageQueue *_1C;
    s32 _20;
    JKRAMCommand *mAramCmd;       // 24
    OSMessageQueue mMessageQueue; // 28
    OSMessage mMessageBuf;
};

class JKRDecomp : public JKRThread {
    JKRDecomp(s32);
    virtual ~JKRDecomp();
    virtual void *run();

public:
    static JKRDecomp *create(s32);
    static JKRDecompCommand *prepareCommand(u8 *, u8 *, u32, u32, JKRDecompCommand::AsyncCallback);
    static void sendCommand(JKRDecompCommand *);
    static bool sync(JKRDecompCommand *, int);
    static JKRDecompCommand *orderAsync(u8 *, u8 *, u32, u32, JKRDecompCommand::AsyncCallback);
    static bool orderSync(u8 *, u8 *, u32, u32);
    static void decode(u8 *, u8 *, u32, u32);
    static void decodeSZP(u8 *, u8 *, u32, u32);
    static void decodeSZS(u8 *, u8 *, u32, u32);
    static CompressionMethod checkCompressed(unsigned char *);

    static OSMessage sMessageBuffer[8];
    static OSMessageQueue sMessageQueue;
    static JKRDecomp *sDecompObject;
};

#endif