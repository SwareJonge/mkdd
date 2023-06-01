#include <JSystem/JAudio/JAUInitializer.h>

JAU_JASInitializer::JAU_JASInitializer()
{
    mAudioMemory = 0;
    mAudioMemSize = 0;
    mDvdThreadID = -1;
    mAudioThreadID = -1;
    mHeapSize = 0x1000;
    mDvdThreadPrio = 3;
    mAudioThreadPrio = 2;
    mTrackPools = 128;
    mDSPLevel = 1.0f;
    mChannelMax = 2;
    mChannelPools = 0;
    mDir = nullptr;
}