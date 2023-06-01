#ifndef JAUDIO_JAUINITIALIZER_H
#define JAUDIO_JAUINITIALIZER_H

#include <JSystem/JKernel/JKRHeap.h>

struct JAU_JASInitializer {
    JAU_JASInitializer();
    void initJASystem(JKRSolidHeap *);

    u32 mAudioMemory;
    u32 mAudioMemSize;
    u32 mHeapSize;
    s32 mDvdThreadPrio;
    s32 mAudioThreadPrio;
    int mDvdThreadID;
    int mAudioThreadID;
    u32 mTrackPools;
    u32 mChannelPools;
    u32 mBlockSize;
    u32 mChannelMax;
    f32 mDSPLevel;
    char *mDir;
};

struct JAU_JAIInitializer {
    JAU_JAIInitializer();
    void initJAInterface();
    
    u32 mSeCount;
    u32 mSeqCount;
    u32 mStreamCount;
    u32 mChildCount;
};

#endif