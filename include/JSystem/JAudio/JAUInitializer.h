#ifndef JAUDIO_JAUINITIALIZER_H
#define JAUDIO_JAUINITIALIZER_H

#include <JSystem/JKernel/JKRHeap.h>

struct JAU_JASInitializer
{
    JAU_JASInitializer();
    void initJASystem(JKRSolidHeap *);

    u32 mAudioMemory;      // 00
    u32 mAudioMemSize;     // 04
    u32 mHeapSize;         // 08
    s32 mDvdThreadPrio;    // 0C
    s32 mAudioThreadPrio;  // 10
    int mDvdThreadID;      // 14
    int mAudioThreadID;    // 18
    int mTrackPoolCount;   // 1C
    int mChannelPoolCount; // 20
    u32 mBlockSize;        // 24
    u32 mChannelMax;       // 28
    f32 mDSPLevel;         // 2C
    char *mDir;            // 30
};

struct JAU_JAIInitializer
{
    JAU_JAIInitializer();
    void initJAInterface();

    u32 mSeCount;     // 0
    u32 mSeqCount;    // 4
    u32 mStreamCount; // C
    u32 mChildCount;  // 8
};

#endif