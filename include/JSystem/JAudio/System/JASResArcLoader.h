#ifndef JAUDIO_JASRESARCLOADER_H
#define JAUDIO_JASRESARCLOADER_H

#include <dolphin/os.h>
#include "JSystem/JKernel/JKRArchive.h"

// from pikmin2
typedef void (*LoadCallback)(u32, u32);

namespace JASResArcLoader
{
    size_t getResSize(const JKRArchive *arc, u16 id);
    size_t getResMaxSize(const JKRArchive *arc);
    static void loadResourceCallback(void *);
    int loadResourceAsync(JKRArchive *, u16, u8 *, u32, LoadCallback lCb, u32);
};

enum ResArcMessage
{
    RESARCMSG_Error = -1,
    RESARCMSG_Success = 0,
};

struct TLoadResInfo
{
    inline TLoadResInfo(JKRArchive *archive, u16 id, void *buf, u32 size)
        : mArchive(archive), mID(id), mBuffer(buf), mBufferSize(size), mCallback(0), mCallbackArg(0), mQueue(0)
    {
    }

    JKRArchive *mArchive;   // _00
    u16 mID;                // _04
    void *mBuffer;          // _08
    u32 mBufferSize;        // _0C
    LoadCallback mCallback; // _10
    u32 mCallbackArg;       // _14, arg to pass to mCallback along with readResource result
    OSMessageQueue *mQueue; // _18
};

#endif
